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


from time import time
from nrp_core.engines.python_grpc import GrpcEngineScript
from nrp_protobuf import dump_pb2

class Script(GrpcEngineScript):

    def __init__(self) -> None:
        super().__init__()
        self.run_loop_num_execs = 0
        self.shutdown_num_execs = 0
        self.timestep = 0

    def initialize(self):
        self._registerDataPack("test_datapack", dump_pb2.String)

    def shutdown(self):
        self.shutdown_num_execs = self.shutdown_num_execs + 1

    def runLoop(self, timestep):
        self.run_loop_num_execs = self.run_loop_num_execs + 1
        self.timestep = timestep

    def reset(self):
        pass

# EOF
