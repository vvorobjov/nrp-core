/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2021 NRP Team
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

#ifndef NEST_KERNEL_DATAPACK_CONTROLLER_H
#define NEST_KERNEL_DATAPACK_CONTROLLER_H

#include <boost/python.hpp>

#include "nrp_json_engine_protocol/engine_server/json_datapack_controller.h"


class NestKernelDataPackController
	: public JsonDataPackController
{
	public:
		NestKernelDataPackController(const DataPackIdentifier & devID, boost::python::dict nest);

		void handleDataPackData(const nlohmann::json &data) override;
		virtual nlohmann::json * getDataPackInformation() override;

	private:

		/*!
		 * \brief Currently running Nest instance
		 */
		boost::python::dict _nest;
};

#endif // NEST_KERNEL_DATAPACK_CONTROLLER_H

// EOF
