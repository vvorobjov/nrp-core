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

#ifndef TRANSCEIVER_FUNCTION_H
#define TRANSCEIVER_FUNCTION_H

#include "nrp_general_library/transceiver_function/transceiver_datapack_interface.h"

#include "nrp_general_library/transceiver_function/function_manager.h"
#include "nrp_general_library/engine_interfaces/engine_client_interface.h"

#include <string>
#include <boost/python.hpp>

/*!
 * \brief Holds a single transfer function decorator
 */
class TransceiverFunction
        : public TransceiverDataPackInterface,
          public PtrTemplates<TransceiverFunction>
{
    public:
        /*!
         * \brief Constructor
         * \param linkedEngine Name of linked engine.
         * This TF will only be called whenever Said engine finishes a timestep
         */
        TransceiverFunction(std::string linkedEngine, bool isPreprocessing);
        virtual ~TransceiverFunction() override = default;

        const std::string &linkedEngineName() const override;

        bool isPreprocessing() const override;

        /*!
         * \brief Decorator __call__() function. Takes the Transfer Function as a parameter. Moves this class into a shared_ptr.
         * \param transceiverFunction User defined TF
         * \return shared_ptr referencing data from this object
         */
        TransceiverDataPackInterface::shared_ptr pySetup(boost::python::object transceiverFunction);

        /*!
         * \brief Execute the transfer function
         * \param args Python args
         * \param kwargs Python keywords
         * \return Returns result of TF
         */
        boost::python::object runTf(boost::python::tuple &args, boost::python::dict &kwargs, datapacks_set_t dataPacks) override;

    protected:
        datapack_identifiers_set_t getRequestedDataPackIDs() const override;

        datapack_identifiers_set_t updateRequestedDataPackIDs(datapack_identifiers_set_t &&datapackIDs) const override;

    private:
        /*!
         * \brief Transfer function that should be executed
         */
        boost::python::object _function;

        /*!
         * \brief Name of linked Engine
         */
        std::string _linkedEngine;

        /*!
         * \brief Indicates if this is a preprocessing function
         */
        bool _isPreprocessing;

        /*!
         * \brief Pointer to location where TFInterperter has stored this TF
         */
        TransceiverDataPackInterface::shared_ptr *_tfInterpreterRegistryPtr = nullptr;

        /*!
         * \brief Gets pointer to location where this TF is stored
         * \return Returns _tfInterpreterRegistryPtr
         */
        TransceiverDataPackInterface::shared_ptr *getTFInterpreterRegistry() override final;

        /*!
         * \brief Checks that the output of executing this TF is correct. If not raise NRPException
         */
        void checkTFOutputIsCorrectOrRaise(const boost::python::object &tfOutput) const;
};

#endif // TRANSCEIVER_FUNCTION_H
