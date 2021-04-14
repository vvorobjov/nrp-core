class ExampleEngineGrpcNRPClient
: public EngineGrpcClient<ExampleEngineGrpcNRPClient, ExampleGrpcConfigConst::EngineSchema, ExampleDevice1, ExampleDevice2, ExampleDevice3>
{
	public:
		ExampleEngineGrpcNRPClient(nlohmann::json &config, ProcessLauncherInterface::unique_ptr &&launcher);
		virtual ~ExampleEngineGrpcNRPClient() override = default;

		virtual void initialize() override;

		virtual void shutdown() override;

        virtual const std::vector<std::string> engineProcStartParams() const override;

        virtual const std::vector<std::string> engineProcEnvParams() const override;
};

using ExampleEngineGrpcLauncher = ExampleEngineGrpcNRPClient::EngineLauncher<ExampleGrpcConfigConst::EngineType>;

CREATE_NRP_ENGINE_LAUNCHER(ExampleEngineGrpcLauncher);
