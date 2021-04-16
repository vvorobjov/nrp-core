#include "nrp_example_engine/nrp_client/example_engine_client.h"

ExampleEngineClient::~ExampleEngineClient()
{}

void ExampleEngineClient::initialize()
{

}

void ExampleEngineClient::shutdown()
{

}

SimulationTime ExampleEngineClient::sendRunLoopStepCommand(SimulationTime timeStep)
{
    return SimulationTime::zero();
}

void ExampleEngineClient::sendDevicesToEngine(const devices_ptr_t &devicesArray)
{

}

EngineClientInterface::devices_set_t ExampleEngineClient::getDevicesFromEngine(const device_identifiers_set_t &deviceIdentifiers)
{
    return EngineClientInterface::devices_set_t();
}

const std::vector<std::string> ExampleEngineClient::engineProcStartParams() const
{
    return std::vector<std::string>();
}

const std::vector<std::string> ExampleEngineClient::engineProcEnvParams() const
{
    return std::vector<std::string>();
}
