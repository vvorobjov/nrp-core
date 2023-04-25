//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2021 NRP Team
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

#include "edlut_grpc_engine/engine_server/edlut_grpc_server.h"
#include "edlut_grpc_engine/engine_server/edlut_grpc_datapack_controller.h"
#include <sstream>


SimulationTime EdlutGrpcServer::_simulationTime = SimulationTime::zero();

EdlutGrpcServer::EdlutGrpcServer(const std::string &serverAddress,
                                     const std::string &engineName,
                                                   const std::string &protobufPluginsPath,
                                                   const nlohmann::json &protobufPlugins)
    : EngineGrpcServer(serverAddress, engineName, protobufPluginsPath, protobufPlugins),
    _engineName(engineName)
{
    this->_inputSpikeDriver = std::make_shared<ArrayInputSpikeDriver>();
    this->_outputSpikeDriver = std::make_shared<ArrayOutputSpikeDriver>();
}

SimulationTime EdlutGrpcServer::runLoopStep(SimulationTime timeStep)
{

    NRP_LOGGER_TRACE("EdlutGrpcServer::runLoopStep called");

    this->_simulationTime += timeStep;

    /* Run edlut simulator */
    this->_edlutSimul->RunSimulationSlot(fromSimulationTime<float, std::ratio<1>>(EdlutGrpcServer::_simulationTime)+this->_sensorialDelay);

    return this->_simulationTime;
}

void EdlutGrpcServer::initialize(const nlohmann::json &data, EngineGrpcServer::lock_t & datapackLock)
{
    NRPLogger::info("Initializing EDLUT gRPC Engine");
    this->_engineConfig = data;
    NRPLogger::debug(this->_engineConfig.dump(4));

    initEdlutEngine(data, datapackLock);
}

void EdlutGrpcServer::initEdlutEngine(const nlohmann::json &data, EngineGrpcServer::lock_t & /*datapackLock*/)
{
    // Create Edlut sim objects
    try{
        auto networkFile = data.at("NetworkFile").get<std::string>();
        auto weightFile = data.at("WeightsFile").get<std::string>();
        auto simTimestep = data.at("EngineTimestep").get<double>();
        auto numThreads = data.at("NumThreads").get<int>();
        this->_sensorialDelay = data.at("SensorialDelay").get<float>();

        this->_edlutSimul = std::make_shared<Simulation> (networkFile.c_str(), weightFile.c_str(), std::numeric_limits<double>::max(), simTimestep, numThreads);

        // Initialize simulation
        this->_edlutSimul->AddInputSpikeDriver(this->_inputSpikeDriver.get());
        this->_edlutSimul->AddOutputSpikeDriver(this->_outputSpikeDriver.get());

        this->_edlutSimul->InitSimulation();
    }
    catch (EDLUTException Exc){
        throw NRPException::logCreate("Failed to initialize \""+this->_engineName+"\" Engine: " + Exc.GetErrorMsg());
    }

    // Register datapack
    this->registerDataPackNoLock("spikes_datapack", new EdlutGrpcDataPackController("spikes_datapack", this->_engineName, this->_edlutSimul, this->_inputSpikeDriver, this->_outputSpikeDriver));
    NRPLogger::info("DataPack \"spikes_datapack\" was registered");

    EdlutGrpcServer::_simulationTime = SimulationTime::zero();
    this->_initRunFlag = true;
}

void EdlutGrpcServer::shutdown(const nlohmann::json &/*data*/)
{
    NRPLogger::info("Shutting down EDLUT simulation");
    this->_shutdownFlag = true;
}

void EdlutGrpcServer::reset()
{
    NRPLogger::info("Resetting EDLUT simulation");

    EngineGrpcServer::lock_t lock(this->_datapackLock);
    this->clearRegisteredDataPacks();
    this->initEdlutEngine(this->_engineConfig, lock);
}


// EOF
