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

#ifndef EDLUT_CURRENTS_DATAPACK_CONTROLLER_SERVER_H
#define EDLUT_CURRENTS_DATAPACK_CONTROLLER_SERVER_H

#include "nrp_general_library/engine_interfaces/datapack_controller.h"
#include "nrp_protobuf/engine_grpc.grpc.pb.h"
#include "nrp_protobuf/edlutdata.pb.h"
#include "nrp_protobuf/proto_python_bindings/proto_field_ops.h"
#include "nrp_protobuf/proto_ops/proto_ops_manager.h"
#include "nrp_general_library/utils/time_utils.h"
#include "edlut_grpc_engine/engine_server/edlut_grpc_server.h"
#include "communication/ArrayInputCurrentDriver.h"

class EdlutCurrentsDataPackController
    : public DataPackController<google::protobuf::Message>
{
    public:
        EdlutCurrentsDataPackController(const std::string & datapackName,
                                    const std::string & engineName,
                                    const std::shared_ptr<Simulation> edlutSim,
                                    const std::shared_ptr<ArrayInputCurrentDriver> icd);

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

        /*!
         * \brief Introduces current data to the simulation network
         *
         * The data will be passed to the network through the input driver.
         *
         */
        void addExternalCurrentActivity(const std::vector<double> & event_time, const std::vector<long int> & neuron_index, const std::vector<float> & current_value) noexcept(false);

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
         * \brief Pointer to the Edlut Simulation Object
         */
        std::shared_ptr<Simulation> _edlutSimul;

        /*!
         * Input current driver
         */
        std::shared_ptr<ArrayInputCurrentDriver> _inputCurrentDriver;

        /*!
         * \brief Buffer vector used by both input and output drivers to temporarily store spikes firing times
         */
        vector<double> _spikesTime;

        /*!
         * \brief Buffer vector used by both input and output drivers to temporarily store network neuron indexes
         */
        vector<long int> _neuronIndexes;

        /*!
         * \brief Buffer vector used by input driver to temporarily store network currents values
         */
        vector<float> _currentValues;

};

#endif // EDLUT_CURRENTS_DATAPACK_CONTROLLER_SERVER_H

// EOF
