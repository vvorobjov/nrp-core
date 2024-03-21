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

#include "nrp_communication_controller.h"

#include "nrp_general_library/utils/nrp_exceptions.h"

#include <nlohmann/json.hpp>

using namespace nlohmann;

std::unique_ptr<NRPJSONCommunicationController> NRPJSONCommunicationController::_instance = nullptr;

NRPJSONCommunicationController::~NRPJSONCommunicationController()
{
    this->_stepController = nullptr;
}

NRPJSONCommunicationController &NRPJSONCommunicationController::getInstance()
{
    if(NRPJSONCommunicationController::_instance)
        return *(NRPJSONCommunicationController::_instance.get());
    else
        throw NRPException::logCreate("Attempting to access NRPJSONCommunicationController singleton, but it has to be"
                                      " instantiated first");
}

NRPJSONCommunicationController &NRPJSONCommunicationController::resetInstance(const std::string &serverURL, const std::string &engineName, const std::string &registrationURL)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    NRPJSONCommunicationController::_instance.reset(new NRPJSONCommunicationController(serverURL, engineName, registrationURL));
    return NRPJSONCommunicationController::getInstance();

}

void NRPJSONCommunicationController::registerStepController(GazeboStepController *stepController)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    EngineJSONServer::lock_t lock(this->_datapackLock);
    this->_stepController = stepController;
}

SimulationTime NRPJSONCommunicationController::runLoopStep(SimulationTime timeStep)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    if(this->_stepController == nullptr)
        throw NRPException::logCreate("Tried to run loop while the controller has not yet been initialized");

    try
    {
        // Execute loop step (Note: The _datapackLock mutex has already been set by EngineJSONServer::runLoopStepHandler, so no calls to reading/writing from/to datapacks is possible at this moment)
        return this->_stepController->runLoopStep(timeStep);
    }
    catch(std::exception &e)
    {
        throw NRPException::logCreate(e, "Error during Gazebo stepping");
    }
}

json NRPJSONCommunicationController::initialize(const json &data, EngineJSONServer::lock_t &lock)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    double waitTime = data.at("WorldLoadTime");
    if(waitTime <= 0)
        waitTime = std::numeric_limits<double>::max();

    // Allow datapacks to register
    lock.unlock();

    // Wait until world plugin loads
    while(this->_stepController == nullptr)
    {
        // Wait for 100ms before retrying
        waitTime -= 0.1;
        usleep(100*1000);

        if(waitTime <= 0)
        {
            lock.lock();
            return nlohmann::json({false});
        }
    }

    // Spawn additional models
    if(data.contains("GazeboSDFModels")) {
        for (const auto &model: data.at("GazeboSDFModels")) {
            // Parse pose
            std::istringstream poseStr(model.at("InitPose").get<std::string>());
            std::vector<std::string> poseArgs(std::istream_iterator<std::string>{poseStr},
                                              std::istream_iterator<std::string>());
            if(poseArgs.size() != 6)
                throw NRPException::logCreate("Error while parsing SDF model" + model.at("Name").get<std::string>()
                                              + ". Pose array must contain 6 elements, contains " + std::to_string(poseArgs.size()));

            // Parse args
            std::vector<std::string> args = {"gz", "model", "--spawn-file=" + model.at("File").get<std::string>(),
                                             "--model-name=" + model.at("Name").get<std::string>(), "-x", poseArgs[0],
                                             "-y", poseArgs[1], "-z", poseArgs[2], "-R", poseArgs[3], "-P", poseArgs[4],
                                             "-Y", poseArgs[5]};

            std::string argsStr;
            for (auto& a : args)
                argsStr += a + " ";

            // Exec cmd
            NRPLogger::info("Spawning model \"" + model.at("Name").get<std::string>() + "\" with command: " + argsStr);
            this->_stepController->addRequiredModel(model.at("Name").get<std::string>());
            auto status = system(argsStr.c_str());
            if(!WIFEXITED(status) || WEXITSTATUS(status) != EXIT_SUCCESS)
                throw NRPException::logCreate("Spawning model \"" + model.at("Name").get<std::string>() + "\" failed");

        }
    }

    // Forces plugins to load
    this->_stepController->finishWorldLoading(waitTime);

    lock.lock();

    return nlohmann::json({true});
}

json NRPJSONCommunicationController::reset(EngineJSONServer::lock_t &lock)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    try{
        // Is it enough to reset just the world?
        this->_stepController->resetWorld();
        for (size_t i = 0; i < this->_sensorPlugins.size(); i++){
            this->_sensorPlugins[i]->Reset();
        }
        for (size_t i = 0; i < this->_modelPlugins.size(); i++){
            this->_modelPlugins[i]->Reset();
        }
    }
    catch(const std::exception &e)
    {
        NRPLogger::error("NRPJSONCommunicationController::reset: failed to resetWorld()");

        return nlohmann::json({false});
    }
    
    return nlohmann::json({true});
}

json NRPJSONCommunicationController::shutdown(const json&)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    return nlohmann::json();
}


NRPJSONCommunicationController::NRPJSONCommunicationController(const std::string &serverURL, const std::string &engineName, const std::string &registrationURL)
    : EngineJSONServer(serverURL, engineName, registrationURL)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
}
