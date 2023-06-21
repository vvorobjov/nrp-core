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

#ifndef ${engine_name_uppercase}_GRPC_SERVER_H
#define ${engine_name_uppercase}_GRPC_SERVER_H

#include "nrp_grpc_engine_protocol/engine_server/engine_proto_wrapper.h"
#include "nrp_general_library/utils/python_interpreter_state.h"
#include "nrp_protobuf/enginetest.pb.h"

class ${engine_name}Engine
    : public EngineProtoWrapper
{
    public:
        ${engine_name}Engine(const std::string &engineName,
                                 const std::string &protobufPluginsPath,
                                 const nlohmann::json &protobufPlugins);
        ~${engine_name}Engine() = default;

        /*!
         * \brief Indicates if the simulation was initialized and is running
         */
        virtual bool initRunFlag() const override { return this->_initRunFlag; };

        /*!
         * \brief Indicates if shutdown was requested by the client
         */
        virtual bool shutdownFlag() const override { return this->_shutdownFlag; };

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
        void initialize(const nlohmann::json &data) override;

        /*!
         * \brief Shutdowns the engine
         *
         * The function will be called at the end of the simulation.
         * It should clean up all resources of the server and call API
         * functions of the simulator responsible for terminating the simulation.
         *
         * \param[in] data Additional arguments passed from the client
         */
        void shutdown() override;

        /*!
         * \brief Resets the engine
         *
         * The function should perform all actions needed to reset the simulation
         * to its initial state.
         */
        void reset() override;

    private:

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
};

#endif // ${engine_name_uppercase}_JSON_SERVER_H

// EOF
