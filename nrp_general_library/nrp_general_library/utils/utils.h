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
#include <boost/python.hpp>
#include <boost/asio.hpp>

/*!
 * \brief Attempts to bind to a given address
 * \param hostIpv4 IP4 address of the host
 * \param port Port to connect to, if 0 a free port is searched and used
 * \return Port used in the connection, same as "port" argument or found free port
 *
 * When "port" is 0, the function asks the OS to look for a free port number. Throws an exception
 * if the connection is not successful
 */
inline int bindOrFindFreePort(std::string hostIpv4, int port = 0)
{
    using namespace boost::asio;

    ip::address addressStruct;

    if(hostIpv4 == "localhost")
    {
        addressStruct = ip::address_v4::loopback();
    }
    else
    {
        addressStruct = ip::address::from_string(hostIpv4);
    }

    io_service service;
    ip::tcp::socket socket(service);
    socket.open(ip::tcp::v4());
    socket.bind(ip::tcp::endpoint(addressStruct, port));

    int newPort = socket.local_endpoint().port();

    // Close the socket, so that the port can be used by the caller
    socket.close();

    return newPort;
}

/*!
 * \brief Attempts to bind to a given address
 * \param hostIpv4 IP4 address of the host
 * \param port Port to connect to
 *
 * Throws an exception if the connection is not successful
 */
inline void bindToAddress(std::string hostIpv4, int port)
{ bindOrFindFreePort(hostIpv4, port); }

/*!
 * \brief Returns a free port number
 * \param hostIpv4 IP4 address of the host
 * \return Free port number
 *
 * Asks the OS to look for a free port number
 */
inline int getFreePort(std::string hostIpv4)
{ return bindOrFindFreePort(hostIpv4); }

/*!
 * \brief Appends 'path' to PYTHON_PATH env variable
 */
inline void appendPythonPath(const std::string &path)
{
    boost::python::handle pathH(boost::python::borrowed(PySys_GetObject("path")));
    boost::python::list paths(pathH);
    paths.append(path);

    PySys_SetObject("path", paths.ptr());
}

#endif // UTILS_H
