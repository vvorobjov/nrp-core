//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2023 NRP Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This project has received funding from the European Union’s Horizon 2020
// Framework Programme for Research and Innovation under the Specific Grant
// Agreement No. 945539 (Human Brain Project SGA3).
//

#include "nrp_nest_server_engine/nrp_client/nest_engine_server_nrp_client.h"
#include "nrp_json_engine_protocol/datapack_interfaces/json_datapack.h"

#include "nrp_general_library/utils/nrp_exceptions.h"
#include "nrp_general_library/utils/restclient_setup.h"
#include "nrp_nest_server_engine/config/cmake_constants.h"
#include "nrp_general_library/config/cmake_constants.h"
#include "nrp_general_library/utils/utils.h"

#include <chrono>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <utility>
#include <sstream>

/*!
 * \brief Anonymous namespace containing helper functions
 */
namespace
{
    /*!
     * \brief Returns source of the brain file as string
     *
     * The source is supposed to be send to the NEST server, which will execute it.
     *
     * This is a helper function of initialize().
     *
     * \param brainFileName Name of the initialization file
     * \return Source code of brain file as string
     */
    std::string readBrainFile(const std::string & brainFileName)
    {
        NRP_LOGGER_TRACE("{} called", __FUNCTION__);

        std::string initCode;
        {
            std::ifstream initFile(brainFileName);
            initCode.assign((std::istreambuf_iterator<char>(initFile) ),
                            (std::istreambuf_iterator<char>()       ) );
        }

        return initCode;
    }

    /*!
     * \brief Checks if engine process is still alive
     *
     * This is a helper function of initialize().
     *
     * \param process Pointer to process object
     * \throws NRPException When process has stopped
     */
    void checkProcessStatus(const ProcessLauncherInterface::unique_ptr & process)
    {
        if(process->getProcessStatus() == ProcessLauncherInterface::ENGINE_RUNNING_STATUS::STOPPED)
        {
            throw NRPException::logCreate("Nest Engine process stopped unexpectedly before the init file could be sent");
        }
    }

    /*!
     * \brief Checks if datapack can be processed by the engine
     *
     * \param[in] datapack     DataPack ID structure to be verified
     * \param[in] engineName Name of the current engine
     *
     * \return True if datapack may be processed by the engine, false otherwise
     */
    bool isDataPackTypeValid(const DataPackIdentifier & datapack, const std::string & engineName)
    {
        // Check if type matches the NestServerDataPack type
        // Skip the check if type was not set

        const bool isTypeValid = (datapack.Type.empty() || datapack.Type == JsonDataPack::getType());

        return (isTypeValid && datapack.EngineName == engineName);
    }

    /*!
     * \brief Returns command execution timeout as SimulationTime
     *
     * This is a helper function of initialize().
     *
     * \return Command execution timeout for this engine
     */
    SimulationTime processCommandTimeout(const float commandTimeout)
    {
        auto timeout = SimulationTime::max();

        if(commandTimeout > 0.0f)
        {
            timeout = toSimulationTime<float, std::ratio<1,1> >(commandTimeout);
        }

        return timeout;
    }

    /*!
     * \brief Extracts populations from NEST server response to brain file execution
     *
     * The NEST server is supposed to return a dictionary names 'populations'. The dictionary
     * should contain a mapping of population (datapack) names to lists of IDs.
     *
     * This is a helper function of initialize().
     *
     * \param[in]  data        Body part of the response from NEST server
     * \param[out] populations Populations mapping structure that will be filled by the function
     */
    void extractPopulations(const std::string & data, NestEngineServerNRPClient::population_mapping_t * populations)
    {
        const auto & respJson = nlohmann::json::parse(data);
        for (const auto & pop_item: respJson.at("data").at("populations").items())
        {
            NRP_LOGGER_TRACE("{}: \"{}\" population found. {} ", __FUNCTION__, pop_item.key(), pop_item.value().dump());
            populations->insert({pop_item.key(), pop_item.value().dump()});
        }
    }


    /*!
     * \brief Extracts connections from NEST server response to brain file execution
     *
     * The NEST server is supposed to return a dictionary names 'connections'.
     * The dictionary should contain a mapping of connection (datapack) names to "source" and "target" population names
     * The datapack will be updated by calling GetConnection(source, target).
     *
     * This is a helper function of initialize().
     *
     * \param[in]  data        Body part of the response from NEST server
     * \param[out] getConnectionsPopulationToArgs Connection names to arguments mapping that will be filled by the function
     */
    void extractConnections(const std::string & data, NestEngineServerNRPClient::get_connection_population_mapping_t * getConnectionsPopulationToArgs)
    {
        const auto & respJson = nlohmann::json::parse(data);

        if(!respJson.at("data").contains("connections")) {
            NRPLogger::debug("NestEngineServerNRPClient::initialize(...) No connections variable found in brain script");
            return;
        }

        // connections variable has been specified in brain script
        for (const auto & conn_item: respJson["data"]["connections"].items())
        {
            const auto & connection_name = conn_item.key();
            const auto & connection_args = conn_item.value();

            // pop item is "connection_name": {"source": "pop_name", "target": "other_pop_name"}
            getConnectionsPopulationToArgs->insert({connection_name,
                                                    std::make_pair(connection_args.at("source"),
                                                                   connection_args.at("target"))});
        }
    }

    /*!
     * \brief Generic REST call function
     *
     * \param url   URL to query
     * \param ctype Content type as string
     * \param data  HTTP POST body
     * \throws NRPException On response code different from 200
     * \return Response body as string
     */
    std::string nestGenericCall(const std::string & url, const std::string & ctype, const std::string & data)
    {
        auto resp = RestClient::post(url, ctype, data);

        if(resp.code != 200)
        {
            throw NRPException("REST call to \"" + url + "\" failed with code " + std::to_string(resp.code));
        }

        return resp.body;
    }

    /*!
     * \brief Sends SetStatus request to NEST server
     *
     * \param serverAddress Address of the NEST server
     * \param argsStr Comma-separated arguments list to SetStatus as string (e.g. "arg1, arg2")
     * \param kwargsStr Comma-separated keyword arguments to SetStatus as string (e.g. (" "key1": value1, "key2": value2 "))

     */
    void nestSetStatus(const std::string & serverAddress, const std::string & argsStr, const std::string & kwargsStr)
    {
        // equivalent to a NESTClient call client.SetStatus(arg1, arg2, key1=value1, key2=value2})
        std::stringstream data;
        data << "{" << R"("args": )" << "[" << argsStr << "], " << kwargsStr << "}";

        nestGenericCall(serverAddress + "/api/SetStatus","application/json", data.str());
    }

    /*!
     * \brief Sends GetStatus request to NEST server
     *
     * \param serverAddress Address of the NEST server
     * \param argsStr Comma-separated arguments list to GetStatus as string (e.g. "arg1, arg2"))
     * \return Response from GetStatus as string
     */
    std::string nestGetStatus(const std::string & serverAddress, const std::string & argsStr)
    {
        //TODO kwargs support so to select keys to get.
        // Datapacks can't pass that info to engine clients yet.
        // equivalent to a NESTClient call client.GetStatus(arg1, arg2, ..)
        std::stringstream data;
        data << "{" << R"("args": )" << "[" << argsStr << "]" << "}";

        return nestGenericCall(serverAddress + "/api/GetStatus", "application/json", data.str());
    }

    /*!
     * \brief Sends GetConnections request to NEST server
     *
     * \param serverAddress Address of the NEST server
     * \param source_ids_str The IDs of the pre-synaptic population as a JSON list
     * \param target_ids_str The IDs of the post-synaptic population as a JSON list
     * \return Response from GetConnections as string
     */
    std::string nestGetConnections(const std::string & serverAddress, const std::string & source_ids_str, const std::string & target_ids_str)
    {
        //TODO kwargs support so to select keys to get.
        // Datapacks can't pass that info to engine clients yet.
        // equivalent to a NESTClient call client.GetConnections(source=source_ids_str, target=target_ids_str})
        std::stringstream data;
        data << "{ " << R"("source": )" << source_ids_str << ", " << R"("target": )" << target_ids_str << " }";
        return nestGenericCall(serverAddress + "/api/GetConnections", "application/json", data.str());
    }

    /*!
     * \brief Sends ResetKernel request to NEST server
     *
     * \param serverAddress Address of the NEST server
     */
    void nestResetKernel(const std::string & serverAddress)
    {
        nestGenericCall(serverAddress + "/api/ResetKernel", "text/plain", "");
    }

    /*!
     * \brief Sends Run request to NEST server
     *
     * \param serverAddress Address of the NEST server
     * \param timeStep Step of the simulation in milliseconds
     */
    void nestSimulate(const std::string & serverAddress, const float timeStep)
    {
        nestGenericCall(serverAddress + "/api/Simulate", "application/json", "[" + std::to_string(timeStep) + "]");
    }

    /*!
     * \brief Sends GetKernelStatus request to NEST server
     *
     * \param serverAddress Address of the NEST server
     * \param data Arguments for GetKernelStatus
     * \return Response from GetKernelStatus as string
     */
    std::string nestGetKernelStatus(const std::string & serverAddress, const std::string & data)
    {
        return nestGenericCall(serverAddress + "/api/GetKernelStatus", "application/json", data);
    }

    /*!
     * \brief Sends exec request to NEST server
     *
     * \param serverAddress Address of the NEST server
     * \param data Python code to be executed by the server
     */
    std::string nestExec(const std::string & serverAddress, const std::string & data)
    {
        return nestGenericCall(serverAddress + "/exec", "application/json", data);
    }
}

NestEngineServerNRPClient::NestEngineServerNRPClient(nlohmann::json &config, ProcessLauncherInterface::unique_ptr &&launcher)
    : EngineClient(config, std::move(launcher))
{
    // Disable RestrictedPython
    this->engineConfig().at("EngineEnvParams").get<std::vector<std::string>>().emplace_back("NEST_SERVER_RESTRICTION_OFF=1");

    // set engine command
    int n_mpi = this->engineConfig().at("MPIProcs").get<int>();
    if(n_mpi <= 1)
        setDefaultProperty<std::string>("EngineProcCmd", NRP_NEST_SERVER_EXECUTABLE_PATH);
    else {
        std::string mpi_cmd = "mpirun -np " + std::to_string(n_mpi) + " " + NRP_NEST_SERVER_MPI_EXECUTABLE_PATH;
        setDefaultProperty<std::string>("EngineProcCmd", mpi_cmd);
    }

    // address
    if(!this->engineConfig().contains("NestServerPort"))
        setDefaultProperty<int>("NestServerPort", getFreePort(this->engineConfig().at("NestServerHost").get<std::string>()));

    RestClientSetup::ensureInstance();

    this->_serverAddress = this->engineConfig().at("NestServerHost").get<std::string>() + ":" +
            std::to_string(this->engineConfig().at("NestServerPort").get<int>());
}

NestEngineServerNRPClient::~NestEngineServerNRPClient()
{}

void NestEngineServerNRPClient::initialize()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    const auto initCode    = readBrainFile(this->engineConfig().at("NestInitFileName"));
    const auto timeout     = processCommandTimeout(this->engineConfig().at("EngineCommandTimeout"));
    const auto startTime   = std::chrono::system_clock::now();
    bool       initSuccess = false;

    // Try sending data to given address. Continue until timeout
    do
    {
        // Check that process is still running

        checkProcessStatus(this->_process);

        // Try sending and executing initFile contents

        std::string response;
        try
        {
            response = nestExec(this->serverAddress(), nlohmann::json({{"source", initCode}, {"return", {"populations", "connections"}}}).dump());
        }
        catch(NRPException &e)
        {
            throw NRPException::logCreate(e, "Failed to execute init file \"" + this->engineConfig().at("NestInitFileName").get<std::string>() +
                    "\": " + response);
        }
        catch(std::exception &e)
        {
            // Server unreachable, retry if server still starting up

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        // Extract all populations from the response and cache them in the populations map

        extractPopulations(response, &this->_populations);

        // Extract all connections from the response and cache them in the connections map
        try
        {
            extractConnections(response, &this->_getConnectionsPopulationToArgs);
        }
        catch(std::exception &e)
        {
            throw NRPException::logCreate(e, "Failed to extract \"connections\" from nest server response."
                                             "The format of the \"connections\" dictionary should be"
                                             "{\"connection_name\": {\"source\": \"a_pop_name\", \"target\": \"another_pop_name\"}}.");
        }

        // Exit the loop

        initSuccess = true;
        break;
    }
    while(std::chrono::system_clock::now() - startTime > timeout);

    // Check if timeout reached

    if(!initSuccess)
    {
        throw NRPException::logCreate("Failed to initialize Nest server. Received no response before timeout reached");
    }
    else
    {
        NRPLogger::info("Nest server is initialized.");
    }

    // Get simulation resolution and cache it

    this->_simulationResolution = std::stof(nestGetKernelStatus(this->serverAddress(), "[\"resolution\"]"));

    NRPLogger::debug("NestEngineServerNRPClient::initialize(...) completed with no errors.");
}

void NestEngineServerNRPClient::reset(){

    try
    {
        nestResetKernel(this->serverAddress());
    }
    catch (std::exception &e)
    {
        throw NRPException::logCreate(e, "Failed to execute NEST ResetKernel at reset");
    }

    this->_populations.clear();

    try
    {
        this->initialize();
    }
    catch (std::exception &e)
    {
        throw NRPException::logCreate(e, "Failed re-initialize NEST during reset");
    }
}

void NestEngineServerNRPClient::shutdown()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    // Empty
    // Revert https://bitbucket.org/hbpneurorobotics/nrp-core/pull-requests/96 changes
    // When nest-server is launched separately with EmptyLaunchCommand, then this "hacked" shutdown gives an error
    // The proper shutdown of the server instances should be handled correctly by the launcher,
    // which knows how it was started
}

const std::string & NestEngineServerNRPClient::getDataPackIdList(const std::string & datapackName) const
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    // Get list of datapack IDs mapped to this datapack name
    try {
        return this->_populations.at(datapackName);
    }
    catch (const std::out_of_range&) {
        throw NRPException::logCreate("DataPack \"" + datapackName + "\" not in populations map");
    }
}

datapacks_vector_t NestEngineServerNRPClient::getDataPacksFromEngine(const datapack_identifiers_set_t &datapackIdentifiers)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    datapacks_vector_t retVals;

    for(const auto &devID : datapackIdentifiers)
    {
        if(isDataPackTypeValid(devID, this->engineName()))
        {
            const auto datapackName = devID.Name;
            std::string response;

            // Request status of the IDs from the list

            try {
                if (this->_populations.count(datapackName) > 0) {
                    // it's a population use GetStatus
                    response = nestGetStatus(this->serverAddress(), getDataPackIdList(datapackName));

                } else if (this->_getConnectionsPopulationToArgs.count(datapackName) > 0) {
                        // it's a connection use GetConnection
                        // fetch the parameters, i.e. "source" and "target" population IDs
                        auto getConnectionArgs = this->_getConnectionsPopulationToArgs.at(datapackName);

                        response = nestGetConnections(this->serverAddress(),
                                                      getDataPackIdList(getConnectionArgs.first),
                                                      getDataPackIdList(getConnectionArgs.second));
                } else {
                    throw NRPException::logCreate("Datapack \"" + datapackName + "\" is not defined in brain script.");
                }
            }
            catch(std::exception& e) {
                throw NRPException::logCreate(e, "Failed to get NEST status for datapack \"" + datapackName + "\"");
            }

            // Extract datapack details from the body
            // Response from GetStatus is an array of JSON objects, which contains datapack parameters

            retVals.push_back(DataPackInterfaceConstSharedPtr(new JsonDataPack(devID.Name, devID.EngineName, new nlohmann::json(nlohmann::json::parse(response)))));
        }
    }

    return retVals;
}

void NestEngineServerNRPClient::sendDataPacksToEngine(const datapacks_set_t &datapacksArray)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    for(auto datapack : datapacksArray)
    {
        if(isDataPackTypeValid(datapack->id(), this->engineName()))
        {
            // Send command along with parameters to nest

            const auto datapackName = datapack->name();

            const std::string kwargsStr = "\"params\": " + ((JsonDataPack const *)datapack.get())->getData().dump();

            try
            {
                // SetStatus(args, kwargs)
                nestSetStatus(this->serverAddress(), getDataPackIdList(datapackName), kwargsStr);
            }
            catch(std::exception& e)
            {
                throw NRPException::logCreate(e, "Failed to set NEST status for datapack\"" + datapackName + "\"");
            }
        }
    }
}

SimulationTime NestEngineServerNRPClient::runLoopStepCallback(SimulationTime timeStep)
{
    // According to the NEST API documentation, Run accepts time to simulate in milliseconds and floating-point format
    const double runTimeMsRounded = getRoundedRunTimeMs(timeStep, this->_simulationResolution);

    nestSimulate(this->serverAddress(), runTimeMsRounded);

    return toSimulationTime<float, std::milli>(std::stof(nestGetKernelStatus(this->serverAddress(), "[\"biological_time\"]")));
}

std::string NestEngineServerNRPClient::serverAddress() const
{
    return this->_serverAddress;
}

const std::vector<std::string> NestEngineServerNRPClient::engineProcStartParams() const
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    
    std::vector<std::string> startParams;

    // Add Server address
    int port = this->engineConfig().at("NestServerPort");
    int n_mpi = this->engineConfig().at("MPIProcs").get<int>();
    if(n_mpi <= 1) {
        startParams.push_back("start");
        startParams.push_back("-o");
        startParams.push_back("-h");
        startParams.push_back(this->engineConfig().at("NestServerHost"));
        startParams.push_back("-p");
        startParams.push_back(std::to_string(port));
        // This parameter is ignored by nest-server if -o is specified
        // And --plugin python3 is used by default at uwsgi launch
        startParams.push_back("-P");
        startParams.push_back("python3");
    }
    else {
        startParams.push_back("--host");
        startParams.push_back(this->engineConfig().at("NestServerHost"));
        startParams.push_back("--port");
        startParams.push_back(std::to_string(port));
    }

    return startParams;
}

// EOF