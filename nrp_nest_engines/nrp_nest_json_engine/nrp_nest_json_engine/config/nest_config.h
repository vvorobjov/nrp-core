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

#ifndef NEST_CONFIG_H
#define NEST_CONFIG_H

#include <string_view>
#include "nrp_general_library/utils/fixed_string.h"
#include "nrp_nest_json_engine/config/cmake_constants.h"

struct NestConfigConst
{
    static constexpr char EngineType[] = "nest_json";
    static constexpr char EngineSchema[] = "https://neurorobotics.net/engines/engines_nest.json#/engine_nest_json";

    /*!
     * \brief Python Path to Nest. Automatically generated via cmake on installation
     */
    static constexpr std::string_view NestPythonPath = "PYTHONPATH=" NRP_PYNEST_PATH ":$PYTHONPATH";

    /*!
     * \brief Path to Nest Server Executable. Automatically generated via cmake on installation
     */
    static constexpr std::string_view NestExecutablePath = "PATH=$PATH:" NRP_NEST_BIN_PATH;

    /*!
     * \brief Argument to pass RNG seed argument to Nest
     */
    static constexpr std::string_view NestRNGSeedArg = "--nrprng";

    /*!
     * \brief After the server executes the init file, this status flag will either be 1 for success or 0 for fail. If the execution fails, a JSON message with more details will be passed as well (under ErrorMsg).
     */
    static constexpr std::string_view InitFileExecStatus = "InitExecStatus";

    /*!
     * \brief After the server executes the init file, this status flag will either be 1 for success or 0 for fail. If the execution fails, a JSON message with more details will be passed as well (under ErrorMsg).
     */
    static constexpr std::string_view ResetExecStatus = "ResetStatus";

    /*!
     * \brief After the server executes the init file, the parsed devMap will be passed back with this param
     */
    static constexpr std::string_view InitFileParseDevMap = "InitFileParseDevMap";

    /*!
     * \brief If the init file could not be parsed, the python error message will be stored under this JSON property name
     */
    static constexpr std::string_view ErrorMsg = "Message";
};

#endif // NEST_CONFIG_H
