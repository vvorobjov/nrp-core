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

#ifndef ${engine_name_uppercase}_GRPC_DATAPACK_CONTROLLER_SERVER_H
#define ${engine_name_uppercase}_GRPC_DATAPACK_CONTROLLER_SERVER_H

#include "nrp_general_library/engine_interfaces/datapack_controller.h"
#include "nrp_grpc_engine_protocol/grpc_server/engine_grpc.grpc.pb.h"

class ${engine_name}GrpcDataPackController
    : public DataPackController<google::protobuf::Message>
{
	public:
		${engine_name}GrpcDataPackController(const std::string & datapackName,
                                    const std::string & engineName);

		/*!
		 * \brief Processes data coming from the transceiver function
		 *
		 * \param[in] data The latest data from the transceiver function
		 */
		void handleDataPackData(const google::protobuf::Message &data) override;

		/*!
		 * \brief Returns the newest simulation data
		 *
		 * The data will be passed to the engine client through gRPC.
		 * There it will be wrapped in a datapack object and passed to the transceiver functions.
		 *
		 * \return Pointer to the latest simulation data. The returned object should NOT
		 *         be cached by the controller, its destruction will be handled by the caller.
		 *         nullptr can be returned when no new data is available.
		 */
		google::protobuf::Message * getDataPackInformation() override;

	private:

		/*!
		 * \brief Name of the datapack that is handled by this controller object
		 */
		std::string _datapackName;

		/*!
		 * \brief Name of the engine to which the controller is bound
		 */
		std::string _engineName;

		/*!
		 * \brief Cached message, can be used for storing incoming or outgoing simulation data
		 */
		EngineGrpc::TestPayload data;
};

#endif // ${engine_name_uppercase}_GRPC_DATAPACK_CONTROLLER_SERVER_H

// EOF
