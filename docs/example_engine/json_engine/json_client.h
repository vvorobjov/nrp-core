class ExampleEngineJSONNRPClient
: public EngineJSONNRPClient<ExampleEngineGrpcNRPClient, ExampleJSONConfigConst::EngineSchema, ExampleDevice1, ExampleDevice2, ExampleDevice3>
{
	public:
		ExampleEngineJSONNRPClient(nlohmann::json &config, ProcessLauncherInterface::unique_ptr &&launcher);
		virtual ~ExampleEngineJSONNRPClient() override = default;

		virtual void initialize() override;

		virtual void shutdown() override;

        virtual const std::vector<std::string> engineProcStartParams() const override;

        virtual const std::vector<std::string> engineProcEnvParams() const override;
};

using ExampleEngineJSONLauncher = ExampleEngineJSONNRPClient::EngineLauncher<ExampleJSONConfigConst::EngineType>;

CREATE_NRP_ENGINE_LAUNCHER(ExampleEngineJSONLauncher);