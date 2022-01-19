from flask import Flask, request, jsonify, abort
import requests
import nrp_core.engines.python_json.server_callbacks as server_callbacks
import urllib.parse

app = Flask(__name__)


@app.errorhandler(500)
def all_exception_handler(error):
    return jsonify(str(error)), 500


@app.route('/initialize', methods=["POST"])
def initialize():
    return jsonify(server_callbacks.initialize(request.json))


@app.route('/run_loop', methods=["POST"])
def run_loop():
    return jsonify(server_callbacks.run_loop(request.json))


@app.route('/set_datapack', methods=["POST"])
def set_datapack():
    return jsonify(server_callbacks.set_datapack(request.json))


@app.route('/get_datapack_information', methods=["POST"])
def get_datapack():
    try:
        response = server_callbacks.get_datapack(request.json)
    except Exception as e:
        abort(500, str(e))
    return jsonify(response)


@app.route('/reset', methods=["POST"])
def reset():
    return jsonify(server_callbacks.reset(request.json))


@app.route('/shutdown', methods=["POST"])
def shutdown():
    return jsonify(server_callbacks.shutdown(request.json))


if __name__ == '__main__':
    from argparse import ArgumentParser
    parser = ArgumentParser()
    parser.add_argument('--engine', type=str, default="")
    parser.add_argument('--serverurl', type=str, default="")
    parser.add_argument('--regservurl', type=str, default="")
    args = parser.parse_args()

    # Register in the registration server of the client

    if(args.regservurl):
        registration_data = { "engine_name": args.engine, "address": args.serverurl }
        response = requests.post("http://" + args.regservurl, json=registration_data).content

    # urlsplit() insists on absolute URLs starting with "//"

    result = urllib.parse.urlsplit('//' + args.serverurl)

    app.run(host=result.hostname, port=result.port)

# EOF
