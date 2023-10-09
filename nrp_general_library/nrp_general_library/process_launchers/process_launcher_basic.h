/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2023 NRP Team
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

#ifndef PROCESS_LAUNCHER_BASIC_H
#define PROCESS_LAUNCHER_BASIC_H

#include "nrp_general_library/config/cmake_constants.h"
#include "nrp_general_library/process_launchers/process_launcher.h"
#include "nrp_general_library/process_launchers/launch_commands/basic_fork.h"
#include "nrp_general_library/process_launchers/launch_commands/docker_launcher.h"
#include "nrp_general_library/process_launchers/launch_commands/empty_launch_command.h"


/*!
 * \brief Basic Process Launcher, for simple process management
 */
inline const char Basic[] = "Basic";

class ProcessLauncherBasic
        : public ProcessLauncher<ProcessLauncherBasic, Basic, BasicFork,
            DockerLauncher, EmptyLaunchCommand>
{
    public: ~ProcessLauncherBasic() override = default;
};

#endif // PROCESS_LAUNCHER_BASIC_H
