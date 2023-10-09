# NRP Core - Backend infrastructure to synchronize simulations
#
# Copyright 2020-2023 NRP Team
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

@EngineDataPack(keyword='datapack1', id=DataPackIdentifier('pf_input1', 'engine'))
@EngineDataPack(keyword='datapack2', id=DataPackIdentifier('pf_input2', 'engine'))
@EngineDataPack(keyword='datapack3', id=DataPackIdentifier('pf_input3', 'engine'))
@PreprocessingFunction("engine")
def transceiver_function(datapack1, datapack2, datapack3):
    ret_dev1 = JsonDataPack("tf_input_preprocessing", "engine")
    ret_dev1.data["test_value1"] = str(datapack1.data["testValue"])
    ret_dev1.data["test_value2"] = str(datapack2.data["testValue"])
    ret_dev1.data["test_value3"] = str(datapack3.data["testValue"])
    
    return [ret_dev1]

# EOF
