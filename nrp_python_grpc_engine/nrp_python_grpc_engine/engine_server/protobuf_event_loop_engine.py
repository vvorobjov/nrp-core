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


import logging
from nrp_core.event_loop import EngineWrapper, EventLoopEngine
from google.protobuf.message import DecodeError
from engine_grpc_pb2 import DataPackMessage, DataPackIdentifier
import nrp_core.engines.python_grpc.grpc_server_callbacks as server_callbacks


class ProtobufEngineWrapper(EngineWrapper):

    def __init__(self):
        self._initialized = False
        self._dp_id = DataPackIdentifier()

    def get_engine_name(self) -> str:
        if not self._initialized:
            raise RuntimeError("Engine wrapper must be initialized first")

        return server_callbacks.get_engine_name()

    def get_registered_datapack_names(self) -> list:
        if not self._initialized:
            raise RuntimeError("Engine wrapper must be initialized first")

        return server_callbacks.get_registered_datapack_names()

    def set_datapack(self, datapack: DataPackMessage):
        if not self._initialized:
            raise RuntimeError("Engine wrapper must be initialized first")

        server_callbacks.set_datapack(datapack)

    def get_datapack(self, dp_name: str) -> DataPackMessage:
        if not self._initialized:
            raise RuntimeError("Engine wrapper must be initialized first")

        self._dp_id.dataPackName = dp_name
        return server_callbacks.get_datapack(self._dp_id)

    def initialize(self, config: dict):
        # overcome TimeRatio check which is not relevant in this application
        config["TimeRatio"] = (1, 1000000000)
        server_callbacks.initialize(config)
        self._dp_id.engineName = server_callbacks.get_engine_name()

        self._initialized = True

    def run_loop(self, timestep_ns: int):
        if not self._initialized:
            raise RuntimeError("Engine wrapper must be initialized first")

        server_callbacks.run_loop(timestep_ns)

    def shutdown(self):
        if not self._initialized:
            return

        server_callbacks.shutdown({})
        self._initialized = False


class ProtobufEventLoopEngine(EventLoopEngine):

    def __init__(self, timestep: float, timestep_thres: float, store_capacity: int,
                 do_process_last: bool,
                 engine_config: dict,
                 mqtt_config: dict,
                 engine_wrapper: ProtobufEngineWrapper):
        super().__init__(timestep, timestep_thres, store_capacity, do_process_last,
                         engine_config, mqtt_config, engine_wrapper)

    def _datapack_from_str(self, dp_name, msg_str):
        m = DataPackMessage()
        try:
            m.ParseFromString(msg_str)
        except DecodeError:
            logging.warning("Received datapack {} couldn't be unpacked".format(dp_name))
            return None

        name = m.dataPackId.dataPackName
        if name != dp_name:
            logging.warning("Received datapack {} in the wrong topic {}".format(name, dp_name))
            return None

        return m

    def _get_datapack_as_str(self, dp_name):
        m = self._engine_wrapper.get_datapack(dp_name)
        return m.SerializeToString()

