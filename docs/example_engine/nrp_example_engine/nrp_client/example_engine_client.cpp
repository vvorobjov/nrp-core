#include "nrp_example_engine/nrp_client/example_engine_client.h"

ExampleEngineClient::ExampleEngineClient(nlohmann::json &config, ProcessLauncherInterface::unique_ptr &&launcher)
        : EngineClient(config, std::move(launcher))
{

}

void ExampleEngineClient::initialize()
{

}

void ExampleEngineClient::reset()
{

}

void ExampleEngineClient::shutdown()
{

}

void ExampleEngineClient::sendDataPacksToEngine(const datapacks_ptr_t &datapacksArray)
{
    (void)datapacksArray;
}

EngineClientInterface::datapacks_set_t ExampleEngineClient::getDataPacksFromEngine(const datapack_identifiers_set_t &datapackIdentifiers)
{
    (void)datapackIdentifiers;
    return EngineClientInterface::datapacks_set_t();
}

const std::vector<std::string> ExampleEngineClient::engineProcStartParams() const
{
    return std::vector<std::string>();
}

const std::vector<std::string> ExampleEngineClient::engineProcEnvParams() const
{
    return std::vector<std::string>();
}

SimulationTime ExampleEngineClient::runLoopStepCallback(SimulationTime)
{
    return getEngineTime();
}

// EOF
