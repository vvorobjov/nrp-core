from importlib import import_module
import os
import sys

script = None

engine_time = 0
timestep = 200000000

def initialize(request_json):

    init_exec_status = True
    init_error_message = ""

    try:
        script_filename = request_json["PythonFileName"]
        script_dirname = os.path.dirname(script_filename)
        script_basename = os.path.basename(script_filename)

        # Append path of the script to sys.path. This is needed for the import to work
        sys.path.append(script_dirname)
        script_module = import_module(script_basename[:len(script_basename) - 3])

        global script
        script = script_module.Script()

        script.initialize()
    except Exception as e:
        init_exec_status = False
        init_error_message = str(e)

    return {"InitExecStatus": init_exec_status, "Message": init_error_message}


def run_loop(request_json):
    global engine_time
    global script

    engine_time = engine_time + timestep
    script.runLoop()
    return {"time": engine_time}


def set_datapack(request_json):
    global script

    if not request_json:
        return {}

    return_dict = {}
    for datapack_name in request_json.keys():
        data = request_json[datapack_name]["data"]
        script._setDataPack(datapack_name, data)
        # TODO Is this needed? Do we have to return anything?
        return_dict[datapack_name] = None

    return return_dict


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
            return_data[datapack_name]["type"] = ""
            return_data[datapack_name]["data"] = data
        else:
            return_data[datapack_name] = {}
            return_data[datapack_name]["engine_name"] = "python"
            return_data[datapack_name]["type"] = ""
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

    init_exec_status = True
    init_error_message = ""

    try:
        script.shutdown()
    except Exception as e:
        init_exec_status = False
        init_error_message = str(e)

    return {"ShutdownExecStatus": init_exec_status, "Message": init_error_message}

# EOF
