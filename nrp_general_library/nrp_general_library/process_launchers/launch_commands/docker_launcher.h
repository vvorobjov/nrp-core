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

#ifndef DOCKER_LAUNCHER_H
#define DOCKER_LAUNCHER_H

#include "nrp_general_library/config/cmake_constants.h"
#include "nrp_general_library/process_launchers/launch_commands/launch_command.h"
#include <iostream>

#include "Python.h"
#define PY_CORE_MODULE "nrp_core"

inline const char LAUNCH_DOCKER_COMMAND[] = "DockerLauncher";

class DockerLauncher
        : public LaunchCommand<LAUNCH_DOCKER_COMMAND>
{
    public:
        ~DockerLauncher() override;

    pid_t launchProcess(const nlohmann::json &launcherConfig, const std::string& procCmd,
                        const std::vector<std::string> &envParams,
                        const std::vector<std::string> &startParams, bool appendParentEnv = true,
                        int logFD = -1) override;

        pid_t stopProcess(unsigned int killWait) override;

        ENGINE_RUNNING_STATUS getProcessStatus() override;

    private:
        /*!
         * \brief ID of the docker container in a remote server
         */
        pid_t _enginePID = -1;
        /*!
         * \brief function object of python to connect remote server
         */
        PyObject* pCID = NULL;
        PyObject* pLog = NULL;
        PyObject* pInit = NULL;
        PyObject* pInspect = NULL;
        PyObject* pShutdown = NULL;

        std::string getDockerInfo(PyObject* pObj);
};

#endif // DOCKER_LAUNCHER_H
