#ifndef EXAMPLE_ENGINE_CLIENT_H
#define EXAMPLE_ENGINE_CLIENT_H

#include "nrp_example_engine/config/example_config.h"
#include "nrp_general_library/engine_interfaces/engine_client_interface.h"
#include "nrp_general_library/plugin_system/plugin.h"

class ExampleEngineClient
        : public EngineClient<ExampleEngineClient, ExampleConfigConst::EngineSchema>
{
	public:

		/*!
		 * \brief Constructor
		 *
		 * The function should initialize all necessary structures and objects, like RPC stub or
		 * REST client. It should also, if it's possible, establish a connection with the engine
		 * server.
		 *
		 * \param[in] config Simulation configuration. Will be stored by the client for further use
		 *            and should be accessible with getEngineConfig method.
		 * \param[in] launcher Process launcher for the engine server. Based on the configuration,
		 *            it will either spawn a server in a separate process, or do nothing.
		 */
		ExampleEngineClient(nlohmann::json &config, ProcessLauncherInterface::unique_ptr &&launcher);

		/*!
		 * \brief Initializes the engine
		 *
		 * The function will be called at the start of the simulation. It should
		 * execute RPC / REST calls that will prepare the engine server and simulator
		 * to run the simulation.
		 */
		void initialize() override;

		void reset() override;

		/*!
		 * \brief Shutdowns the engine
		 *
		 * The function will be called at the end of the simulation. It should
		 * execute RPC / REST calls that will shutdown the simulator and engine
		 * server gracefully.
		 */
		void shutdown() override;

		void sendDevicesToEngine(const devices_ptr_t &devicesArray) override;
		devices_set_t getDevicesFromEngine(const device_identifiers_set_t &deviceIdentifiers) override;

		/*!
		 * \brief Returns start parameters that should be passed to the engine server
		 *
		 * The function should prepare and return start parameters that are going to be passed
		 * to the engine server on startup.
		 *
		 * \return Vector of start parameters for the engine server. The parameters must
		 *         be strings.
		 */
		const std::vector<std::string> engineProcStartParams() const override;

		/*!
		 * \brief Returns environment variables that should be used when starting the engine server
		 *
		 * The function should prepare and return additional environment variables that
		 * will be used when spawning the engine server process.
		 *
		 * \return Vector of start parameters for the engine server. The parameters must
		 *         be strings.
		 */
		const std::vector<std::string> engineProcEnvParams() const override;

	protected:

		/*!
		 * \brief Runs a single step of the simulation
		 *
		 * The function will be called every simulation loop, after getDevicesFromEngine and
		 * before sendDevicesToEngine. It should execute RPC / REST calls that will advance the simulation
		 * by the requested time step, and that will retrieve current simulation time after the current
		 * step is completed.
		 *
		 * \param[in] timeStep Requested time step, by which the simulation should advance
		 *
		 * \return Simulation time after the loop step
		 */
		SimulationTime runLoopStepCallback(SimulationTime timeStep) override;
};

using ExampleEngineLauncher = ExampleEngineClient::EngineLauncher<ExampleConfigConst::EngineType>;

CREATE_NRP_ENGINE_LAUNCHER(ExampleEngineLauncher);


#endif // EXAMPLE_ENGINE_CLIENT_H
