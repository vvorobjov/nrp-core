#ifndef EXAMPLE_ENGINE_SERVER_H
#define EXAMPLE_ENGINE_SERVER_H

#include "nrp_json_engine_protocol/datapack_interfaces/json_datapack.h"

class ExampleEngineServer
{
    public:
        ExampleEngineServer(const std::string &engineName, const std::string & a, const std::string & b);
        ~ExampleEngineServer() = default;

        bool initRunFlag() const;
        bool shutdownFlag() const;
        bool isServerRunning() const;

        constexpr const std::string &engineName() const
        {   return this->_engineName;   }

        float runLoopStep(float timeStep);
        void initialize();
        void shutdown();
        void startServerAsync();

    private:
        bool _initRunFlag = false;
        bool _shutdownFlag = false;
        bool _isServerRunning = false;

        std::string _engineName;
};

#endif // EXAMPLE_ENGINE_SERVER_H
