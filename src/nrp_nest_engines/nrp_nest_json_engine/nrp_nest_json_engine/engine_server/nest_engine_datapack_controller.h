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

#ifndef NEST_ENGINE_DATAPACK_CONTROLLER_H
#define NEST_ENGINE_DATAPACK_CONTROLLER_H

#include "nrp_json_engine_protocol/datapack_interfaces/json_datapack.h"
#include "nrp_json_engine_protocol/engine_server/json_datapack_controller.h"
#include "python/python_module.h"

#include <boost/python.hpp>


class NestEngineJSONDataPackController
        : public JsonDataPackController
{
    public:
        NestEngineJSONDataPackController(const DataPackIdentifier & devID, boost::python::object nodeCollection, boost::python::dict nest);

        void handleDataPackData(const nlohmann::json &data) override;
        virtual nlohmann::json * getDataPackInformation() override;

        /*!
         * \brief Set Nest properties
         * \param nest Nest instance
         * \param nodeCollection Nest GIDs of model managed by this controller
         */
        void setNestID(boost::python::dict nest, boost::python::object nodeCollection);

    protected:

        /*!
         * \brief Currently running Nest instance
         */
        boost::python::dict _nest;

        /*!
         * \brief Nest GID of model managed by this controller
         */
        boost::python::object _nodeCollection;

        /*!
         * \brief Retrieves datapack status from Nest
         * \return Returns dictionary containing datapack status
         */
        boost::python::list getStatusFromNest();

        /*!
         * \brief Retrieves datapack status from Nest
         * \param nest NEST object
         * \param nodeCollection Nodes of datapack
         * \return Returns dictionary with data
         */
        static boost::python::list getStatusFromNest(boost::python::dict &nest, const boost::python::object &nodeCollection);
};

#endif // NEST_ENGINE_DATAPACK_CONTROLLER_H
