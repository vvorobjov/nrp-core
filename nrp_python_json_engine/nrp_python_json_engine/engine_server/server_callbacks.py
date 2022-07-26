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
from nrp_core.data.nrp_json import JsonDataPack
from nrp_core.engines.python_json import EngineScript
import os
import sys


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


def initialize(request_json: dict) -> dict:
    """Imports module containing the Script class, instantiates it, and runs its initialize() method"""
    global script

    # Retrieve the time units ratio used by the client and make sure that
    # the server is using correct time units.
    # Currently only nanoseconds are supported

    (num, den) = request_json["TimeRatio"]

    if num != 1 or den != 1000000000:
        raise Exception(f"NRP-Core was compiled with SimulationTime units different from nanoseconds (i.e. ratio "
                            f"{num} / {den}), but PythonJSONEngine only support nanoseconds.")

    # Load the python script module and check if the Script class inherits from EngineScript

    script_module = _import_python_script(request_json["PythonFileName"])
    if not issubclass(script_module.Script, EngineScript):
        raise Exception("Script class must inherit from EngineScript class")

    # Instantiate the Script class and run its initialize() method

    script = script_module.Script()
    script._name = request_json["EngineName"]
    script._config = request_json
    script.initialize()
    _flush_std()


def run_loop(request_json: dict) -> dict:
    """Advances the simulation time and runs the runLoop method of the Script object"""
    global script

    script._advanceTime(request_json["time_step"])
    script.runLoop(request_json["time_step"])

    return {"time": script._time_ns}


def set_datapack(request_json: dict) -> None:
    """Sets given data on requested datapacks stored in the Script object"""
    global script
    expected_keys = set(("data", "engine_name", "type"))

    if not request_json:
        return

    for datapack_name in request_json:
        datapack = request_json[datapack_name]

        # Check if any of the expected keys are not in the datapack

        if not expected_keys.issubset(datapack.keys()):
            raise Exception(f"Malformed DataPack. Expected keys: {str(expected_keys)}"
                            f"; actual keys: {str(datapack.keys())}")

        # Check if DataPack type is correct and save the data

        if datapack["type"] == JsonDataPack.getType():
            script._setDataPack(datapack_name, datapack["data"])
        else:
            raise Exception(f"Unable to set datapacks of type '{datapack['type']}'"
                            f" (DataPack named '{datapack['name']}'")


def get_datapack(request_json: dict) -> dict:
    """Returns requested datapacks stored in the Script object"""
    global script

    if not request_json:
        return {}

    return_data = {}

    for datapack_name in request_json:
        engine_name = request_json[datapack_name]["engine_name"]

        # Check if engine name in the request matches the actual name
        if engine_name != script._name:
            raise Exception(f"Requesting DataPack '{datapack_name}'"
                            f" from incorrect engine (engine name in the request '{engine_name}'"
                            f", engine that received the request '{script._name}'")

        # TODO: "type" in the request is currently empty, so we can't validate it...

        data = script._getDataPack(datapack_name)

        return_data[datapack_name] = {}
        return_data[datapack_name]["engine_name"] = engine_name
        return_data[datapack_name]["type"] = JsonDataPack.getType()
        return_data[datapack_name]["data"] = data

    return return_data


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
