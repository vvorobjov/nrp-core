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

from NRPPythonModule import *
from NRPGeneralPythonTestModule import TestInputDevice, TestOutputDevice

@FromEngineDevice(keyword='device1', id=DeviceIdentifier('pf_input1', 'engine', 'type'))
@FromEngineDevice(keyword='device2', id=DeviceIdentifier('pf_input2', 'engine', 'type'))
@FromEngineDevice(keyword='device3', id=DeviceIdentifier('pf_input3', 'engine', 'type'))
@PreprocessingFunction("engine")
def transceiver_function(device1, device2, device3):
    ret_dev1 = PythonDevice()
    ret_dev2 = TestInputDevice()
    ret_dev1.data = {"test_value1" : str(device1.test_value),
                     "test_value2" : str(device2.test_value),
                     "test_value3" : str(device3.test_value)}
    
    return [ret_dev1, ret_dev2]

# EOF
