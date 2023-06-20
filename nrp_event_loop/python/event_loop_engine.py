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

from typing import Type
import signal
from argparse import ArgumentParser
from abc import ABC, abstractmethod
import logging
import json
import paho.mqtt.client as mqtt
from nrp_core.event_loop import EventLoopInterface


class EngineWrapper(ABC):

    @abstractmethod
    def get_engine_name(self):
        pass

    @abstractmethod
    def get_registered_datapack_names(self):
        pass

    @abstractmethod
    def set_datapack(self, datapack):
        pass

    @abstractmethod
    def get_datapack(self, dp_name: str):
        pass

    @abstractmethod
    def initialize(self, config: dict):
        pass

    @abstractmethod
    def run_loop(self, timestep_ns: int):
        pass

    @abstractmethod
    def shutdown(self):
        pass


class EventLoopEngine(EventLoopInterface):

    def __init__(self, timestep: float, timestep_thres: float, store_capacity: int,
                 do_process_last: bool,
                 engine_config: dict,
                 mqtt_config: dict,
                 engine_wrapper: EngineWrapper):
        super().__init__(timestep, timestep_thres)

        self._store_capacity = store_capacity
        self._do_process_last = do_process_last
        self._mqtt_config = mqtt_config
        self._engine_config = engine_config
        self._engine_wrapper = engine_wrapper

        self._timestep_ns = int(self._timestep * 1e9)

        self._dp_names = None
        self._dp_store = {}

        self._client = None
        # for testing only
        self._allowNoMqtt = False

    def __del__(self):
        self.shutdown()

    def _initialize_cb(self):
        self._engine_wrapper.initialize(self._engine_config)

        self._dp_names = self._engine_wrapper.get_registered_datapack_names()
        for name in self._dp_names:
            self._dp_store[name] = []

        def on_connect(client, userdata, flags, rc):
            if rc == 0:
                logging.info("Connected to mqtt server")
            else:
                logging.warning("Failed to connect to mqtt server with result code {}".format(rc))
                return

            for name in self._dp_names:
                logging.info("Subscribing to topic: {}".format(self._dp_topic_set(name)))
                client.subscribe(self._dp_topic_set(name), 1)

        def on_message(client, userdata, msg):
            t = msg.topic
            self._topic_callback(t[t.rfind('/')+1:], msg.payload)

        self._client = mqtt.Client(self._mqtt_config["ClientName"] if "ClientName" in self._mqtt_config
                                   else self._engine_wrapper.get_engine_name(), True)

        self._client.on_connect = on_connect
        self._client.on_message = on_message

        try:
            self._client.connect(self._mqtt_config["MQTTBroker"] if "MQTTBroker" in self._mqtt_config
                             else "localhost")
        except ConnectionRefusedError as e:
            if not self._allowNoMqtt:
                raise RuntimeError("EventLoopEngine failed to connect to MQTT. Ensure that the MQTT broker is running "
                                   "and check the engine configuration.")
        finally:
            self._client.loop()

    def _run_loop_cb(self):
        if self._client.is_connected():
            self._client.loop()

        for dp_name in self._dp_store:
            if self._do_process_last and len(self._dp_store[dp_name]):
                self._engine_wrapper.set_datapack(self._dp_store[dp_name][-1])
            else:
                for dp in self._dp_store[dp_name]:
                    self._engine_wrapper.set_datapack(dp)
            self._dp_store[dp_name].clear()

        self._engine_wrapper.run_loop(self._timestep_ns)

        if self._client.is_connected():
            for name in self._dp_names:
                self._client.publish(self._dp_topic_get(name), self._get_datapack_as_str(name), 1)

    def _shutdown_cb(self):
        self._engine_wrapper.shutdown()
        if self._client.is_connected():
            self._client.disconnect()

    def _topic_callback(self, dp_name, msg_str):
        logging.debug("Received datapack {}: {}".format(dp_name, msg_str))
        if dp_name not in self._dp_store:
            logging.warning("Received datapack {} is not registered".format(dp_name))
        elif len(self._dp_store[dp_name]) >= self._store_capacity:
            logging.debug("Received datapack {} but queue is fulled, it will be discarded".format(dp_name))
        else:
            dp = self._datapack_from_str(dp_name, msg_str)
            if dp:
                self._dp_store[dp_name].append(dp)

    @abstractmethod
    def _datapack_from_str(self, dp_name, msg_str):
        raise NotImplementedError("not implemented in EventLoopEngine base class")

    @abstractmethod
    def _get_datapack_as_str(self, dp_name):
        raise NotImplementedError("not implemented in EventLoopEngine base class")

    def _dp_topic_get(self, dp_name):
        return "{}/get/{}".format(self._engine_wrapper.get_engine_name(), dp_name)

    def _dp_topic_set(self, dp_name):
        return "{}/set/{}".format(self._engine_wrapper.get_engine_name(), dp_name)
    
    
def run_event_loop_engine_app(parse_config_f: callable, engine_wrapper_c: Type[EngineWrapper],
                              event_loop_engine_c: Type[EventLoopEngine]):
    log_levels = {'critical': logging.CRITICAL,
                 'fatal': logging.FATAL,
                 'error': logging.ERROR,
                 'warning': logging.WARNING,
                 'warn': logging.WARN,
                 'info': logging.INFO,
                 'debug': logging.DEBUG,
                 'notset': logging.NOTSET}

    parser = ArgumentParser()
    parser.add_argument('--config', type=str, required=True)
    parser.add_argument('--loglevel', type=str, required=False, default='INFO', choices=log_levels.keys())
    args = parser.parse_args()

    config = json.loads(parse_config_f(args.config))

    logging.basicConfig(level=log_levels[args.loglevel], format='[%(asctime)s] [{}] [%(levelname)s] %(message)s'
                        .format(config["EngineConfig"]["EngineName"]))

    engine = engine_wrapper_c()
    ele = event_loop_engine_c(config["Timestep"],
                                  config["TimestepWarnThreshold"],
                                  config["DataQueueSize"],
                                  config["ProcessLastMsg"],
                                  config["EngineConfig"],
                                  config["MQTTConfig"] if "MQTTConfig" in config else {},
                                  engine)

    def interrupt_handler(signum, frame):
        logging.info("Received stop signal")
        ele.stop_loop()

    signal.signal(signal.SIGINT, interrupt_handler)

    ele.initialize()
    ele.run_loop(config["Timeout"])
    ele.shutdown()
