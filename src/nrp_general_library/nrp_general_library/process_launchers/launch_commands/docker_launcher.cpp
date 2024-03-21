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

#include "nrp_general_library/process_launchers/launch_commands/docker_launcher.h"

#include "nrp_general_library/utils/nrp_exceptions.h"

#include <unistd.h>


DockerLauncher::~DockerLauncher()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    // Stop and remove the docker container
    this->stopProcess(60);
}

pid_t DockerLauncher::launchProcess(const nlohmann::json &launcherConfig, const std::string& procCmd,
                               const std::vector<std::string> &envParams,
                               const std::vector<std::string> &startParams,
                               bool /*appendParentEnv*/,
                               int logFD)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    _printLogs = launcherConfig["PrintLogs"];

    if(logFD >= 0)
        NRPLogger::debug("log File Descriptor (logFD) is not used by DockerLauncher");

    // Set NRPDockerHandle params
    _procCmd = procCmd;
    for(auto& param : startParams)
        _procCmd.append(" " + param);

    bpy::list envPythonL;
    for(size_t n = 0; n < envParams.size(); ++n)
        envPythonL.append(bpy::str(envParams[n]));


    // Call DockerHandle init with engine configuration
    bpy::object nrpModule = bpy::import(PYTHON_CLIENT_MODULE_NAME_STR);
    bpy::dict nrpDict(nrpModule.attr("__dict__"));

    try {
        _dockerHandle = nrpDict["NRPDockerHandle"](
                bpy::str(launcherConfig["DockerDaemonAddress"].get<std::string>()),
                bpy::str(launcherConfig["ImageName"].get<std::string>()),
                bpy::str(launcherConfig["UploadFolder"].get<std::string>()),
                bpy::str("nrp-run.bash " + _procCmd),
                envPythonL);
    }
    catch (bpy::error_already_set const &) {
        PyErr_Print();
        std::string msg = "Failed to launch Engine process \"" + _procCmd + "\"in Docker container";
        throw NRPException::logCreate(msg);
    }

    // Parameter end
    std::string id = bpy::extract<std::string>(_dockerHandle.attr("get_container_id")());
    this->_enginePID = !id.empty() ? 1 : 0;
    return this->_enginePID;
}

pid_t DockerLauncher::stopProcess(unsigned int /*killWait*/)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    if(this->_enginePID != 0)
    {
        try {
            NRPLogger::info("Stopping Engine process: " + _procCmd);
            _dockerHandle.attr("stop")();
            if(_printLogs) {
                NRPLogger::info("Engine process \"" + _procCmd + "\" docker logs start:\n\n" +
                this->getDockerLogs());
                NRPLogger::info("Engine process \"" + _procCmd + "\" docker logs end");
                _dockerHandle.attr("remove")();
            }
            this->_enginePID = 0;
        }
        catch (bpy::error_already_set const &) {
            PyErr_Print();
        }
    }

    return 0;
}

LaunchCommandInterface::ENGINE_RUNNING_STATUS DockerLauncher::getProcessStatus()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    if(this->_enginePID < 0)
        return ENGINE_RUNNING_STATUS::STOPPED;

    // Get status
    auto procStatus = ENGINE_RUNNING_STATUS::STOPPED;
    bpy::object pyProcStatus(_dockerHandle.attr("get_status")());
    if(not pyProcStatus.is_none()) {
        bpy::dict pyProcStatusDict(pyProcStatus);
        if(bpy::extract<bool>(pyProcStatusDict["running"]))
            procStatus = ENGINE_RUNNING_STATUS::RUNNING;
    }
    return procStatus;
}

std::string DockerLauncher::getDockerLogs()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    std::string logs = "";
    if(this->_enginePID > 0)
    {
        bpy::list pyLog(_dockerHandle.attr("get_logs")());
        for(auto i = 0; i < bpy::len(pyLog); ++i) {
            logs.append(bpy::extract<std::string>(pyLog[i]));
//            logs.append("\n");
        }
    }
    return logs;
}

