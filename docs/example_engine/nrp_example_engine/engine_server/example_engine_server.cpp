#include "nrp_example_engine/engine_server/example_engine_server.h"

ExampleEngineServer::ExampleEngineServer(const std::string &engineName, const std::string &, const std::string &)
    : _engineName(engineName)
{}

bool ExampleEngineServer::initRunFlag() const
{
    return this->_initRunFlag;
}

bool ExampleEngineServer::shutdownFlag() const
{
    return this->_shutdownFlag;
}

bool ExampleEngineServer::isServerRunning() const
{
    return this->_isServerRunning;
}

void ExampleEngineServer::shutdown()
{
    this->_shutdownFlag = true;
}

void ExampleEngineServer::initialize()
{
    this->_initRunFlag = true;
}

void ExampleEngineServer::startServerAsync()
{
    // This method should start a server in asynchronous mode
}

// EOF
