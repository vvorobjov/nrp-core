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

#include "nrp_event_loop/event_loop/event_loop_engine_opts_parser.h"
#include "nrp_general_library/utils/nrp_logger.h"

cxxopts::Options ELEOptsParser::createOptionParser(bool allowUnrecognised)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);

    cxxopts::Options options("NRP Gazebo Server");

    if(allowUnrecognised)
        options.allow_unrecognised_options();

    options.add_options()
            ("c,config", "Simulation config file", cxxopts::value<std::string>())
            ("loglevel", "Console Log level",
             cxxopts::value<std::string>()->default_value("info"));

    return options;
}

cxxopts::ParseResult ELEOptsParser::parseOpts(int argc, char *argv[], cxxopts::Options parser)
{
    NRP_LOGGER_TRACE("{} called", __FUNCTION__);
    
    return parser.parse(argc, argv);
}
