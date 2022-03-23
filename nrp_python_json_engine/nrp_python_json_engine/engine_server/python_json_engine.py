#!/usr/bin/env python3

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


from argparse import Namespace, ArgumentParser
import flask
from flask import Flask, request, jsonify, abort
import requests
import nrp_core.engines.python_json.server_callbacks as server_callbacks
import urllib.parse
import socket
from contextlib import closing

# Disable debug printouts

import logging
log = logging.getLogger("werkzeug")
log.setLevel(logging.ERROR)

# Make sure that we are running flask version 2.x.x

assert flask.__version__.startswith("2"), "Required flask version 2.x.x, found " + flask.__version__

app = Flask(__name__)


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


@app.route('/set_datapacks', methods=["POST"])
def set_datapack():
    return jsonify(server_callbacks.set_datapack(request.json))


@app.route('/get_datapacks', methods=["POST"])
def get_datapack():
    return jsonify(server_callbacks.get_datapack(request.json))


@app.route('/reset', methods=["POST"])
def reset():
    return jsonify(server_callbacks.reset(request.json))


@app.route('/shutdown', methods=["POST"])
def shutdown():
    return jsonify(server_callbacks.shutdown(request.json))


def parse_arguments() -> Namespace:
    parser = ArgumentParser()
    parser.add_argument('--engine',     type=str, required=True)
    parser.add_argument('--serverurl',  type=str, required=True)
    parser.add_argument('--regservurl', type=str, required=True)
    return parser.parse_args()


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


def extract_hostname_port_from_url(url: str) -> tuple:
    # urlsplit() insists on absolute URLs starting with "//"

    result = urllib.parse.urlsplit('//' + url)
    port = result.port
    hostname = result.hostname

    # Check if the requested port is free. If it's not, then choose another one randomly

    if is_port_in_use(port):
        port = find_free_port()

    return hostname, port


def register_in_regserver(regserver_url: str, engine_name: str, hostname: str, port: int) -> None:
    """Registers in the registration server of the client"""

    registration_data = { "engine_name": engine_name, "address": hostname + ":" + str(port) }
    response = requests.post("http://" + regserver_url, json=registration_data).content


if __name__ == '__main__':
    args = parse_arguments()
    hostname, port = extract_hostname_port_from_url(args.serverurl)
    register_in_regserver(args.regservurl, args.engine, hostname, port)

    app.run(host=hostname, port=port)

# EOF
