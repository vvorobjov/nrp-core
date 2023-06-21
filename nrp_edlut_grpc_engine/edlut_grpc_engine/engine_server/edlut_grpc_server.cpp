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

#include "edlut_grpc_engine/engine_server/edlut_grpc_server.h"
#include "edlut_grpc_engine/engine_server/edlut_spikes_datapack_controller.h"
#include "edlut_grpc_engine/engine_server/edlut_currents_datapack_controller.h"
#include <sstream>


SimulationTime EdlutEngine::_simulationTime = SimulationTime::zero();

EdlutEngine::EdlutEngine(const std::string &engineName,
                                                   const std::string &protobufPluginsPath,
                                                   const nlohmann::json &protobufPlugins)
    : EngineProtoWrapper(engineName, protobufPluginsPath, protobufPlugins),
    _engineName(engineName)
{
    this->_inputSpikeDriver = std::make_shared<ArrayInputSpikeDriver>();
    this->_outputSpikeDriver = std::make_shared<ArrayOutputSpikeDriver>();
    this->_inputCurrentDriver = std::make_shared<ArrayInputCurrentDriver>();
}

SimulationTime EdlutEngine::runLoopStep(SimulationTime timeStep)
{

    NRP_LOGGER_TRACE("EdlutEngine::runLoopStep called");

    this->_simulationTime += timeStep;

    /* Run edlut simulator */
    this->_edlutSimul->RunSimulationSlot(fromSimulationTime<float, std::ratio<1>>(EdlutEngine::_simulationTime)+this->_sensorialDelay);

    return this->_simulationTime;
}

void EdlutEngine::initialize(const nlohmann::json &data)
{
    NRPLogger::info("Initializing EDLUT gRPC Engine");
    this->_engineConfig = data;
    NRPLogger::debug(this->_engineConfig.dump(4));

    initEdlutEngine(data);
}

void EdlutEngine::initEdlutEngine(const nlohmann::json &data)
{
    // Create Edlut sim objects
    try{
        auto networkFile = data.at("NetworkFile").get<std::string>();
        auto weightFile = data.at("WeightsFile").get<std::string>();
        auto simTimestep = data.at("EngineTimestep").get<double>();
        auto numThreads = data.at("NumThreads").get<int>();
        this->_sensorialDelay = data.at("SensorialDelay").get<float>();
        auto saveWeightsPeriod = data.at("SaveWeightsPeriod").get<float>();

        this->_edlutSimul = std::make_shared<Simulation> (networkFile.c_str(), weightFile.c_str(), std::numeric_limits<double>::max(), simTimestep, numThreads);

        // Initialize simulation
        this->_edlutSimul->AddInputSpikeDriver(this->_inputSpikeDriver.get());
        this->_edlutSimul->AddOutputSpikeDriver(this->_outputSpikeDriver.get());
        this->_edlutSimul->AddInputCurrentDriver(this->_inputCurrentDriver.get());

        std::string weight_file = "output_weight.dat";
        this->_edlutSimul->AddOutputWeightDriver(new FileOutputWeightDriver(weight_file.c_str()));
        if(saveWeightsPeriod>0.0){
            this->_edlutSimul->SetSaveStep(saveWeightsPeriod);
            this->_edlutSimul->GetQueue()->InsertEventWithSynchronization(new SaveWeightsEvent(this->_edlutSimul->GetSaveStep(), this->_edlutSimul.get()));
        }

        this->_edlutSimul->InitSimulation();
    }
    catch (EDLUTException Exc){
        throw NRPException::logCreate("Failed to initialize \""+this->_engineName+"\" Engine: " + Exc.GetErrorMsg());
    }

    // Register datapack
    this->registerDataPack("spikes_datapack", new EdlutSpikesDataPackController("spikes_datapack", this->_engineName, this->_edlutSimul, this->_inputSpikeDriver, this->_outputSpikeDriver));
    this->registerDataPack("currents_datapack", new EdlutCurrentsDataPackController("currents_datapack", this->_engineName, this->_edlutSimul, this->_inputCurrentDriver));
    NRPLogger::info("DataPacks \"spikes_datapack\" and \"currents_datapack\" were registered");

    EdlutEngine::_simulationTime = SimulationTime::zero();
    this->_initRunFlag = true;
}

void EdlutEngine::shutdown()
{
    NRPLogger::info("Shutting down EDLUT simulation");
    this->_edlutSimul->SaveWeights();
    this->_shutdownFlag = true;
}

void EdlutEngine::reset()
{
    NRPLogger::info("Resetting EDLUT simulation");

    this->clearRegisteredDataPacks();
    this->initEdlutEngine(this->_engineConfig);
}


// EOF
