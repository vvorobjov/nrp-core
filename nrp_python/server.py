from flask import Flask, request, jsonify
from importlib import import_module

app = Flask(__name__)

engine_time = 0
timestep = 200000000

import os
import sys

script = None

@app.route('/initialize', methods=["POST"])
def initialize():
    # TODO Import python engine module

    script_filename = request.json["PythonFileName"]
    script_dirname = os.path.dirname(script_filename)
    script_basename = os.path.basename(script_filename)

    # Append path of the script to sys.path. This is needed for the import to work
    sys.path.append(script_dirname)
    script_module = import_module(script_basename[:len(script_basename) - 3])

    global script
    script = script_module.Script()

    script.initialize()
    return jsonify({"InitExecStatus": True})

@app.route('/run_loop', methods=["POST"])
def run_loop():
    global engine_time
    engine_time = engine_time + timestep
    script.runLoop()
    return jsonify({"time": engine_time})

@app.route('/set_datapack', methods=["POST"])
def set_datapack():
    return jsonify({})

@app.route('/get_datapack_information', methods=["POST"])
def get_datapack():
    global script

    requested_datapacks = request.json

    if not requested_datapacks:
        return jsonify({})

    return_data = {}
    
    for datapack_name in requested_datapacks.keys():
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

    return jsonify(return_data)

@app.route('/reset', methods=["POST"])
def reset():
    return 'Hello World!'

@app.route('/shutdown', methods=["POST"])
def shutdown():
    script.shutdown()
    return {}

# EOF
