/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2021 NRP Team
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project has received funding from the European Union’s Horizon 2020
 * Framework Programme for Research and Innovation under the Specific Grant
 * Agreement No. 945539 (Human Brain Project SGA3).
 */

#ifndef PY_ENGINE_SCRIPT_H
#define PY_ENGINE_SCRIPT_H

#include "nrp_general_library/utils/ptr_templates.h"
#include "nrp_python_json_engine/engine_server/python_json_server.h"

#include <boost/python.hpp>
#include <map>

/*!
 * \brief C++ class to interface with user-defined Python script class
 */
class PyEngineScript
        : public PtrTemplates<PyEngineScript>
{
	public:
		PyEngineScript() = default;
		virtual ~PyEngineScript();

		/*!
		 * \brief Initialization function. Called at start of simulation
		 */
		virtual void initialize();

		/*!
		 * \brief Runs main script loop and updates _time
		 * \param timestep Time (in seconds) to run the loop
		 * \return Returns total simulation time of this engine
		 */
		inline SimulationTime runLoop(SimulationTime timestep)
		{
			this->_time += timestep;
			this->runLoopFcn(timestep);

			return this->_time;
		}

		/*!
		 * \brief Shutdown function. Called at end of simulation
		 */
		virtual void shutdown();

		/*!
		 * \brief Get simulation time of this engine
		 */
		SimulationTime simTime() const;

		/*!
		 * \brief Register datapack
		 * \param datapackName Name of datapack
		 */
		void registerDataPack(std::string datapackName);

		/*!
		 * \brief Get DataPack Data
		 * \param datapackName Name of datapack
		 * \return Returns datapack data
		 */
		boost::python::object &getDataPack(const std::string &datapackName);

		/*!
		 * \brief Set DataPack Data
		 * \param datapackName Name of datapack
		 * \param data Data to store in datapack
		 */
		void setDataPack(const std::string &datapackName, boost::python::object data);

		/*!
		 * \brief Save ptr to PythonJSONServer instance that owns this script
		 * \param pServer Pointer to PythonJSONServer
		 */
		void setPythonJSONServer(PythonJSONServer *pServer);

	protected:
		/*!
		 * \brief Main script loop. Will run for timestep seconds
		 * \param timestep Time (in seconds) to run the loop
		 */
		virtual void runLoopFcn(SimulationTime timestep) = 0;

	private:
		/*!
		 * \brief Engine Time
		 */
		SimulationTime _time = SimulationTime::zero();

		/*!
		 * \brief Ptr to PythonJSONServer instance that owns this script
		 */
		PythonJSONServer *_pServer = nullptr;

		/*!
		 * \brief DataPack Controllers
		 */
		std::list<std::shared_ptr<PythonEngineJSONDataPackController>> _datapackControllers;

		/*!
		 * \brief Map from keyword to datapack data
		 */
		std::map<std::string, boost::python::object*> _nameDataPackMap;
};

using PyEngineScriptSharedPtr = PyEngineScript::shared_ptr;
using PyEngineScriptConstSharedPtr = PyEngineScript::const_shared_ptr;

#endif // PY_ENGINE_SCRIPT_H
