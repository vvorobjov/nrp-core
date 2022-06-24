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

#include "nrp_general_library/process_launchers/launch_commands/docker_launcher.h"

#include "nrp_general_library/utils/nrp_exceptions.h"

#include <unistd.h>


DockerLauncher::~DockerLauncher()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    // Stop and remove the docker container in a remote server 
    this->stopProcess(60);
}

pid_t DockerLauncher::launchProcess(const nlohmann::json &launcherConfig, const std::string& procCmd,
                               const std::vector<std::string> &envParams,
                               const std::vector<std::string> &startParams,
                               bool /*appendParentEnv*/,
                               int logFD)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    if(logFD >= 0)
        NRPLogger::debug("log File Descriptor (logFD) is not used by DockerLauncher");

    // Set engineConfig and pass it to DockerConnector
    nlohmann::json configInfo = launcherConfig;
    configInfo["ProcCmd"] = "nrp-run.bash " + procCmd;
    configInfo["ProcCmdArgs"] = startParams;
    configInfo["ExecEnvironment"] = envParams;

    // Call DockerConnector init with engine configuration
    Py_Initialize();
    PyObject* pArg = PyTuple_New(1);                              
    PyTuple_SetItem(pArg, 0, Py_BuildValue("s", configInfo.dump().c_str()));

    PyObject* pModule = PyImport_ImportModule(PY_CORE_MODULE);
    PyObject* pClass = PyObject_GetAttrString(pModule, "DockerConnector");
    PyObject* pObject = PyEval_CallObject(pClass, pArg);
    
    this->pCID = PyObject_GetAttrString(pObject, "get_container_id");
    this->pLog = PyObject_GetAttrString(pObject, "get_log_info");
    this->pInit = PyObject_GetAttrString(pObject, "initializing");
    this->pInspect = PyObject_GetAttrString(pObject, "get_container_status");
    this->pShutdown = PyObject_GetAttrString(pObject, "shutdown");
    std::string tmpLog = this->getDockerInfo(this->pLog);
    if(tmpLog != ""){
        NRPLogger::error("{}", tmpLog);
        exit(-1);
    }

    NRPLogger::info("Remote server {} is connected! ",
     configInfo["DockerServerAddress"]);

    PyEval_CallObject(this->pInit, NULL);
    tmpLog = this->getDockerInfo(this->pLog);
    if(tmpLog != ""){
        NRPLogger::error("{}", tmpLog);
        exit(-1);
    }

    // Parameter end
    const auto tPid = std::stoi(this->getDockerInfo(this->pCID));
    this->_enginePID = tPid;
    NRPLogger::info("Remote container ID: {}", tPid);
    return tPid;
}

pid_t DockerLauncher::stopProcess(unsigned int killWait)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    if(this->_enginePID > 0)
    {
        usleep(10*1000);
        PyEval_CallObject(this->pShutdown, NULL);
        // Set to maximum wait time if time is set to 0
        if(killWait == 0)
            killWait = std::numeric_limits<unsigned int>::max();
        const auto end = std::chrono::system_clock::now() + std::chrono::duration<size_t>(killWait);
        while(std::chrono::system_clock::now() < end){
            if(this->getProcessStatus() == ENGINE_RUNNING_STATUS::STOPPED)
                break;
            usleep(10*1000);
            PyEval_CallObject(this->pShutdown, NULL);
        }
        this->_enginePID = -1;
    }
    Py_Finalize();
    return 0;
}

LaunchCommandInterface::ENGINE_RUNNING_STATUS DockerLauncher::getProcessStatus()
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    if(this->_enginePID < 0)
        return ENGINE_RUNNING_STATUS::STOPPED;
    // Check if the remote contained was already removed before
    std::string tmpStatus = "";
    tmpStatus = this->getDockerInfo(this->pInspect);
    std::string tmpLog = "";
    tmpLog = this->getDockerInfo(this->pLog);
    if(tmpLog != ""){
        NRPLogger::error("{}", tmpLog);
        exit(-1);
    }
    else if (tmpStatus == "STOPPED"){
        return ENGINE_RUNNING_STATUS::STOPPED;
    }
    return ENGINE_RUNNING_STATUS::RUNNING;
}

std::string DockerLauncher::getDockerInfo(PyObject* pObj){
    PyObject* pRet1 = PyEval_CallObject(pObj, NULL);
    char* ipInfo=0;
    PyArg_Parse(pRet1, "s", &ipInfo);
    std::string backStr(ipInfo);

    return backStr;
}

