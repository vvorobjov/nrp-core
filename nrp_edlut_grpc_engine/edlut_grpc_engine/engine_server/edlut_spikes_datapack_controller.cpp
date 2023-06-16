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

#include "edlut_grpc_engine/engine_server/edlut_spikes_datapack_controller.h"

EdlutSpikesDataPackController::EdlutSpikesDataPackController(const std::string & datapackName,
                                                         const std::string & engineName,
                                                         const std::shared_ptr<Simulation> edlutSim,
                                                         const std::shared_ptr<ArrayInputSpikeDriver> isd,
                                                         const std::shared_ptr<ArrayOutputSpikeDriver> osd)
    : _datapackName(datapackName), _engineName(engineName), _edlutSimul(edlutSim), _inputSpikeDriver(isd), _outputSpikeDriver(osd)
{ }

void EdlutSpikesDataPackController::handleDataPackData(const google::protobuf::Message &data)
{
    // In order to access the data from the message, you need to cast it to the proper type
    const auto &d = dynamic_cast<const EdlutData::Spikes &>(data);

    std::stringstream m_data;
    m_data <<"Simulation time EDLUT "<< fromSimulationTime<float, std::ratio<1>>(EdlutEngine::_simulationTime) <<std::endl;
    NRPLogger::debug(m_data.str());

    for(int j=0; j<d.spikes_time_size();j++)
        this->_spikesTime.push_back(d.spikes_time(j));

    for(int j=0; j<d.neuron_indexes_size();j++)
        this->_neuronIndexes.push_back(d.neuron_indexes(j));

    this->addExternalSpikeActivity(this->_spikesTime,this->_neuronIndexes);
    this->_neuronIndexes.clear();
    this->_spikesTime.clear();

}

google::protobuf::Message * EdlutSpikesDataPackController::getDataPackInformation()
{
    this->getSpikeActivity(this->_spikesTime,this->_neuronIndexes);
    
    // Create a new protobuf message and fill it
    auto payload = new EdlutData::Spikes();

    for(auto &spike: this->_spikesTime)
        payload->add_spikes_time(spike);
    
    for(auto &index: this->_neuronIndexes)
        payload->add_neuron_indexes(index);

    this->_neuronIndexes.clear();
    this->_spikesTime.clear();

    // Return the protobuf message
    return payload;
}

void EdlutSpikesDataPackController::addExternalSpikeActivity(const std::vector<double> & event_time, const std::vector<long int> & neuron_index) noexcept(false){
    try{
        //we introduce the new activity in the driver.
        if (event_time.size()>0) {
            this->_inputSpikeDriver->LoadInputs(this->_edlutSimul->GetQueue(), this->_edlutSimul->GetNetwork(), event_time.size(), &event_time[0], &neuron_index[0]);
        }
    }
    catch (EDLUTException Exc){
        cerr << Exc << endl;
        throw EDLUTException(TASK_EDLUT_INTERFACE, ERROR_EDLUT_INTERFACE, REPAIR_EDLUT_INTERFACE);
    }
}

void EdlutSpikesDataPackController::getSpikeActivity(std::vector<double> & event_time, std::vector<long int> & neuron_index) noexcept(false){
    try{

        double * OutputSpikeTimes;
        long int * OutputSpikeCells;

        unsigned int OutputSpikes = this->_outputSpikeDriver->GetBufferedSpikes(OutputSpikeTimes,OutputSpikeCells);

        if (OutputSpikes>0) {
            event_time.resize(OutputSpikes);
            neuron_index.resize(OutputSpikes);

        double * SpTimesPtr = OutputSpikeTimes;
        long int * SpCellsPtr = OutputSpikeCells;
        std::vector<double>::iterator itTimes = event_time.begin();
        std::vector<long int>::iterator itNeurons = neuron_index.begin();

        for (unsigned int counter=0; counter<OutputSpikes; ++counter,++SpTimesPtr, ++SpCellsPtr, ++itTimes, ++itNeurons) {
            *itTimes = *SpTimesPtr;
            *itNeurons = *SpCellsPtr;
        }
    }

    return;
    }
    catch (EDLUTException Exc){
        cerr << Exc << endl;
        throw EDLUTException(TASK_EDLUT_INTERFACE, ERROR_EDLUT_INTERFACE, REPAIR_EDLUT_INTERFACE);
    }
}

// EOF
