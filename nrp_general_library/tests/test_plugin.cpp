//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2021 NRP Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This project has received funding from the European Union’s Horizon 2020
// Framework Programme for Research and Innovation under the Specific Grant
// Agreement No. 945539 (Human Brain Project SGA3).
//

#include "nrp_general_library/engine_interfaces/engine_client_interface.h"

#include "nrp_general_library/plugin_system/plugin.h"

struct TestEngineConfigConst
{
	static constexpr char EngineType[] = "test_engine";
    static constexpr char EngineSchema[] = "https://neurorobotics.net/engines/engine_base.json#EngineBase";
};

class TestEngine
        : public EngineClient<TestEngine, TestEngineConfigConst::EngineSchema>
{
	public:
		TestEngine(nlohmann::json  &configHolder, ProcessLauncherInterface::unique_ptr &&launcher)
		    : EngineClient(configHolder, std::move(launcher))
		{}

		virtual void initialize() override
		{}

		virtual void reset() override
		{}

		virtual void shutdown() override
		{}

        virtual const std::vector<std::string> engineProcStartParams() const override
        { return std::vector<std::string>(); }

        virtual const std::vector<std::string> engineProcEnvParams() const override
        { return std::vector<std::string>(); }

		virtual SimulationTime getEngineTime() const override
		{	return SimulationTime::zero();	}

		virtual void runLoopStep(SimulationTime) override
		{}

		virtual void waitForStepCompletion(float) override
		{}

		virtual void sendDevicesToEngine(const devices_ptr_t &) override
		{}


		virtual devices_set_t getDevicesFromEngine(const device_identifiers_set_t &deviceIdentifiers) override
		{
			devices_set_t retVal;
			for(const auto &devID : deviceIdentifiers)
			{
				retVal.emplace(new DeviceInterface(devID));
			}

			return retVal;
		}
};

CREATE_NRP_ENGINE_LAUNCHER(TestEngine::EngineLauncher<TestEngineConfigConst::EngineType>);
