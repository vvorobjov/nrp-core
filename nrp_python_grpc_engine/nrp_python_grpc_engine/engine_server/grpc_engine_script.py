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
import typing

from google.protobuf.message import Message
from engine_grpc_pb2 import DataPackMessage, DataPackIdentifier


class GrpcEngineScript:

    def __init__(self) -> None:
        """Constructor"""
        self._time_ns = 0
        self._name = ""
        self._config = None

        self._datapacks_msgs: typing.Dict[str, Message] = {}

    def _advanceTime(self, timestep_ns: int) -> None:
        """Advances the simulation time by given timestep"""
        self._time_ns = self._time_ns + timestep_ns

    def _registerDataPack(self, datapack_name: str, protobuf_type: Message) -> None:
        """
        Registers a datapack with given name in the engine.
        The datapack-handling functions can be called only on registered datapacks.

        :raises: ValueError: When there is a datapack already registered for datapack_name
        :raises: TypeError: protobuf_type has to be a type and a subclass of google.protobuf.message.Message
        """
        if datapack_name in self._datapacks_msgs:
            raise ValueError(f"[{self._name}] While registering Datapack."
                             f" DataPack with specified name ({datapack_name}) already registered")

        try:
            is_proto = issubclass(protobuf_type, Message)
        except TypeError as t_ex:
            raise TypeError(
                    f"[{self._name}] While registering Datapack. 'protobuf_type' argument must be a type") from t_ex
        else:
            if not is_proto:
                raise TypeError(f"[{self._name}] While registering Datapack."
                                f" 'protobuf_type' argument must be a Python Protobuf Message type")
        
        self._datapacks_msgs[datapack_name] = protobuf_type()

    def _getDataPack(self, datapack_name: str):
        """
        Returns cached data of the datapack with given name.

        :raises: ValueError: 'datapack_name' is not registered.
        """
        if datapack_name not in self._datapacks_msgs:
            raise ValueError(f"[{self._name}] While getting Datapack."
                             f" Attempting to get data from an unregistered DataPack ({datapack_name})")
        
        return self._datapacks_msgs[datapack_name]

    def _getDataPackMsg(self, datapack_id: DataPackIdentifier) -> DataPackMessage:
        """
        Returns a datapack msg with cached data with given datapack_id.

        :raises: ValueError: the datapack with 'datapack_id' is not bound to this engine.
        """
        datapack_name = datapack_id.dataPackName
        engine_name = datapack_id.engineName

        # Check if engine name in the request matches the actual name
        if engine_name != self._name:
            raise ValueError(f"Requesting DataPack '{datapack_name}'"
                             f" from incorrect engine (engine name in the request '{engine_name}',"
                             f" engine that received the request '{self._name}'")

        data = DataPackMessage()
        data.dataPackId.dataPackName = datapack_name
        data.dataPackId.engineName = engine_name
        data.data.Pack(self._getDataPack(datapack_name))

        return data

    def _setDataPack(self, datapack_name: str, data) -> None:
        """
        Sets cached data of the datapack with given name.

        :raises: ValueError: 'datapack_name' is not registered.
        :raises: TypeError: the type of 'data' is not the type of
                            the registered datapack 'datapack_name'.
        """
        if datapack_name not in self._datapacks_msgs:
            raise ValueError(f"[{self._name}] While setting Datapack. Attempting to set data on "
                             f"an unregistered DataPack ({datapack_name})")

        if not isinstance(data, type(self._datapacks_msgs[datapack_name])):
            raise TypeError(f"[{self._name}] While setting Datapack. DataPack ({datapack_name}) "
                            f"was registered with type {type(self._datapacks_msgs[datapack_name])}. "
                            f"Attempting to set it with type {type(data)}")

        self._datapacks_msgs[datapack_name] = data

    def _handleDataPackMsg(self, datapack: DataPackMessage) -> None:
        """
        Sets cached data of the specified datapack.

        :raises: ValueError: the datapack with 'datapack_id' is not bound to this engine.
        :raises: ValueError: the datapack is not registered.

        :raises: TypeError: datapack unpacking failed. Likely because of wrong type of data.
        """
        datapack_name = datapack.dataPackId.dataPackName
        engine_name = datapack.dataPackId.engineName

        # Check if engine name in the request matches the actual name
        if engine_name != self._name:
            raise ValueError(f"Sent DataPack '{datapack_name}'"
                             f" to incorrect engine (engine name in the request is '{engine_name}',"
                             f" engine that received the request is '{self._name}'")

        if datapack_name not in self._datapacks_msgs:
            raise ValueError(f"[{self._name}] While handling Datapack Message. Unregistered DataPack ({datapack_name})")

        if not datapack.data.Unpack(msg := self._datapacks_msgs[datapack_name]):
            raise TypeError(f"[{self._name}] While handling Datapack Message. DataPack ({datapack_name}) "
                            f"is of type {type(msg)}. Could not unpack the sent Datapack Message.")

    def initialize(self) -> None:
        """
        Initializes the simulation.

        This function should be overwritten by the derived class and
        should contain all steps necessary to initialize the simulation.
        """
        pass

    def runLoop(self, timestep_ns: int) -> None:
        """
        Runs a single step of the simulation.

        This function should be overwritten by the derived class and
        should contain all steps necessary to advance the simulation
        by the specified time.
        """
        pass

    def shutdown(self) -> None:
        """
        Shutdowns the simulation.

        This function should be overwritten by the derived class and
        should contain all steps necessary to cleanly shutdown the simulation.
        """
        pass

    def reset(self) -> None:
        """
        Resets the simulation.

        This function should be overwritten by the derived class and
        should contain all steps necessary to reset the simulation.
        """
        pass

# EOF
