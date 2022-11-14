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

#ifndef PYTHON_CONFIG_H
#define PYTHON_CONFIG_H

#include <string_view>

#include "nrp_general_library/utils/fixed_string.h"

struct PythonConfigConst
{
    static constexpr char EngineType[] = "python_json";
    static constexpr char EngineSchema[] = "json://nrp-core/engines/engine_python.json#/python_json";

    /*!
     * \brief After the server executes the init file, this status flag will either be 1 for success or 0 for fail. If the execution fails, a JSON message with more details will be passed as well (under ErrorMsg).
     */
    static constexpr std::string_view InitFileExecStatus = "InitExecStatus";

    /*!
     * \brief After the server resets, this status flag will either be 1 for success or 0 for fail. If the execution fails, a JSON message with more details will be passed as well (under ErrorMsg).
     */
    static constexpr std::string_view ResetExecStatus = "ResetExecStatus";

    /*!
     * \brief If the init file could not be parsed, the python error message will be stored under this JSON property name
     */
    static constexpr std::string_view ErrorMsg = "Message";

    /*!
    * \brief Parameter name that is used to pass along the server address
    */
    static constexpr std::string_view ExtraServerOptionsArg = "options";

    /*!
     * \brief Ratio used by SimulationTime
     *
     * The ratio should be passed to the server on initialization.
     * In that way we can communicate to the server what time units are used in the simulation.
     * This will impact the time step passed with runLoop command, as well as the simulation
     * time returned.
     */
    static constexpr std::string_view SimulationTimeRatio = "TimeRatio";
};

#endif // PYTHON_CONFIG_H
