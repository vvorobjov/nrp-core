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

#ifndef DATA_DATAPACK_CONTROLLER_H
#define DATA_DATAPACK_CONTROLLER_H

/*!
 *  \brief Helper class to handle DataPacks on the Engine Server side
 *
 *  As a general policy DataDataPackController is not supposed to take ownership of DATA_TYPE objects which receives or
 *  returns. This must be consider for each DataDataPackController specialization.
 *
 *  \tparam DATA_TYPE Object type that this controller handles
 */
template< class DATA_TYPE>
class DataPackController
{
public:

    /*!
     * \brief Constructor
     */
    DataPackController() = default;

    virtual ~DataPackController() = default;

    /*!
     * \brief Get datapack information to be forwarded to the NRP
     * \return Returns a DATA_TYPE pointer containing requested data
     */
    virtual DATA_TYPE* getDataPackInformation() = 0;

    /*!
     * \brief Handle received datapack data
     * \param data Data to be processed
     */
    virtual void handleDataPackData(const DATA_TYPE &data) = 0;
};

#endif // DATA_DATAPACK_CONTROLLER_H
