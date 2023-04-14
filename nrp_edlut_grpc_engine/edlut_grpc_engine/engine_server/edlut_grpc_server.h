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

#ifndef EDLUT_GRPC_SERVER_H
#define EDLUT_GRPC_SERVER_H

#include "nrp_grpc_engine_protocol/engine_server/engine_grpc_server.h"
#include "nrp_general_library/utils/python_interpreter_state.h"
#include "nrp_protobuf/enginetest.pb.h"
#include "simulation/Simulation.h"
#include "communication/ArrayInputSpikeDriver.h"
#include "communication/ArrayOutputSpikeDriver.h"

class EdlutGrpcServer
    : public EngineGrpcServer
{
    public:
        EdlutGrpcServer(const std::string &serverAddress, const std::string &engineName,
                                 const std::string &protobufPluginsPath,
                                 const nlohmann::json &protobufPlugins);
        ~EdlutGrpcServer() = default;

        /*!
         * \brief Indicates if the simulation was initialized and is running
         */
        bool initRunFlag() const { return this->_initRunFlag; };

        /*!
         * \brief Indicates if shutdown was requested by the client
         */
        bool shutdownFlag() const { return this->_shutdownFlag; };

        /*!
         * \brief Runs a single step of the simulation
         *
         * The function will be called every simulation loop, after getDataPacksFromEngine and
         * before sendDataPacksToEngine. It should advance the simulation by the requested time step,
         *
         * \param[in] timeStep Requested time step, by which the simulation should advance
         *
         * \return Simulation time after the loop step
         */
        SimulationTime runLoopStep(SimulationTime timeStep) override;

        /*!
         * \brief Initializes the engine
         *
         * The function will be called at the start of the simulation.
         * It should initialize the server and call API functions
         * of the simulator responsible for simulation initialization.
         *
         * \param[in] data Engine configuration data in form of JSON object
         */
        void initialize(const nlohmann::json &data, EngineGrpcServer::lock_t &datapackLock) override;

        /*!
         * \brief Shutdowns the engine
         *
         * The function will be called at the end of the simulation.
         * It should clean up all resources of the server and call API
         * functions of the simulator responsible for terminating the simulation.
         *
         * \param[in] data Additional arguments passed from the client
         */
        void shutdown(const nlohmann::json &data) override;

        /*!
         * \brief Resets the engine
         *
         * The function should perform all actions needed to reset the simulation
         * to its initial state.
         */
        void reset() override;

        /*!
         * \brief Engine simulation time
         */
        static SimulationTime _simulationTime;


    private:

        /*!
         * \brief Helper function used by Initializes and Reset to initialize the engine
         */
        void initEdlutEngine(const nlohmann::json &data, EngineGrpcServer::lock_t &datapackLock);

        /*!
         * \brief Indicates if the simulation was initialized and is running
         */
        bool _initRunFlag = false;

        /*!
         * \brief Indicates if shutdown was requested by the client
         */
        bool _shutdownFlag = false;

        /*!
         * \brief Name of the engine
         */
        std::string _engineName;

        /*!
         * \brief The list of the names of the associated DataPacks
         */
        std::vector< std::string > _dataPacksNames;

        /*!
         * \brief Pointer to EDLUT simulation object
         */
        std::shared_ptr<Simulation> _edlutSimul;

        /*!
         * \brief Input spike driver to EDLUT
         */
        std::shared_ptr<ArrayInputSpikeDriver> _inputSpikeDriver;

        /*!
         * \brief Output spike driver from EDLUT
         */
        std::shared_ptr<ArrayOutputSpikeDriver> _outputSpikeDriver;

        /*!
         * \brief Parameters backup needed to reset EDLUT simulation as json object
         */
        nlohmann::json _engineConfig;




};

#endif // EDLUT_JSON_SERVER_H

// EOF