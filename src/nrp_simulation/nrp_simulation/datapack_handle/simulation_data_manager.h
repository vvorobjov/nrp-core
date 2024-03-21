/*
 * NRP Core - Backend infrastructure to synchronize simulations
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

#ifndef SIMULATION_DATA_MANAGER_H
#define SIMULATION_DATA_MANAGER_H


#include "nrp_general_library/transceiver_function/function_manager.h"
#include "nrp_protobuf/nrp_server.grpc.pb.h"


/*!
 * \brief Manages all simulation data
 *
 * This class is responsible for managing all simulation data. In particular,
 * it manages all DataPacks that are produced during the course of the simulation.
 *
 * The DataPacks are stored in five separate pools:
 * - Engine Pool for DataPacks coming from the Engines
 * - Preprocessing Pool for DataPacks coming from the Preprocessing Functions
 * - Transceiver Pool for DataPacks coming from the Transceiver Functions
 * - External Pool for DataPacks coming from external processes, most likely a Master Script
 * - Trajectory for DataPacks coming from the Status Function
 *
 * The first four pools are meant to store the most recent simulation data coming from various sources.
 * They are implemented as sets, so he DataPacks stored in these pools are overwritten
 * (updated) whenever a new DataPack with matching ID is inserted.
 *
 * The Trajectory is used to store a history of observations produced by the Status Function.
 * It is implemented using a vector, so the order of DataPacks is preserved.
 * The DataPacks from this pool is intended to be sent back to the Master Script whenever
 * the done flag coming from the Status Function is set.
 */
class SimulationDataManager
{

public:

    /*!
     * \brief Appends the input vector to the trajectory
     */
    void pushToTrajectory(datapacks_vector_t dataPacks);

    /*!
     * \brief Returns the trajectory vector
     */
    const datapacks_vector_t & getTrajectory() const;

    /*!
     * \brief Clears the trajectory vector
     */
    void clearTrajectory();

    /*!
     * \brief Sets value of the 'done' flag
     */
    void setDoneFlag(bool doneFlag);

    /*!
     * \brief Returns value of the 'done' flag
     */
    bool getDoneFlag() const;

    /*!
     * \brief Clears all data stored in the manager
     */
    void clear();

    /*!
     * \brief Updates the pool of DataPacks coming from an external source (e.g. a master script)
     *
     * The method will overwrite DataPacks that are already in the pool, if their ID matches
     * the ID of any of the DataPacks passed as argument.
     *
     * \param dataPacks A list of DataPacks that will be inserted into the External pool
     */
    void updateExternalPool(datapacks_vector_t dataPacks);

    /*!
     * \brief Updates the pool of DataPacks coming from the Preprocessing Functions
     *
     * The method will overwrite DataPacks that are already in the pool, if their ID matches
     * the ID of any of the DataPacks passed as argument.
     *
     * \param dataPacks A list of DataPacks that will be inserted into the Preprocessing pool
     */
    void updatePreprocessingPool(datapacks_vector_t dataPacks);

    /*!
     * \brief Updates the pool of DataPacks coming from the Transceiver Functions
     *
     * The method will overwrite DataPacks that are already in the pool, if their ID matches
     * the ID of any of the DataPacks passed as argument.
     *
     * \param dataPacks A list of DataPacks that will be inserted into the Transceiver pool
     */
    void updateTransceiverPool(datapacks_vector_t dataPacks);

    /*!
     * \brief Updates the pool of DataPacks coming from Engines
     *
     * The method will overwrite DataPacks that are already in the pool, if their ID matches
     * the ID of any of the DataPacks passed as argument, and if the new DataPacks are not empty.
     *
     * \param dataPacks A list of DataPacks that will be inserted into the Engine pool
     */
    void updateEnginePool(datapacks_vector_t dataPacks);

    /*!
     * \brief Returns a set of DataPacks that are intended to be sent to the Engine with given name
     *
     * The method combines DataPacks from the Transceiver and External pools, and filters them by
     * the Engine name given as argument.
     */
    datapacks_set_t getEngineDataPacks(const std::string & engineName) const;

    /*!
     * \brief Returns a set of DataPacks that are intended to be accessed by the Transceiver Functions
     *
     * The method combines DataPacks from the Engine and Preprocessing pools.
     */
    datapacks_set_t getTransceiverDataPacks() const;

    /*!
     * \brief Returns a set of DataPacks that are intended to be accessed by the Preprocessing Functions
     *
     * The method returns DataPacks from the Engine pool.
     */
    datapacks_set_t getPreprocessingDataPacks() const;

    /*!
     * \brief Returns a set of DataPacks that are intended to be accessed by the Status Function
     *
     * The method combines DataPacks from the Engine, Preprocessing and Transceiver pools.
     */
    datapacks_set_t getStatusDataPacks() const;

    /*!
     * \brief Performs bookkeeping at start of the simulation iteration
     *
     * This method should be called at the start of each simulation iteration.
     * It resets the isUpdated flags on all stored DataPacks.
     */
    void startNewIteration();

private:

    bool _doneFlag = false;

    /*!
     * \brief DataPacks produced by the Preprocessing Functions
     */
    datapacks_set_t _preprocessingPool;

    /*!
     * \brief DataPacks produced by the Transceiver Functions
     */
    datapacks_set_t _transceiverPool;

    /*!
     * \brief DataPacks produced by the Engines
     */
    datapacks_set_t _enginePool;

    /*!
     * \brief DataPacks produced by the Status Function
     */
    datapacks_vector_t _trajectory;

    /*!
     * \brief DataPacks produced by external processes, for example a Master Script
     */
    datapacks_set_t _externalPool;
};


#endif // SIMULATION_DATA_MANAGER_H

// EOF
