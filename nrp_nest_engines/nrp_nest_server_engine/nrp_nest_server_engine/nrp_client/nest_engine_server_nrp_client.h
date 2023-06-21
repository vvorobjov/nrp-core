/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2023 NRP Team
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project has received funding from the European Union’s Horizon 2020
 * Framework Programme for Research and Innovation under the Specific Grant
 * Agreement No. 945539 (Human Brain Project SGA3).
 */

#ifndef NEST_ENGINE_SERVER_NRP_CLIENT_H
#define NEST_ENGINE_SERVER_NRP_CLIENT_H

#include "nrp_nest_server_engine/config/nest_server_config.h"
#include "nrp_general_library/engine_interfaces/engine_client_interface.h"
#include "nrp_general_library/plugin_system/plugin.h"

#include <future>
#include <unistd.h>

/*!
 * \brief NRP - Nest Communicator on the NRP side. Converts DataPackInterface classes from/to JSON objects
 */
class NestEngineServerNRPClient
        : public EngineClient<NestEngineServerNRPClient, NestServerConfigConst::EngineSchema>
{
        /*!
         * \brief Number of seconds to wait for Nest to exit cleanly after first SIGTERM signal. Afterwards, send a SIGKILL
         */
        static constexpr size_t _killWait = 10;

        /*!
         * \brief NestEngineServerNRPClient will look for an unbound port as default. This is the port number at which to start the search
         */
        static constexpr uint16_t PortSearchStart = 5000;

    public:
        NestEngineServerNRPClient(nlohmann::json &config, ProcessLauncherInterface::unique_ptr &&launcher);
        virtual ~NestEngineServerNRPClient() override;

        virtual void initialize() override;
        virtual void reset() override;
        virtual void shutdown() override;

        SimulationTime runLoopStepCallback(SimulationTime timeStep) override;

        virtual void sendDataPacksToEngine(const datapacks_set_t & datapacksArray) override;

        virtual const std::vector<std::string> engineProcStartParams() const override;

        using population_mapping_t = std::map<std::string, std::string>;


        virtual datapacks_vector_t getDataPacksFromEngine(const datapack_identifiers_set_t &datapackIdentifiers) override;

    private:

        /*!
         * \brief Future used during asynchronous execution of the runStep function
         */
        std::future<bool> _runStepThread;

        /*!
         * \brief Contains populations returned by server after loading the brain file
         *
         * The structure contains (population_name, [IDs]) pairs, which are returned
         * by the NEST server during initialization. The mapping may be used to access
         * populations of neurons by their name, rather than by specifying their IDs.
         *
         * The list of IDs is stored as string, formatted as JSON array.
         */
        population_mapping_t _populations;

        /*!
         * \brief NEST simulation resolution cached at engine initialization
         */
        float _simulationResolution = 0.0f;

        /*!
         * \brief Address of NEST server
         */
        std::string _serverAddress;

        bool runStepFcn(SimulationTime timestep);

        /*!
         * \brief Returns NEST server address
         *
         * \return Address of NEST server
         */
        std::string serverAddress() const;

        /*!
         * \brief Returns a JSON array of datapack IDs mapped to specified datapack name
         *
         * \param datapackName Name of the datapack
         * \return Reference to JSON array of datapack IDs, as string
         */
        const std::string & getDataPackIdList(const std::string & datapackName) const;
};

using NestEngineServerNRPClientLauncher = NestEngineServerNRPClient::EngineLauncher<NestServerConfigConst::EngineType>;


CREATE_NRP_ENGINE_LAUNCHER(NestEngineServerNRPClientLauncher);


#endif // NEST_ENGINE_SERVER_NRP_CLIENT_H
