# NRP Core - Backend infrastructure to synchronize simulations
#
# Copyright 2020-2021 NRP Team
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# This project has received funding from the European Union’s Horizon 2020
# Framework Programme for Research and Innovation under the Specific Grant
# Agreement No. 945539 (Human Brain Project SGA3).

from nrp_core import *
from nrp_core.data.nrp_json import *

@EngineDataPack(keyword='datapack_engine', id=DataPackIdentifier('tf_input_engine', 'engine'))
@PreprocessedDataPack(keyword='datapack_preprocessed', id=DataPackIdentifier('tf_input_preprocessing', 'engine'))
@TransceiverFunction("engine")
def transceiver_function(datapack_engine, datapack_preprocessed):
    test_val1 = datapack_engine.data["testValue"]
    test_val2 = datapack_preprocessed.data["test_value"]
    
    ret_dev = JsonDataPack('return_datapack', 'engine')
    ret_dev.data["test_value1"] = str(test_val1)
    ret_dev.data["test_value2"] = test_val2
    
    return [ret_dev]

# EOF
