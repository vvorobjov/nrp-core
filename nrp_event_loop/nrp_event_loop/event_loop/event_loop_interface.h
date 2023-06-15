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

#ifndef EVENT_LOOP_INTERFACE_H
#define EVENT_LOOP_INTERFACE_H

#include <chrono>
#include <thread>
#include <future>

#include <nlohmann/json.hpp>

/*!
 * \brief Manages simulation loop. Runs physics and brain interface, and synchronizes them via Transfer Functions
 */
class EventLoopInterface
{
    public:

        virtual ~EventLoopInterface() = default;

        EventLoopInterface() = delete;

        /*!
         * \brief Constructor
         */
        EventLoopInterface(std::chrono::milliseconds timestep, std::chrono::milliseconds timestepThres);

        /*!
         * \brief Initialize loop
         */
        virtual void initialize();

        /*!
         * \brief Run a single loop
         */
        void runLoopOnce(const std::chrono::time_point<std::chrono::steady_clock>& startTime);

        /*!
         * \brief Run loop
        */
        void runLoop(std::chrono::milliseconds timeout);

        /*!
         * \brief Run loop in a thread
         *
         * \param timeout time in seconds the loop will be run for
         * \param doInit if true, Initialize is executed before runLoop, also in the same thread.
         *               This is useful in cases where initialize needs to interact with the main thread
         */
        void runLoopAsync(std::chrono::milliseconds timeout = std::chrono::milliseconds(0), bool doInit = false);

        /*!
         * \brief Stop loop
         *
         * It is intended to be used together with runLoopAsync
         */
        void stopLoop();

        /*!
         * \brief Shutdown loop
         */
        void shutdown();

        /*!
         * \brief Returns true if the event loop is currently running, false otherwise
         *
         * It is intended to be used together with runLoopAsync
         */
        bool isRunning();

        /*!
         * \brief Blocks execution until the loop reaches timeout
         *
         * It is intended to be used together with runLoopAsync
         */
        void waitForLoopEnd();

    protected:

        /*!
         * \brief Initialize loop
         */
        virtual void initializeCB() = 0;

        /*!
         * \brief Execute actions which must be performed every loop
         */
        virtual void runLoopCB() = 0;

        /*!
         * \brief Shutdown loop
         */
        virtual void shutdownCB() = 0;

        /*! \brief timestep of the event loop  */
        std::chrono::milliseconds _timestep;
        /*! \brief allowed time deviation in event loop timestep execution before printing a warning message */
        std::chrono::milliseconds _timestepThres;

        /*!
         * \brief Internal isRunning function
         *
         * It can be used from threads, even when runLoopAsync was not used
         */
        bool isRunningNotAsync();

    private:

        /*! \brief future state of the event loop thread run async  */
        std::future<void> _runFuture;
        /*! \brief boolean variable used to step the event loop from parent thread */
        std::atomic<bool> _doRun;
        /*! \brief flag telling if the event loop has been initialized */
        bool _isInitialized = false;
};


#endif // EVENT_LOOP_INTERFACE_H
