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

#ifndef EMPTY_LAUNCH_H
#define EMPTY_LAUNCH_H

#include "nrp_general_library/config/cmake_constants.h"
#include "nrp_general_library/process_launchers/launch_commands/launch_command.h"

/*!
 * \brief Empty Launch Command. A "dummy" launcher that doesn't launch a process.
          Useful in the cases when the EngineServer is not intended to be launched by the corresponding EngineClient.
 */
inline const char EmptyLaunchC[] = "EmptyLaunchCommand";

class EmptyLaunchCommand
        : public LaunchCommand<EmptyLaunchC>
{

    public:
        ~EmptyLaunchCommand() = default;

        /*!
        * \brief launchEngineProcess always returns -1
        * \return -1
        */
        pid_t launchEngineProcess(const nlohmann::json &, const std::vector<std::string> &, const std::vector<std::string> &, bool )
        {
            return -1;
        }

        /*!
        * \brief stopEngineProcess always returns 0
        * \return 0
        */
        pid_t stopEngineProcess(unsigned int) override 
        {
            return 0;
        }

        /*!
        * \brief getProcessStatus always returns ENGINE_RUNNING_STATUS::UNKNOWN
        * \return ENGINE_RUNNING_STATUS::UNKNOWN
        */
        ENGINE_RUNNING_STATUS getProcessStatus() override 
        {
            return ENGINE_RUNNING_STATUS::UNKNOWN;
        }

};  

#endif // EMPTY_LAUNCH_H
