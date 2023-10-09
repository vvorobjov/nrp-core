//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2023 NRP Team
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
    static constexpr char EngineSchema[] = "json://nrp-core/engines/engine_base.json#EngineBase";
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

        virtual void sendDataPacksToEngine(const datapacks_set_t & /*dataPacks*/) override
        {}

        SimulationTime runLoopStepCallback(SimulationTime /*timeStep*/) override
        {
            return SimulationTime::zero();
        }

        virtual datapacks_vector_t getDataPacksFromEngine(const datapack_identifiers_set_t &datapackIdentifiers) override
        {
            datapacks_vector_t retVal;
            for(const auto &devID : datapackIdentifiers)
            {
                retVal.push_back(std::shared_ptr<DataPackInterface>(new DataPackInterface(devID)));
            }

            return retVal;
        }
};

CREATE_NRP_ENGINE_LAUNCHER(TestEngine::EngineLauncher<TestEngineConfigConst::EngineType>);
