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

#include "edlut_grpc_engine/engine_server/edlut_currents_datapack_controller.h"

EdlutCurrentsDataPackController::EdlutCurrentsDataPackController(const std::string & datapackName,
                                                         const std::string & engineName,
                                                         const std::shared_ptr<Simulation> edlutSim,
                                                         const std::shared_ptr<ArrayInputCurrentDriver> icd)
    : _datapackName(datapackName), _engineName(engineName), _edlutSimul(edlutSim), _inputCurrentDriver(icd)
{ }

void EdlutCurrentsDataPackController::handleDataPackData(const google::protobuf::Message &data)
{
    // In order to access the data from the message, you need to cast it to the proper type
    const auto &d = dynamic_cast<const EdlutData::Currents &>(data);

    for(int j=0; j<d.spikes_time_size();j++)
        this->_spikesTime.push_back(d.spikes_time(j));

    for(int j=0; j<d.neuron_indexes_size();j++)
        this->_neuronIndexes.push_back(d.neuron_indexes(j));

    for(int j=0; j<d.current_values_size();j++)
        this->_currentValues.push_back(d.current_values(j));

    this->addExternalCurrentActivity(this->_spikesTime,this->_neuronIndexes,this->_currentValues);
    this->_neuronIndexes.clear();
    this->_spikesTime.clear();
    this->_currentValues.clear();

}

google::protobuf::Message * EdlutCurrentsDataPackController::getDataPackInformation()
{
    auto payload = new EdlutData::Currents();
    this->_neuronIndexes.clear();
    this->_spikesTime.clear();
    this->_currentValues.clear();

    // Return the protobuf message
    return payload;
}

void EdlutCurrentsDataPackController::addExternalCurrentActivity(const std::vector<double> & event_time, const std::vector<long int> & neuron_index, const std::vector<float> & current_value) noexcept(false){
    try{
        //we introduce the new activity in the driver.
        if (event_time.size()>0) {
            this->_inputCurrentDriver->LoadInputs(this->_edlutSimul->GetQueue(), this->_edlutSimul->GetNetwork(),
                    event_time.size(), &event_time[0], &neuron_index[0], &current_value[0]);
        }
    }
    catch (EDLUTException Exc){
        cerr << Exc << endl;
        throw EDLUTException(TASK_EDLUT_INTERFACE, ERROR_EDLUT_INTERFACE, REPAIR_EDLUT_INTERFACE);
    }
}



// EOF
