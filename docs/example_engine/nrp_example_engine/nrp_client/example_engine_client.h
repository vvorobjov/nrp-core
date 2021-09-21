#ifndef EXAMPLE_ENGINE_CLIENT_H
#define EXAMPLE_ENGINE_CLIENT_H

#include "nrp_example_engine/config/example_config.h"
#include "nrp_general_library/engine_interfaces/engine_client_interface.h"
#include "nrp_general_library/plugin_system/plugin.h"

#include <future>

class ExampleEngineClient
        : public EngineClient<ExampleEngineClient, ExampleConfigConst::EngineSchema>
{
	public:
		ExampleEngineClient(nlohmann::json &config, ProcessLauncherInterface::unique_ptr &&launcher)
		    : EngineClient(config, std::move(launcher))
		{}

		virtual ~ExampleEngineClient() override;

		virtual void initialize() override;
		virtual void reset() override;
		virtual void shutdown() override;

		virtual SimulationTime getEngineTime() const override
		{
			return this->_engineTime;
		}

		SimulationTime runLoopStepCallback(SimulationTime) override
		{
			return getEngineTime();
		}

		virtual void sendDevicesToEngine(const devices_ptr_t &devicesArray) override;
		virtual devices_set_t getDevicesFromEngine(const device_identifiers_set_t &deviceIdentifiers) override;

		virtual const std::vector<std::string> engineProcStartParams() const override;
        virtual const std::vector<std::string> engineProcEnvParams() const override;

	private:
		SimulationTime _engineTime = SimulationTime::zero();
		std::future<SimulationTime> _loopStepThread;
};

using ExampleEngineLauncher = ExampleEngineClient::EngineLauncher<ExampleConfigConst::EngineType>;

CREATE_NRP_ENGINE_LAUNCHER(ExampleEngineLauncher);


#endif // EXAMPLE_ENGINE_CLIENT_H
