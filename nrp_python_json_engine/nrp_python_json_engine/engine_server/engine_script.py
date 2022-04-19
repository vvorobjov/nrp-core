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


class EngineScript:

    def __init__(self) -> None:
        """Constructor"""
        self._datapacks = {}
        self._time_ns = 0
        self._name = ""
        self._config = None


    def _advanceTime(self, timestep_ns: int) -> None:
        """Advances the simulation time by given timestep"""
        self._time_ns = self._time_ns + timestep_ns


    def _registerDataPack(self, datapack_name: str) -> None:
        """
        Registers a datapack with given name in the engine.
        The datapack-handling functions can be called only on registered datapacks"
        """
        if datapack_name in self._datapacks:
            raise Exception(f"DataPack with specified name ({datapack_name}) already registered")

        self._datapacks[datapack_name] = None


    def _getDataPack(self, datapack_name: str) -> dict:
        """Returns cached data of the datapack with given name"""
        if datapack_name not in self._datapacks:
            raise Exception(f"Attempting to get data from an unregistered DataPack ({datapack_name})")

        return self._datapacks[datapack_name]


    def _setDataPack(self, datapack_name: str, data: dict) -> None:
        """Sets cached data of the datapack with given name"""
        if datapack_name not in self._datapacks:
            raise Exception(f"Attempting to set data on an unregistered DataPack ({datapack_name})")

        self._datapacks[datapack_name] = data


    def initialize(self) -> None:
        """
        Initializes the simulation.

        This function should be overwritten by the derived class and
        should contain all steps necessary to initialize the simulation.
        """
        pass


    def runLoop(self, timestep_ns: int) -> None:
        """
        Runs a signle step of the simulation.

        This function should be overwritten by the derived class and
        should contain all steps necessary to advance the simulation
        by the specified time.
        """
        pass


    def shutdown(self) -> None:
        """
        Shutdowns the simulation.

        This function should be overwritten by the derived class and
        should contain all steps necessary to clearly shutdown the simulation.
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
