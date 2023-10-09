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

#include "nrp_json_engine_protocol/engine_server/engine_json_server.h"
#include "nrp_json_engine_protocol/config/engine_json_config.h"
#include "nrp_json_engine_protocol/nrp_client/engine_json_registration_server.h"
#include "nrp_general_library/utils/restclient_setup.h"

#include <nlohmann/json.hpp>
#include <pistache/listener.h>

using json = nlohmann::json;

EngineJSONServer::EngineJSONServer(const std::string &engineAddress, const std::string &engineName, const std::string &clientAddress)
    : _serverAddress(engineAddress),
      _router(EngineJSONServer::setRoutes(this)),
      _engineName(engineName),
      _loggerCfg(engineName)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    RestClientSetup::ensureInstance();

    // Try to bind to preferred address. If that fails, try incremental ports
    const auto addrParser = Pistache::AddressParser(engineAddress);

    const auto maxTries = std::max<decltype(EngineJSONConfigConst::MaxAddrBindTries)>(1, EngineJSONConfigConst::MaxAddrBindTries);
    int i = 0;
    do
    {
        const Pistache::Address newEngineAddr(addrParser.rawHost(), static_cast<uint16_t>(std::stoi(addrParser.rawPort())+i));
        this->_serverAddress = newEngineAddr.host() + ":" + newEngineAddr.port().toString();

        try
        {
            this->_pEndpoint = enpoint_ptr_t(new Pistache::Http::Endpoint(newEngineAddr));

            // Add routes to endpoint
            this->_pEndpoint->setHandler(this->_router.handler());

            // Try to bind to new addr
            this->startServerAsync();
        }
        catch(std::runtime_error &)
        {
            // Throw error only after maxTries attempts
            if(++i >= maxTries)
                throw;

            this->_pEndpoint.release();
        }
    }
    while(this->_pEndpoint == nullptr);

    // Register port
    if(!this->_engineName.empty())
    {
        if(!EngineJSONRegistrationServer::sendClientEngineRequest(clientAddress, this->_engineName, this->_serverAddress, 20, 1))
            throw NRPException::logCreate(std::string("Error while trying to register engine \"") + this->_engineName + "\" at " + clientAddress);
    }

    NRPLogger::info("EngineJSONServer {} has been created", this->_engineName);
}

EngineJSONServer::~EngineJSONServer()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    this->shutdownServer();
}

bool EngineJSONServer::isServerRunning() const
{
    return this->_serverRunning;
}

void EngineJSONServer::startServerAsync()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    if(!this->_serverRunning)
    {
        std::unique_lock devLock(this->_datapackLock);
        this->_pEndpoint->serveThreaded();
        this->_serverRunning = true;
    }
}

void EngineJSONServer::startServer()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    if(!this->_serverRunning)
    {
        this->_serverRunning = true;

        // Start server and wait for some shutdown command
        this->_pEndpoint->serve();

        this->_serverRunning = false;
    }
}

void EngineJSONServer::shutdownServer()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    if(this->_serverRunning)
    {
        EngineJSONServer::lock_t devLock(this->_datapackLock, std::defer_lock);
        if(!devLock.try_lock_for(ShutdownWaitTime))
            throw NRPException::logCreate("Couldn't get datapack lock for shutdown");

        this->_pEndpoint->shutdown();
        this->_serverRunning = false;
    }
}

uint16_t EngineJSONServer::serverPort() const
{   
    if(this->_serverRunning)
        return this->_pEndpoint->getPort();

    return 0;
}

std::string EngineJSONServer::serverAddress() const
{
    return this->_serverAddress;
}

void EngineJSONServer::registerDataPack(const std::string &datapackName, JsonDataPackController *interface)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    EngineJSONServer::lock_t lock(this->_datapackLock);
    return this->registerDataPackNoLock(datapackName, interface);
}

void EngineJSONServer::registerDataPackNoLock(const std::string &datapackName, JsonDataPackController *interface)
{   
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    
    this->_datapacksControllers.emplace(datapackName, interface);
}

void EngineJSONServer::clearRegisteredDataPacks()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    // Do not lock scope. This method is called from the route handlers, which should already have locked down access.
    //EngineJSONServer::lock_t lock(this->_datapackLock);

    this->_datapacksControllers.clear();
}

nlohmann::json EngineJSONServer::getDataPackData(const nlohmann::json &reqData)
{
    // Prevent other datapack reading/setting calls as well as loop execution
    EngineJSONServer::lock_t lock(this->_datapackLock);

    json jres;

    for(auto curRequest = reqData.begin(); curRequest != reqData.end(); ++curRequest)
    {
        const auto &devName = EngineJSONServer::getIteratorKey(curRequest);
        const auto devInterface = this->_datapacksControllers.find(devName);

        if(devInterface != this->_datapacksControllers.end())
        {
            auto dev = devInterface->second->getDataPackInformation();

            if(dev == nullptr)
            {
                jres.update(devInterface->second->getEmptyDataPack());
            }
            else
            {
                jres.update(*dev);
            }
        }
        else
        {
            jres[devName] = nlohmann::json();
        }
    }

    return jres;
}

void EngineJSONServer::setDataPackData(const nlohmann::json &reqData)
{
    // Prevent other datapack reading/setting calls as well as loop execution
    EngineJSONServer::lock_t lock(this->_datapackLock);

    for(nlohmann::json::const_iterator devDataIterator = reqData.begin(); devDataIterator != reqData.end(); ++devDataIterator)
    {
        const std::string &devName = EngineJSONServer::getIteratorKey(devDataIterator);
        const auto devInterface = this->_datapacksControllers.find(devName);

        try
        {
            if(devInterface != this->_datapacksControllers.end())
                devInterface->second->handleDataPackData(*devDataIterator);
        }
        catch(std::exception &e)
        {
            throw NRPException::logCreate(e, "Couldn't handle datapack " + devName);
        }
    }

}

bool EngineJSONServer::shutdownFlag()
{
    std::lock_guard<std::mutex> shutdown_lock(this->_shutdown_mutex);
    return this->_shutdownFlag;
}

Pistache::Rest::Router EngineJSONServer::setRoutes(EngineJSONServer *server)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    Pistache::Rest::Router router;
    Pistache::Rest::Routes::Post(router, EngineJSONServer::GetDataPackInformationRoute.data(), Pistache::Rest::Routes::bind(&EngineJSONServer::getDataPackDataHandler, server));
    Pistache::Rest::Routes::Post(router, EngineJSONServer::SetDataPackRoute.data(),            Pistache::Rest::Routes::bind(&EngineJSONServer::setDataPackProcessorr, server));
    Pistache::Rest::Routes::Post(router, EngineJSONServer::RunLoopStepRoute.data(),          Pistache::Rest::Routes::bind(&EngineJSONServer::runLoopStepHandler, server));
    Pistache::Rest::Routes::Post(router, EngineJSONServer::InitializeRoute.data(),           Pistache::Rest::Routes::bind(&EngineJSONServer::initializeHandler, server));
    Pistache::Rest::Routes::Post(router, EngineJSONServer::ResetRoute.data(),                Pistache::Rest::Routes::bind(&EngineJSONServer::resetHandler, server));
    Pistache::Rest::Routes::Post(router, EngineJSONServer::ShutdownRoute.data(),             Pistache::Rest::Routes::bind(&EngineJSONServer::shutdownHandler, server));

    return router;
}

nlohmann::json EngineJSONServer::parseRequest(const Pistache::Rest::Request &req, Pistache::Http::ResponseWriter &res)
{
    json jrequest;
    try
    {
        jrequest = json::parse(req.body());
    }
    catch(std::exception &e)
    {
        auto err = NRPException::logCreate("Failed while parsing JSON object " + std::string(jrequest) + ": " + e.what());

        res.send(Pistache::Http::Code::Bad_Request);
        throw err;
    }

    return jrequest;
}

void EngineJSONServer::getDataPackDataHandler(const Pistache::Rest::Request &req, Pistache::Http::ResponseWriter res)
{
    const json jrequest(this->parseRequest(req, res));
    res.send(Pistache::Http::Code::Ok, this->getDataPackData(jrequest).dump());
}

void EngineJSONServer::setDataPackProcessorr(const Pistache::Rest::Request &req, Pistache::Http::ResponseWriter res)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    json jrequest;
    try
    {
        jrequest = json::parse(req.body());
    }
    catch(std::exception &e)
    {
        // TODO: Catch json parse error
        NRPException::logCreate(e, "Failed while parsing JSON object: " + std::string(jrequest));

        res.send(Pistache::Http::Code::Bad_Request);
        throw;
    }

    try
    {
        this->setDataPackData(jrequest);
        res.send(Pistache::Http::Code::Ok, "{}");
    }
    catch(std::exception &e)
    {
        // Send back error code if datapack could not be set
        res.send(Pistache::Http::Code::Internal_Server_Error);
        throw NRPException::logCreate(e.what());
    }
}

const std::string &EngineJSONServer::getIteratorKey(const nlohmann::json::const_iterator &jsonIterator)
{
    try
    {
        return jsonIterator.key();
    }
    catch(std::exception &e)
    {
        throw NRPException::logCreate(e, "No Key available for this JSON object");
    }
}

void EngineJSONServer::runLoopStepHandler(const Pistache::Rest::Request &req, Pistache::Http::ResponseWriter res)
{
    const json jrequest = this->parseRequest(req, res);

    SimulationTime timeStep;
    try
    {
        timeStep = SimulationTime(jrequest.at(EngineJSONConfigConst::EngineTimeStepName.data()));
    }
    catch(std::exception &e)
    {
        const auto err = NRPException::logCreate("Couldn't parse RunLoopStep JSON request " + std::string(jrequest) + ": " + e.what());

        res.send(Pistache::Http::Code::Bad_Request);
        throw err;
    }

    try
    {
        // Prevent other datapack reading/setting calls as well as loop execution
        EngineJSONServer::lock_t lock(this->_datapackLock);

        const auto retJson(nlohmann::json({{EngineJSONConfigConst::EngineTimeName.data(), (this->runLoopStep(timeStep)).count()}}));
        res.send(Pistache::Http::Code::Ok, retJson.dump());
    }
    catch(std::exception &e)
    {
        const auto err = NRPException::logCreate(std::string("Error while executing loop step: ") + e.what());

        res.send(Pistache::Http::Code::Internal_Server_Error);
        throw err;
    }
}

void EngineJSONServer::initializeHandler(const Pistache::Rest::Request &req, Pistache::Http::ResponseWriter res)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    const json jrequest = this->parseRequest(req, res);

    json jresp;
    try
    {
        // Prevent other datapack reading/setting calls as well as loop execution
        EngineJSONServer::lock_t lock(this->_datapackLock);

        // Run initialization function
        jresp = this->initialize(jrequest, lock);
    }
    catch(std::exception &e)
    {
        const auto err = NRPException::logCreate(std::string("Error while executing initialization: ") + e.what());

        res.send(Pistache::Http::Code::Internal_Server_Error);
        throw err;
    }

    // Return init response
    res.send(Pistache::Http::Code::Ok, jresp.dump());
}

void EngineJSONServer::resetHandler(const Pistache::Rest::Request &req, Pistache::Http::ResponseWriter res)
{
    const json jrequest = this->parseRequest(req, res);

    json jresp;
    try
    {
        // Prevent other datapack reading/setting calls as well as loop execution
        EngineJSONServer::lock_t lock(this->_datapackLock);

        // Run initialization function
        jresp = this->reset(lock);
    }
    catch(std::exception &e)
    {
        const auto err = NRPException::logCreate(std::string("Error while executing reset: ") + e.what());

        res.send(Pistache::Http::Code::Internal_Server_Error);
        throw err;
    }

    // Return init response
    res.send(Pistache::Http::Code::Ok, jresp.dump());
}

void EngineJSONServer::shutdownHandler(const Pistache::Rest::Request &req, Pistache::Http::ResponseWriter res)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    // Guard against the main thread shutting us down before
    // shutdown activities are finished and the response is sent back to the client

    std::lock_guard<std::mutex> shutdown_lock(this->_shutdown_mutex);

    // Tell the main thread that we ought to shut down

    this->_shutdownFlag = true;

    const json jrequest = this->parseRequest(req, res);

    json jresp;
    try
    {
        // Prevent other datapack reading/setting calls as well as loop execution
        EngineJSONServer::lock_t lock(this->_datapackLock);

        // Run shutdown function
        jresp = this->shutdown(jrequest);
    }
    catch(std::exception &e)
    {
        const auto err = NRPException::logCreate(std::string("Error while executing shutdown: ") + e.what());

        res.send(Pistache::Http::Code::Internal_Server_Error);
        throw err;
    }

    // Return shutdown response
    res.send(Pistache::Http::Code::Ok, jresp.dump());
}

Pistache::Http::Endpoint EngineJSONServer::createEndpoint(std::string *engineAddress, const std::string &engineName)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    if(engineName.empty())
    {
        NRPLogger::error("No engine name specified for server at address {}. Skipping registration", *engineAddress);

        return Pistache::Http::Endpoint(Pistache::Address(*engineAddress));
    }
    else
    {
        // Bind to a port
        const auto parsedAddr = Pistache::AddressParser(*engineAddress);
        const uint16_t basePort = static_cast<uint16_t>(std::stoi(parsedAddr.rawPort()));

        Pistache::Address engineAddr;

        for(uint16_t i = 0; i < EngineJSONConfigConst::MaxAddrBindTries; ++i)
        {
            engineAddr = Pistache::Address(parsedAddr.rawHost(), basePort+i);

            // Try to bind to given addr
            try
            {
                *engineAddress = engineAddr.host() + ":" + engineAddr.port().toString();
                return Pistache::Http::Endpoint(engineAddr);
            }
            catch(std::runtime_error &)
            {
                // Throw error only after EngineJSONConfigConst::MaxAddrBindTries attempts
                if(i >= EngineJSONConfigConst::MaxAddrBindTries-1)
                    throw;

                continue;
            }
        }
    }

    throw std::runtime_error("Could not bind EngineJSONServer to any ports");
}
