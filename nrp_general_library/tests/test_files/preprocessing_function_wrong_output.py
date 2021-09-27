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
from NRPGeneralPythonTestModule import TestInputDataPack, TestOutputDataPack

@FromEngineDataPack(keyword='datapack', id=DataPackIdentifier('pf_input', 'engine', 'type'))
@PreprocessingFunction("engine")
def transceiver_function(datapack):
    test_val = datapack.test_value
    
    ret_dev = PythonDataPack("tf_input_preprocessing", "another_engine")
    ret_dev.data = {"test_value" : str(test_val + 1)}
    
    return [ret_dev]

# EOF
