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


from importlib import import_module
from types import ModuleType

import os
import sys

import engine_grpc_pb2 as python_grpc_engine_pb2
from nrp_core.engines.python_grpc import GrpcEngineScript

script = None


def _import_python_script(filename: str) -> ModuleType:
    """Imports a module with given name and returns a handle to it"""
    script_dirname, script_basename = os.path.split(filename)

    # Append path of the script to sys.path. This is needed for the import to work
    sys.path.append(script_dirname)

    return import_module(os.path.splitext(script_basename)[0])


def _flush_std():
    """Flush stdout and stderr, when they are redirected externally they need to be flushed manually"""
    sys.stdout.flush()
    sys.stderr.flush()


def get_engine_name() -> str:
    """Returns Engine name"""
    global script

    return script._getEngineName()


def initialize(request_json: dict) -> dict:
    """Imports module containing the Script class, instantiates it, and runs its initialize() method"""
    global script

    # Retrieve the time units ratio used by the client and make sure that
    # the server is using correct time units.
    # Currently only nanoseconds are supported

    (num, den) = request_json["TimeRatio"]

    if num != 1 or den != 1000000000:
        raise Exception(f"NRP-Core was compiled with SimulationTime units different from nanoseconds (i.e. ratio "
                            f"{num} / {den}), but PythonGRPCEngine only support nanoseconds.")

    # Load the python script module and check if the Script class inherits from GrpcEngineScript

    script_module = _import_python_script(request_json["PythonFileName"])
    if not issubclass(script_module.Script, GrpcEngineScript):
        raise Exception("Script class must inherit from GrpcEngineScript class")

    # Instantiate the Script class and run its initialize() method

    script = script_module.Script()
    script._name = request_json["EngineName"]
    script._config = request_json
    script.initialize()
    _flush_std()


def run_loop(time_step: int) -> int:
    """Advances the simulation time and runs the runLoop method of the Script object"""
    global script

    script._advanceTime(time_step)
    script.runLoop(time_step)

    return script._time_ns


def set_datapacks(request_grpc: python_grpc_engine_pb2.SetDataPacksRequest) \
    -> python_grpc_engine_pb2.SetDataPacksReply:
    """Sets given data on requested datapacks stored in the Script object"""
    if hasattr(request_grpc, "dataPacks"):
        for datapack in request_grpc.dataPacks:
            set_datapack(datapack)


def set_datapack(datapack: python_grpc_engine_pb2.DataPackMessage) -> None:
    """Set datapack in the Script object"""
    global script
    script._handleDataPackMsg(datapack)


def get_registered_datapack_names() -> list:
    """Returns the list of registered datapack names"""
    global script

    return script._getRegisteredDataPackNames()


def get_datapacks(request_grpc: python_grpc_engine_pb2.GetDataPacksRequest)  \
    -> python_grpc_engine_pb2.GetDataPacksReply:
    """Returns requested datapacks stored in the Script object"""
    return_data = python_grpc_engine_pb2.GetDataPacksReply()

    if hasattr(request_grpc, "datapackIds"):
        for datapack_id in request_grpc.datapackIds:
            return_data.dataPacks.append(get_datapack(datapack_id))

    return return_data


def get_datapack(datapack_id: python_grpc_engine_pb2.DataPackIdentifier) -> python_grpc_engine_pb2.DataPackMessage:
    """Returns requested datapack stored in the Script object"""
    global script
    return script._getDataPackMsg(datapack_id)


def reset(request_json: dict) -> dict:
    """Calls the reset() method of the Script object"""
    global script

    script.reset()
    _flush_std()
    script._time_ns = 0


def shutdown(request_json: dict) -> None:
    """Calls the shutdown() method of the Script object"""
    global script

    if script:
        script.shutdown()
    _flush_std()

# EOF
