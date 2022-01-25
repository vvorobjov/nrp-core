#!/usr/bin/env python3

import flask
from flask import Flask, request, jsonify, abort
import requests
import nrp_core.engines.python_json.server_callbacks as server_callbacks
import urllib.parse
import socket
from contextlib import closing

# Make sure that we are running flask version 2.x.x

assert flask.__version__.startswith("2"), "Required flask version 2.x.x, found " + flask.__version__

app = Flask(__name__)


def is_port_in_use(port: int) -> bool:
    """Checks if given port is already in use"""
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        return s.connect_ex(('localhost', port)) == 0


def find_free_port() -> int:
    """Returns a random free port number"""
    with closing(socket.socket(socket.AF_INET, socket.SOCK_STREAM)) as s:
        # If we bind to port 0, the OS will choose an available port for us
        s.bind(('', 0))
        return s.getsockname()[1]


@app.errorhandler(500)
def all_exception_handler(error):
    """
    Custom error handler for error code 500. The default handler returns a html response,
    which doesn't look very nice when printed to terminal.
    """
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
    return jsonify(server_callbacks.get_datapack(request.json))


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

    # urlsplit() insists on absolute URLs starting with "//"

    result = urllib.parse.urlsplit('//' + args.serverurl)
    port = result.port
    hostname = result.hostname

    # Check if requested port is free

    if is_port_in_use(port):
        port = find_free_port()

    # Register in the registration server of the client

    if(args.regservurl):
        registration_data = { "engine_name": args.engine, "address": hostname + ":" + str(port) }
        response = requests.post("http://" + args.regservurl, json=registration_data).content

    # Start the server

    app.run(host=hostname, port=port)

# EOF
