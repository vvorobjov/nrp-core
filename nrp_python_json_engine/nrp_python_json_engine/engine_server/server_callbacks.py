from importlib import import_module
from types import ModuleType

from flask import request
from nrp_core.data.nrp_json import JsonDataPack
import os
import sys

script = None


def _import_python_script(filename: str) -> ModuleType:
    """Imports a module with given name and returns a handle to it"""
    script_dirname = os.path.dirname(filename)
    script_basename = os.path.basename(filename)

    # Append path of the script to sys.path. This is needed for the import to work
    sys.path.append(script_dirname)

    return import_module(script_basename[:len(script_basename) - 3])


def initialize(request_json: dict) -> dict:
    """Imports module containing the Script class, instantiates it, and runs its initialize() method"""
    global script

    init_exec_status = True
    init_error_message = ""

    try:
        script_module = _import_python_script(request_json["PythonFileName"])
        script = script_module.Script()
        script._name = request_json["EngineName"]
        script.initialize()
    except Exception as e:
        init_exec_status = False
        init_error_message = str(e)

    return {"InitExecStatus": init_exec_status, "Message": init_error_message}


def run_loop(request_json: dict) -> dict:
    """Advances the simulation time and runs the runLoop method of the Script object"""
    global script

    script._advanceTime(request_json["time_step"])
    script.runLoop(request_json["time_step"])

    return {"time": script._time}


def set_datapack(request_json: dict) -> None:
    """Sets given data on requested datapacks stored in the Script object"""
    global script
    expected_keys = set(("data", "engine_name", "type"))

    if not request_json:
        return

    for datapack_name in request_json.keys():

        # Check if all expected keys are in the datapack

        if expected_keys > request_json[datapack_name].keys():
            raise Exception("Malformed DataPack. Expected keys: " +
                            str(expected_keys) +
                            "; actual keys: " +
                            str(request_json[datapack_name].keys()))

        # Check if DataPack type is correct and save the data

        if(request_json[datapack_name]["type"] == JsonDataPack.getType()):
            script._setDataPack(datapack_name, request_json[datapack_name]["data"])
        else:
            raise Exception("Unable to set datapacks of type \"" +
                            request_json[datapack_name]["type"] +
                            "\" (DataPack named \"" +
                            request_json[datapack_name]["name"] + "\")")


def get_datapack(request_json: dict) -> dict:
    """Returns requested datapacks stored in the Script object"""
    global script

    if not request_json:
        return {}

    return_data = {}

    for datapack_name in request_json.keys():

        # Check if engine name in the request matches the actual name
        if(request_json[datapack_name]["engine_name"] != script._name):
            raise Exception("Requesting DataPack '" + datapack_name +
                            "' from incorrect engine (engine name in the request '" +
                            request_json[datapack_name]["engine_name"] +
                            "', engine that received the request '" + script._name + "'")

        data = script._getDataPack(datapack_name)

        return_data[datapack_name] = {}
        return_data[datapack_name]["engine_name"] = script._name
        return_data[datapack_name]["type"] = JsonDataPack.getType()

        if(data):
            return_data[datapack_name]["data"] = data
        else:
            return_data[datapack_name]["data"] = None

    return return_data


def reset(request_json: dict) -> dict:
    """Calls the reset() method of the Script object"""
    global script

    reset_exec_status = True
    reset_error_message = ""

    try:
        script.reset()
    except Exception as e:
        reset_exec_status = False
        reset_error_message = str(e)

    return {"ResetExecStatus": reset_exec_status, "Message": reset_error_message}


def shutdown(request_json: dict) -> None:
    """Calls the shutdown() method of the Script object"""
    global script

    script.shutdown()

# EOF
