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

// This file contains helper functions that are not classified in any of the more specific util files

#ifndef UTILS_H
#define UTILS_H

#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <stdexcept>
#include <unistd.h>


/*!
 * \brief Searchs for an unbound port starting from startPort. Returns the first unbound port found as a uint16_t
 */
uint16_t findUnboundPort(uint16_t startPort)
{
    uint16_t sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
        throw std::runtime_error(std::string("Socket Error: ") + strerror(errno));

    struct sockaddr_in serv_addr;
    do
    {
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(startPort);
        if(bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) >= 0)
            break;

        if(errno != EADDRINUSE)
            throw std::runtime_error("Failed to bind port " + std::to_string(startPort) + ": " + strerror(errno));

        startPort += 1;
    }
    while(true);

    if(close(sock) < 0)
        throw std::runtime_error("Failed to close socket at port " + std::to_string(startPort) + ": " + strerror(errno));

    return startPort++;
}

#endif UTILS_H
