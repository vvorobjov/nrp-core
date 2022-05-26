/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2021 NRP Team
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project has received funding from the European Union’s Horizon 2020
 * Framework Programme for Research and Innovation under the Specific Grant
 * Agreement No. 945539 (Human Brain Project SGA3).
 */

#ifndef BASIC_FORK_H
#define BASIC_FORK_H

#include "nrp_general_library/config/cmake_constants.h"
#include "nrp_general_library/process_launchers/launch_commands/launch_command.h"

inline const char LAUNCH_COMMAND[] = "BasicFork";

class BasicFork
        : public LaunchCommand<LAUNCH_COMMAND>
{
        /*!
         *  \brief Command to set environment variables. More versatile than the C function setenv
         */
        static constexpr std::string_view EnvCfgCmd = NRP_ENGINE_SET_ENV_CMD;

    public:
        ~BasicFork() override;

        pid_t launchProcess(const std::string& procCmd, const std::vector<std::string> &envParams,
                            const std::vector<std::string> &startParams, bool appendParentEnv = true,
                                          int logFD = -1) override;

        pid_t launchEngineProcess(const nlohmann::json &launcher_info, const std::vector<std::string> &envParams,
                                  const std::vector<std::string> &startParams, bool appendParentEnv = true) override;

        pid_t stopProcess(unsigned int killWait) override;

        ENGINE_RUNNING_STATUS getProcessStatus() override;

    private:
        /*!
         * \brief PID of child process running the engine
         */
        pid_t _PID;

        /*!
         * \brief Add environment variables to environ
         * \param envVars Environment variables
         */
        static void appendEnvVars(const std::vector<std::string> &envVars);
};

#endif // BASIC_FORK_H
