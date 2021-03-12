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

@FromEngineDevice(keyword='device_engine', id=DeviceIdentifier('tf_input_engine', 'engine', 'type'))
@PreprocessedDevice(keyword='device_preprocessed', id=DeviceIdentifier('tf_input_preprocessing', 'engine', 'type'))
@TransceiverFunction("engine")
def transceiver_function(device_engine, device_preprocessed):
    test_val1 = device_engine.test_value
    test_val2 = device_preprocessed.data["test_value"]
    
    ret_dev = PythonDevice()
    ret_dev.data = {"test_value1" : str(test_val1),
                    "test_value2" : str(test_val2)}
    
    return [ret_dev]

# EOF
