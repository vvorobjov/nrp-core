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

#ifndef NEST_JSON_SERVER_H
#define NEST_JSON_SERVER_H

#include "nrp_json_engine_protocol/engine_server/engine_json_server.h"
#include "nrp_general_library/utils/python_interpreter_state.h"
#include "nrp_nest_json_engine/engine_server/nest_engine_datapack_controller.h"

#include "nrp_nest_json_engine/python/create_datapack_class.h"


#include <boost/python.hpp>

class NestJSONServer
        : public EngineJSONServer
{
    public:
        NestJSONServer(const std::string &serverAddress, const std::string &engineName, const std::string &registrationAddress, boost::python::dict globals);
        virtual ~NestJSONServer() override;

        /*!
         * \brief Has the initialization been executed?
         * \return Returns true once the initialize function has been run once
         */
        bool initRunFlag() const;

        virtual SimulationTime runLoopStep(SimulationTime timeStep) override;
        virtual nlohmann::json initialize(const nlohmann::json &data, EngineJSONServer::lock_t &datapackLock) override;
        virtual nlohmann::json reset(EngineJSONServer::lock_t &datapackLock) override;
        virtual nlohmann::json shutdown(const nlohmann::json &data) override;

    private:
        /*!
         * \brief Init Flag. Set to true once the server has executed the initialize function
         */
        bool _initRunFlag = false;

        /*!
         * \brief NEST Preparation Flag. Set to true once nest.Prepare() was run and back to false after nest.Cleanup() was run
         */
        bool _nestPreparedFlag = false;

        /*!
         * \brief Global Python variables
         */
        boost::python::dict _pyGlobals;

        /*!
         * \brief Nest object
         */
        boost::python::dict _pyNest;

        /*!
         * \brief NRP Nest object
         */
        boost::python::dict _pyNRPNest;

        /*!
         * \brief NEST DataPack Mapping (from DataPack Name to NEST NodeCollection)
         */
        boost::python::dict _devMap;

        /*!
         * \brief List of datapack ptrs. Used to manage controller deletion
         */
        std::list<std::shared_ptr<DataPackController<nlohmann::json>>> _datapackControllerPtrs;

        /*!
         *  \brief GIL Lock state
         */
        PyGILState_STATE _pyGILState;

        /*!
         * \brief Converts seconds to milliseconds
         * \param sec Seconds
         * \return Returns milliseconds
         */
        static constexpr float convertSecToMill(const float sec);

        /*!
         * \brief Converts milliseconds to seconds
         * \param milsec Milliseconds
         * \return Returns seconds
         */
        static constexpr float convertMillToSec(const float millsec);

        /*!
         * \brief Creates an error message to be returned to the main NRP process
         * \param errMsg Error text
         * \return Returns a JSON object containing the error text as well as a failure flag
         */
        static nlohmann::json formatInitErrorMessage(const std::string &errMsg);

        nlohmann::json getDataPackData(const nlohmann::json &reqData) override;

        void setDataPackData(const nlohmann::json &reqData) override;

        nlohmann::json _initData;
};

#endif // NEST_JSON_SERVER_H
