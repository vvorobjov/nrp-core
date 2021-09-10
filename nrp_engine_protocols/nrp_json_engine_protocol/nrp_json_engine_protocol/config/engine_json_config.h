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

#ifndef ENGINE_JSON_CONFIG_H
#define ENGINE_JSON_CONFIG_H

#include <string_view>

struct EngineJSONConfigConst
{
		/*!
		 * \brief Maximum amount of tries for EngineJSONserver to bind to different ports
		 */
		static constexpr short MaxAddrBindTries = 1024;

		/*!
		 * \brief Parameter name that is used to pass along the server address
		 */
		static constexpr std::string_view EngineServerAddrArg = "serverurl";

		/*!
		 * \brief Parameter name that is used to pass along the server address
		 */
		static constexpr std::string_view EngineRegistrationServerAddrArg = "regservurl";

		/*!
		 * \brief Parameter name that is used to pass along the engine name
		 */
		static constexpr std::string_view EngineNameArg = "engine";

		/*!
		 * \brief REST Server Route from which to get device information
		 */
		static constexpr std::string_view EngineServerGetDevicesRoute = "/get_device_information";

		/*!
		 * \brief REST Server Route to which to send device changes
		 */
		static constexpr std::string_view EngineServerSetDevicesRoute = "/set_device";

		/*!
		 * \brief REST Server Route to execute a single loop
		 */
		static constexpr std::string_view EngineServerRunLoopStepRoute = "/run_loop";

		/*!
		 * \brief REST Server Route for engine initialization
		 */
		static constexpr std::string_view EngineServerInitializeRoute = "/initialize";

		/*!
		 * \brief REST Server Route for engine initialization
		 */
		static constexpr std::string_view EngineServerResetRoute = "/reset";

		/*!
		 * \brief REST Server Route for engine shutdown
		 */
		static constexpr std::string_view EngineServerShutdownRoute = "/shutdown";

		/*!
		 * \brief JSON name under which the runLoopStep timeStep is saved
		 */
		static constexpr std::string_view EngineTimeStepName = "time_step";

		/*!
		 * \brief JSON name under which the runLoopStep engine time is sent
		 */
		static constexpr std::string_view EngineTimeName = "time";

		/*!
		 * \brief Content Type passed between server and client
		 */
		static constexpr std::string_view EngineServerContentType = "application/json";
};

#endif // ENGINE_JSON_CONFIG_H
