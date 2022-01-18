from flask import Flask, request, jsonify
import requests
import nrp_core.engines.python_json.server_callbacks as server_callbacks

app = Flask(__name__)

# Register in the registration server of the client

registration_data = { "engine_name": "python", "address": "localhost:9002" }
response = requests.post('http://localhost:9001', json=registration_data).content


@app.route('/initialize', methods=["POST"])
def initialize():
    return jsonify(server_callbacks.initialize(request.json))


@app.route('/run_loop', methods=["POST"])
def run_loop():
    return jsonify(server_callbacks.run_loop(request.json))


@app.route('/set_datapack', methods=["POST"])
def set_datapack():
    return jsonify(server_callbacks.run_loop(request.json))


@app.route('/get_datapack_information', methods=["POST"])
def get_datapack():
    return jsonify(server_callbacks.get_datapack(request.json))


@app.route('/reset', methods=["POST"])
def reset():
    return jsonify(server_callbacks.reset(request.json))


@app.route('/shutdown', methods=["POST"])
def shutdown():
    return jsonify(server_callbacks.shutdown(request.json))

# EOF
