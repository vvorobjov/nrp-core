#!/usr/bin/env python3

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
#
# This file is a modified version of another one originally authored by:
# Bhaskara Marthi with license BSD.
# The original file can be found at: https://github.com/bhaskara/boost_python_ros

import sys
import code_generation as cg


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: {0} PACKAGE MESSAGE".format(sys.argv[0]))
        sys.exit()
    print(cg.generate_file(sys.argv[1], sys.argv[2]))
    
