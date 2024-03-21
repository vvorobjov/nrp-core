# NRP Core - Backend infrastructure to synchronize simulations
#
# Copyright 2020-2023 NRP Team
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

import json
import logging
from nrp_core.event_loop import EngineWrapper, EventLoopEngine
import nrp_core.engines.python_json.server_callbacks as server_callbacks


class JSONEngineWrapper(EngineWrapper):

    def __init__(self):
        self._initialized = False
        self._run_loop_req = {}

    def get_engine_name(self) -> str:
        if not self._initialized:
            raise RuntimeError("Engine wrapper must be initialized first")

        return server_callbacks.get_engine_name()

    def get_registered_datapack_names(self) -> list:
        if not self._initialized:
            raise RuntimeError("Engine wrapper must be initialized first")

        return server_callbacks.get_registered_datapack_names()

    def set_datapack(self, datapack: dict):
        if not self._initialized:
            raise RuntimeError("Engine wrapper must be initialized first")

        server_callbacks.set_datapack(datapack["name"], datapack)

    def get_datapack(self, dp_name: str) -> dict:
        if not self._initialized:
            raise RuntimeError("Engine wrapper must be initialized first")

        datapack = server_callbacks.get_datapack(dp_name)
        datapack["name"] = dp_name
        return datapack

    def initialize(self, config: dict):
        # overcome TimeRatio check which is not relevant in this application
        config["TimeRatio"] = (1, 1000000000)
        server_callbacks.initialize(config)

        self._initialized = True

    def run_loop(self, timestep_ns: int):
        if not self._initialized:
            raise RuntimeError("Engine wrapper must be initialized first")

        self._run_loop_req["time_step"] = timestep_ns
        server_callbacks.run_loop(self._run_loop_req)

    def shutdown(self):
        if not self._initialized:
            return

        server_callbacks.shutdown({})
        self._initialized = False


class JSONEventLoopEngine(EventLoopEngine):

    def __init__(self, timestep: float, timestep_thres: float, store_capacity: int,
                 do_process_last: bool,
                 engine_config: dict,
                 mqtt_config: dict,
                 engine_wrapper: JSONEngineWrapper):
        super().__init__(timestep, timestep_thres, store_capacity, do_process_last,
                         engine_config, mqtt_config, engine_wrapper)

    def _datapack_from_str(self, dp_name, msg_str):
        try:
            m = json.loads(msg_str)
        except json.decoder.JSONDecodeError as e:
            logging.warning("Received datapack {} couldn't be unpacked: {}".format(dp_name, e))
            return None

        if type(m) is not dict:
            logging.warning("Received datapack data {} is not a dictionary".format(dp_name))
            return None
        else:
            m["name"] = dp_name
            return m

    def _get_datapack_as_str(self, dp_name):
        m = self._engine_wrapper.get_datapack(dp_name)
        return json.dumps(m)

