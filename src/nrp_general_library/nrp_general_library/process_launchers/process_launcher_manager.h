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

#ifndef PROCESS_LAUNCHER_MANAGER_H
#define PROCESS_LAUNCHER_MANAGER_H

#include "nrp_general_library/process_launchers/process_launcher.h"
#include "nrp_general_library/process_launchers/process_launcher_basic.h"
#include "nrp_general_library/utils/nrp_exceptions.h"

#include <iostream>
#include <map>

/*!
 * \brief Class to manage process managers
 */
template<class ...PROCESS_LAUNCHERS>
class ProcessLauncherManager
        : public PtrTemplates<ProcessLauncherManager<PROCESS_LAUNCHERS...> >
{
    public:
        /*!
         * \brief Constructor. Registers all Process Launchers for further use
         */
        ProcessLauncherManager(int logFD = -1) :
            _logFD(logFD)
        {   this->registerProcessLauncher<PROCESS_LAUNCHERS...>();  }
        ~ProcessLauncherManager() = default;

        // Delete copy and move constructors
        ProcessLauncherManager(const ProcessLauncherManager&) = delete;
        ProcessLauncherManager(ProcessLauncherManager&&) = delete;

        ProcessLauncherManager &operator=(ProcessLauncherManager&&) = delete;
        ProcessLauncherManager &operator=(const ProcessLauncherManager&) = delete;

        /*!
         * \brief Create a new process launcher
         * \param launcherType Name of launcher
         * \return Returns ptr to launcher
         */
        ProcessLauncherInterface::unique_ptr createProcessLauncher(const std::string &launcherType) const
        {
            NRP_LOGGER_TRACE("{} called", __FUNCTION__);
            //ProcessLauncherManager *const manager = ProcessLauncherManager::getInstance();

            const auto launcherIterator = this->_processLaunchers.find(launcherType);
            if(launcherIterator == this->_processLaunchers.end())
                throw NRPException::logCreate("Could not find process launcher of type \"" + launcherType + "\"");

            return launcherIterator->second->createLauncher(_logFD);
        }

        void registerProcessLauncher(ProcessLauncherInterface::unique_ptr &&launcher)
        {   this->_processLaunchers.emplace(launcher->launcherName(), std::move(launcher)); }

    private:

        /*!
         * \brief All Process Launchers
         */
        std::map<std::string, ProcessLauncherInterface::unique_ptr> _processLaunchers;

        /*!
         * \brief logFD File descriptor to route stdout and stderror outputs in launched process
         */
         int _logFD = -1;

        /*!
         *  \brief Register process launchers specified in the template on startup
         *  \tparam PROCESS_LAUNCHER Launcher class to register
         *  \tparam REST Remaining launchers to register
         */
        template<class PROCESS_LAUNCHER, class ...REST>
        void registerProcessLauncher()
        {
            static_assert(std::is_base_of_v<ProcessLauncherInterface, PROCESS_LAUNCHER> && std::is_convertible_v<const volatile PROCESS_LAUNCHER*, const volatile ProcessLauncherInterface*>,"Parameter PROCESS_LAUNCHER must derive from ProcessLauncherInterface");

            NRP_LOGGER_TRACE("{} called", __FUNCTION__);
            
            ProcessLauncherInterface::unique_ptr launcher(new PROCESS_LAUNCHER());
            this->registerProcessLauncher(std::move(launcher));

            if constexpr (sizeof... (REST) > 0)
            {   this->registerProcessLauncher<REST...>();   }
        }
};

/*!
 * \brief Type to manage all available process launchers
 */
using MainProcessLauncherManager = ProcessLauncherManager<ProcessLauncherBasic>;

using MainProcessLauncherManagerSharedPtr = MainProcessLauncherManager::shared_ptr;
using MainProcessLauncherManagerConstSharedPtr = MainProcessLauncherManager::const_shared_ptr;


#endif // PROCESS_LAUNCHER_MANAGER_H
