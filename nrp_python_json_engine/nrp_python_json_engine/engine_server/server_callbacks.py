from importlib import import_module
from nrp_core.data.nrp_json import JsonDataPack
import os
import sys

script = None


def _import_python_script(filename):
    global script

    script_dirname = os.path.dirname(filename)
    script_basename = os.path.basename(filename)

    # Append path of the script to sys.path. This is needed for the import to work
    sys.path.append(script_dirname)
    script_module = import_module(script_basename[:len(script_basename) - 3])

    script = script_module.Script()


def initialize(request_json):
    global script

    init_exec_status = True
    init_error_message = ""

    try:
        _import_python_script(request_json["PythonFileName"])

        script.initialize()
    except Exception as e:
        init_exec_status = False
        init_error_message = str(e)

    return {"InitExecStatus": init_exec_status, "Message": init_error_message}


def run_loop(request_json):
    global script

    script._advanceTime(request_json["time_step"])
    script.runLoop(request_json["time_step"])

    return {"time": script._time}


def set_datapack(request_json):
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


def get_datapack(request_json):
    global script

    if not request_json:
        return {}

    return_data = {}

    for datapack_name in request_json.keys():
        data = script._getDataPack(datapack_name)
        if(data):
            return_data[datapack_name] = {}
            return_data[datapack_name]["engine_name"] = "python"
            return_data[datapack_name]["type"] = JsonDataPack.getType()
            return_data[datapack_name]["data"] = data
        else:
            return_data[datapack_name] = {}
            return_data[datapack_name]["engine_name"] = "python"
            return_data[datapack_name]["type"] = JsonDataPack.getType()
            return_data[datapack_name]["data"] = None

    return return_data


def reset(request_json):
    global script

    reset_exec_status = True
    reset_error_message = ""

    try:
        script.reset()
    except Exception as e:
        reset_exec_status = False
        reset_error_message = str(e)

    return {"ResetExecStatus": reset_exec_status, "Message": reset_error_message}


def shutdown(request_json):
    global script

    script.shutdown()

# EOF
