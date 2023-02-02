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

#ifndef STATUS_FUNCTION_H
#define STATUS_FUNCTION_H

#include "nrp_general_library/transceiver_function/transceiver_datapack_interface.h"

#include "nrp_general_library/transceiver_function/function_manager.h"
#include "nrp_general_library/engine_interfaces/engine_client_interface.h"

#include <string>
#include <boost/python.hpp>

/*!
 * \brief Holds a single Python function created with StatusFunction decorator
 */
class StatusFunction
        : public TransceiverDataPackInterface,
          public PtrTemplates<StatusFunction>
{
    public:
        /*!
         * \brief Constructs a status function object
         *
         * Status functions live on the boundary between C++ and Python worlds.
         * The constructor only allocates the memory needed by the objects,
         * but they are truly initialized only when a proper Python decorator is called from the Python context.
         * The decorators are linked to the pySetup method, which performs the true initialization.
         */
        StatusFunction();
        virtual ~StatusFunction() override = default;

        /*!
         * \brief Returns an empty string
         *
         * Status functions are not linked to any engine, so the string returned by this method
         * should always be empty. The method is implemented in order to conform to the base interface.
         */
        const std::string &linkedEngineName() const override final;

        /*!
         * \brief Returns false
         *
         * Status functions don't have the concept of pre-processing.
         * The method is implemented in order to conform to the base interface.
         */
        bool isPreprocessing() const override final;

        /*!
         * \brief Performs actual initialization of the object.
         * \param statusFunction User-defined python function. It will be linked to this C++ object.
         * \return shared_ptr referencing data from this object
         *
         * This method is linked to the __call__() method of Python StatusFunction decorator.
         * It links the user-defined python function to this C++ object.
         * It also registers the C++ object in the function manager.
         */
        TransceiverDataPackInterface::shared_ptr pySetup(boost::python::object statusFunction);

        /*!
         * \brief Runs the status function
         * \param args Python args
         * \param kwargs Python keywords
         * \return Result of status function execution
         */
        boost::python::object runTf(boost::python::tuple &args, boost::python::dict &kwargs, datapacks_set_t dataPacks) override;

    protected:
        datapack_identifiers_set_t getRequestedDataPackIDs() const override;

        datapack_identifiers_set_t updateRequestedDataPackIDs(datapack_identifiers_set_t &&datapackIDs) const override;

    private:

        std::string name = "";
        /*!
         * \brief Python function linked to the C++ object
         */
        boost::python::object _function;

        /*!
         * \brief Pointer to location where TFInterperter has stored this status function
         */
        TransceiverDataPackInterface::shared_ptr *_tfInterpreterRegistryPtr = nullptr;

        /*!
         * \brief Gets pointer to location where this status function is stored
         * \return Returns _tfInterpreterRegistryPtr
         */
        TransceiverDataPackInterface::shared_ptr *getTFInterpreterRegistry() override final;
};

#endif // STATUS_FUNCTION_H

// EOF
