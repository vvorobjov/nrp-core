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

#include "nrp_general_library/utils/nrp_logger.h"

#include <iomanip>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <unistd.h>
#include <vector>
#include <algorithm>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */

#include <iostream>

bool NRPLogger::shutdownSharedMemory(int& fd){
    close(fd);
    shm_unlink(_sharedMemCfgName.data());
    return false;
}

bool NRPLogger::setSharedMemoryForLauncher()
{
    int error = 0;
    // Trying to open the SHared Memory object for saving setting
    int fd = shm_open(_sharedMemCfgName.data(), O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd != -1)
    {
        //Trying to resize ShM with the size of the settings structure
        error = ftruncate(fd, sizeof(LoggerConfig));

        // Mapping ShM
        LoggerConfig *cfg;
        cfg = static_cast<LoggerConfig *>(mmap(NULL, sizeof(LoggerConfig), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
        if (cfg == MAP_FAILED)
        {
            return shutdownSharedMemory(fd);
        }
        else
        {
            // Init semaphores
            error = sem_init(&cfg->sem1, 1, 0);
            if (error == -1){
                return shutdownSharedMemory(fd);
            }

            // Writing settings
            cfg->consoleLogLevel = (uint)_consoleLogLevel;
            cfg->fileLogLevel = (uint)_fileLogLevel;
            cfg->logDirLen = _logDir.length();
            memcpy(&cfg->logDir, _logDir.c_str(), _logDir.length());

            // Sending signal that writing is finished
            error = sem_post(&cfg->sem1);
            if (error == -1){
                return shutdownSharedMemory(fd);
            }
        }
    }
    else
    {
        return shutdownSharedMemory(fd);
    }

    close(fd);
    return true;
}

bool NRPLogger::getSharedMemoryFromLauncher(){
    int error = 0;
    // Trying to open the SHared Memory object for loading of setting
    int fd = shm_open(_sharedMemCfgName.data(), O_RDWR, 0);
    if (fd != -1)
    {
        // Mapping ShM
        LoggerConfig *cfg;
        cfg = static_cast<LoggerConfig *>(mmap(NULL, sizeof(LoggerConfig), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
        if (cfg == MAP_FAILED)
        {
            return shutdownSharedMemory(fd);
        }
        else
        {
            // semaphore waiting timeout 1.0s
            struct timespec tp;
            tp.tv_sec = 1;
            tp.tv_nsec = 0;

            // waiting for the signal that the settings are written
            error = sem_timedwait(&cfg->sem1, &tp);
            if (error == -1)
            {
                return shutdownSharedMemory(fd);
            }

            error = sem_init(&cfg->sem1, 1, 0);
            if (error == -1){
                return shutdownSharedMemory(fd);
            }

            // Loading the settings
            this->_consoleLogLevel = (NRPLogger::level_t)cfg->consoleLogLevel;
            this->_fileLogLevel = (NRPLogger::level_t)cfg->fileLogLevel;
            this->_logDir = std::string(cfg->logDir, cfg->logDir + cfg->logDirLen);

            // Sending signal that reading is finished
            // Return true, because the configuration is read.
            error = sem_post(&cfg->sem1);
            if (error == -1){
                shutdownSharedMemory(fd);
                return true;
            }
        }
    }
    else {
        return shutdownSharedMemory(fd);
    }

    close(fd);
    return true;
}


NRPLogger::NRPLogger(
    std::string loggerName)
    : NRPLogger(
        loggerName, 
        NRPLogger::level_t::off, 
        NRPLogger::level_t::info, 
        _defaultLogDir.data(), 
        false)
{
}

NRPLogger::NRPLogger(
    std::string loggerName,
    NRPLogger::level_t fileLogLevel,
    NRPLogger::level_t consoleLogLevel,
    std::string logDir,
    bool doSavePars)
    : _baseFilename("")
    , _loggerName(loggerName)
    , _fileLogLevel(fileLogLevel)
    , _consoleLogLevel(consoleLogLevel)
    , _logDir(logDir)
    , _consoleOut(stdout)
    , _doSavePars(doSavePars)
{
    // Flag of communication with ShM
    bool sharedMemOperation = false;
    try
    {
        if (_doSavePars)
        {
            // If _doSavePars, try to save settings to ShM
            sharedMemOperation = this->setSharedMemoryForLauncher();
        }
        else
        {
            // If _doSavePars, try to save settings to ShM
            sharedMemOperation = this->getSharedMemoryFromLauncher();
        }
    }
    catch (...)
    {
        sharedMemOperation = false;
    }

    this->registerDefaultLogger();

    if (sharedMemOperation)
    {
        if (_doSavePars)
        {
            debug("The global logger settings were saved");
        }
        else
        {
            debug("The logger is configured from the global settings");
        }
    }
    else
    {
        if (_doSavePars)
        {
            debug("Couldn't save the global logger settings as launcher");
        }
        else
        {
            debug("Couldn't obtain the global logger settings, using provided data");
            this->_loggerName = loggerName;
            this->_fileLogLevel = fileLogLevel;
            this->_consoleLogLevel = consoleLogLevel;
            this->_logDir = logDir;
        }
    }
}

void NRPLogger::registerDefaultLogger()
{
    // Append current date and time to filename
    try
    {
        const auto t = std::time(nullptr);
        const auto tm = *std::localtime(&t);

        std::ostringstream oss;
        oss << std::put_time(&tm, "-%Y%m%d-%H%M%S-") << getpid() << ".log";

        _baseFilename = oss.str();
    }
    catch(std::exception &e)
    {
        const auto errMsg = std::string("Failed to get current time: ") + e.what();
        fprintf(_consoleOut, "%s\n", errMsg.data());
        throw std::runtime_error(errMsg);
    }

    // if logger name is empty, then assign the default logger name
    if (_loggerName.empty()){
        _loggerName = this->_defaultLoggerName.data();
    }

    // exit if logger already exists
    if (spdlog::get(_loggerName) != nullptr){
        warn("Logger with name {} already exists, using it as default", _loggerName);
        spdlog::set_default_logger(spdlog::get(_loggerName));
        return;
    }

    // Log filename
    std::string fileName = _logDir + "/" + _loggerName + _baseFilename;

    // Create log sinks for file and console
    std::vector<spdlog::sink_ptr> sinks;
    try
    {
        sinks.emplace_back(new spdlog::sinks::ansicolor_sink<spdlog::details::console_nullmutex>(_consoleOut, spdlog::color_mode::automatic));
        sinks.back()->set_level(_consoleLogLevel);
        if (_fileLogLevel != spdlog::level::off)
        {
            sinks.emplace_back(new spdlog::sinks::basic_file_sink_st(fileName));
            sinks.back()->set_level(_fileLogLevel);
        }
    }
    catch(std::exception &e)
    {
        const auto errMsg = std::string("Failed to create spdlogger sinks: ") + e.what();
        fprintf(_consoleOut, "%s\n", errMsg.data());
        throw std::runtime_error(errMsg);
    }

    // Create logger
    try
    {
        std::shared_ptr<spdlog::logger> nrpLoggerPtr(new spdlog::logger(_loggerName, sinks.begin(), sinks.end()));
        // The general log level is a minimun of two levels
        nrpLoggerPtr->set_level((_consoleLogLevel > _fileLogLevel) ? _fileLogLevel : _consoleLogLevel);
        // flush to file if info level appears
        nrpLoggerPtr->flush_on(spdlog::level::info);

        // Set new logger as default
        spdlog::register_logger(nrpLoggerPtr);
        spdlog::set_default_logger(nrpLoggerPtr);
    }
    catch(std::exception &e)
    {
        const auto errMsg = std::string("Failed to create and register spdlogger: ") + e.what();
        fprintf(_consoleOut, "%s\n", errMsg.data());
        throw std::runtime_error(errMsg);
    }
    
    // If log goes to the file, add corresponding notification as well
    if (_fileLogLevel != spdlog::level::off) 
    {
        debug("Starting logging to file: {}", fileName);
    }

    // Notify user that the loggin has started and show the log severity
    debug(
        "The [{}] logger registered as process default, console:{}, file:{}, dir:{}", 
        _loggerName, 
        spdlog::level::to_string_view(_consoleLogLevel), 
        spdlog::level::to_string_view(_fileLogLevel), 
        _logDir);
}

NRPLogger::~NRPLogger()
{
    try
    {
        shm_unlink(_sharedMemCfgName.data());
    }
    catch (...)
    {
        debug("Couldn't unlink the shared memory object");
    }

    debug("Shutting down logger...");
    NRPLogger::shutdownDefault();
}

void NRPLogger::flush(){
    spdlog::get(_loggerName)->flush();
}

void NRPLogger::shutdownDefault()
{
    spdlog::shutdown();
}