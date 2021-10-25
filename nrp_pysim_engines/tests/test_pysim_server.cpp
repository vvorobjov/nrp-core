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

#include <gtest/gtest.h>

#include "nrp_general_library/utils/python_interpreter_state.h"
#include "nrp_python_json_engine/engine_server/python_json_server.h"
#include "nrp_opensim_engine/config/opensim_config.h"
#include "tests/test_env_cmake.h"

#include <boost/python.hpp>
#include <future>
#include <restclient-cpp/restclient.h>

namespace python = boost::python;

TEST(TestPySimServer, TestSimulatorDevices)
{
    std::string argvDat = "TestProg";
    char *argv = &argvDat[0];
    PythonInterpreterState pyState(1, &argv);

    auto pyGlobals = python::dict(python::import("__main__").attr("__dict__"));
    python::object pyLocals;

    nlohmann::json config;
    config["EngineName"] = "test_opensim_init";
    config["EngineType"] = "test_engine_opensim";
    config["Simulator"] = "Opensim";
    config["EngineTimestep"] = "0.01";
    config["PythonFileName"] = TEST_SIMULATOR_SIMPLE_FILE_NAME;
    config["WorldFileName"] = TEST_SIMULATOR_SIM_WORLD_NAME;
    config["Visualizer"] = false;
    std::string server_address = "localhost:1234";
    config["ServerAddress"] = server_address;

    PythonJSONServer server(config.at("ServerAddress"), pyGlobals);

    // Test Init
    pyState.allowThreads();

    EngineJSONServer::mutex_t fakeMutex;
    EngineJSONServer::lock_t fakeLock(fakeMutex);
    nlohmann::json respParse = server.initialize(config, fakeLock);

    const auto execResult = respParse[OpenSimConfigConst::InitFileExecStatus.data()].get<bool>();
    ASSERT_EQ(execResult, true);
    ASSERT_EQ(server.initRunFlag(), true);

    // Test runStep REST call
    const SimulationTime timeStep(1000);
    ASSERT_EQ(server.runLoopStep(timeStep), timeStep);

    // Test getDataPack REST call EngineServerGetDataPacksRoute
    server.startServerAsync();

    pyState.endAllowThreads();
    server.shutdownServer();
}