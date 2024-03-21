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

#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <chrono>
#include <map>
#include <fstream>
#include <filesystem>
#include <unistd.h>

using SimulationTime = std::chrono::nanoseconds;

/*!
* \brief Converts given value to SimulationTime object
*
* The function is parametrized with two parameters: vartype and ratio.
* Vartype is the type of variable that is supposed to be converted to SimulationTime.
* Usually this will be int or float. Ratio is std::ratio class.
*/
template <class vartype, class ratio>
static SimulationTime toSimulationTime(vartype time)
{
    return std::chrono::duration_cast<SimulationTime>(std::chrono::duration<vartype, ratio>(time));
}

/*!
 * \brief Converts floating-point seconds into SimulationTime
 *
 * \param time The value to be converted
 * \return SimulationTime object that corresponds to the argument
 */
SimulationTime toSimulationTimeFromSeconds(double time);

/*!
* \brief Converts SimulationTime object to specified type and with given ratio
*/
template <class vartype, class ratio>
static vartype fromSimulationTime(SimulationTime time)
{
    return std::chrono::duration_cast<std::chrono::duration<vartype, ratio>>(time).count();
}


/*!
 * \brief Calculates simulation run time rounded to milliseconds, accounting for given resolution
 *
 * \param runTime                Simulation run time that will be rounded
 * \param simulationResolutionMs Simulation resolution in milliseconds
 *
 * \throws NRPException When simulation run time is smaller than simulation resolution
 *
 * \return Rounded simulation run time
 */
double getRoundedRunTimeMs(const SimulationTime runTime, const float simulationResolutionMs);

/*!
 * \brief returns the current local time as a string in the format: YYMMDD-hhmmss-pid,
 * being pid the pid of the calling process
 */
std::string getTimestamp();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef TIME_PROFILE

// macro used to generate unique (within a file) anonymous var names
#define _CONCAT_(x,y) x ## y
#define CONCAT(x,y) _CONCAT_(x,y)
#define ANONYMOUS_VAR CONCAT(_anonymous, __LINE__)

/*!
 * \brief macro which sets the clock time timepoints are logged wrt. It is voided by not defining \TIME_PROFILE
 */
#define NRP_LOG_TIME_SET_START TimeProfiler::setStartTime()

/*!
 * \brief macro which logs the current clock time wrt 'start' time in microseconds. It is voided by not defining \TIME_PROFILE
 */
#define NRP_LOG_TIME(filename) TimeProfiler::recordTimePoint(filename)
#define NRP_LOG_TIME_WITH_COMMENT(filename, comment) TimeProfiler::recordTimePoint(filename, comment)

/*!
 * \brief macro which records the time passed between the call and the end of the block. It is voided by not defining \TIME_PROFILE
 */
#define NRP_LOG_TIME_BLOCK(filename) auto ANONYMOUS_VAR = BlockProfiler(filename)
#define NRP_LOG_TIME_BLOCK_WITH_COMMENT(filename, comment) auto ANONYMOUS_VAR = BlockProfiler(filename, comment)

/*!
 * \brief Struct containing time profile logs and methods
 */
struct TimeProfiler {

    static std::map<std::string,  std::ofstream> files;
    static std::chrono::time_point<std::chrono::high_resolution_clock> start;
    static std::string timeLogsDir;

    /*!
     * \brief set the clock start which timepoints are logged wrt
     */
    static void setStartTime();

    /*!
     * \brief Records the current clock time wrt 'start' time in microseconds
     *
     * \param filename name of the file to which the record will be added
     * \param comment text  to be prepended to the time record
     * \param newLine if true a new line is added, if false a space is added after the record
     */
    static void recordTimePoint(const std::string& filename, const std::string& comment = "", bool newLine = true);

    /*!
     * \brief add a record with the specified duration
     *
     * \param filename name of the file to which the record will be added
     * \param comment text to be prepended to the time record
     * \param newLine if true a new line is added, if false a space is added after the record
     */
    static void recordDuration(const std::string& filename, const std::chrono::microseconds& duration, const std::string& comment = "", bool newLine = true);
};

/*!
 * \brief Records the time passed between the creation and the destruction of the object
 */
struct BlockProfiler {
    BlockProfiler() = delete;

    /*!
     * \brief Constructor
     *
     * \param filename name of the file to which the record will be added
     * \param comment text to be prepended to the time record
     */
    BlockProfiler(const std::string& filename, const std::string& comment="");

    ~BlockProfiler();

    std::chrono::time_point<std::chrono::high_resolution_clock> _start;
    std::string _filename;
    std::string _comment;
};

#else
#define NRP_LOG_TIME_SET_START
#define NRP_LOG_TIME(filename)
#define NRP_LOG_TIME_WITH_COMMENT(filename, comment)
#define NRP_LOG_TIME_BLOCK(filename)
#define NRP_LOG_TIME_BLOCK_WITH_COMMENT(filename, comment)
#endif

#endif // TIME_UTILS_H

// EOF
