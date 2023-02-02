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

#ifndef TRANSCEIVER_DATAPACK_INTERFACE_H
#define TRANSCEIVER_DATAPACK_INTERFACE_H

#include "nrp_general_library/engine_interfaces/engine_client_interface.h"
#include "nrp_general_library/utils/ptr_templates.h"

//#include "nrp_general_library/transceiver_function/transceiver_function_interpreter.h"

#include <boost/python.hpp>

class FunctionManager;
using TransceiverFunctionInterpreterSharedPtr = std::shared_ptr<FunctionManager>;
using TransceiverFunctionInterpreterConstSharedPtr = std::shared_ptr<const FunctionManager>;


/*!
 * \brief Base of TF Decorators.
 */
class TransceiverDataPackInterface
        : public PtrTemplates<TransceiverDataPackInterface>
{
    public:
        TransceiverDataPackInterface() = default;
        virtual ~TransceiverDataPackInterface() = default;

        /*!
         * \brief Decorator __call__() function. Takes the lower decorator as a parameter. Moves the given class into a shared_ptr, which will be managed by the next decorator
         * \param tfDataPack Lower Decorator
         * \return shared_ptr referencing data from this object
         */
        template<class TRANSCEIVER_DATAPACK>
        TransceiverDataPackInterface::shared_ptr pySetup(const TransceiverDataPackInterface::shared_ptr &tfDataPack)
        {
            static_assert((std::is_base_of_v<TransceiverDataPackInterface, TRANSCEIVER_DATAPACK>) || (std::is_same_v<TransceiverDataPackInterface, TRANSCEIVER_DATAPACK>),"Parameter TRANSCEIVER_DATAPACK must derive from TransceiverDataPackInterface or be same");

            this->_nextDecorator = tfDataPack;

            auto thisPtr = this->moveToSharedPtr<TRANSCEIVER_DATAPACK>();

            TransceiverDataPackInterface::shared_ptr *const registryPtr = this->_nextDecorator->getTFInterpreterRegistry();
            assert(registryPtr != nullptr);

            *registryPtr = thisPtr;
            return thisPtr;
        }

        /*!
         * \brief Get name of engine this transceiver is linked to
         */
        virtual const std::string &linkedEngineName() const;

        /*!
         * \brief Indicates if this is a preprocessing function
         */
        virtual bool isPreprocessing() const;

        /*!
         * \brief Execute Transceiver Function. Base class will simply call runTf on _function
         * \param args Arguments for execution. Can be altered by any TransceiverDataPackInterfaces. Base class will only pass them along
         * \param kwargs Keyword arguments for execution. Can be altered by any TransceiverDataPackInterfaces. Base class will only pass them along
         * \return Returns result of TransceiverFunction execution.
         */
        virtual boost::python::object runTf(boost::python::tuple &args, boost::python::dict &kwargs, datapacks_set_t dataPacks);

        /*!
         *  \brief Appends its own datapack requests onto datapackIDs. Uses getRequestedDataPackIDs to check which IDs are requested by this datapack
         *  \param datapackIDs Container with datapack IDs that gets expanded
         *  \return Returns datapackIDs, with own datapackIDs appended
         */
        virtual datapack_identifiers_set_t updateRequestedDataPackIDs(datapack_identifiers_set_t &&datapackIDs = datapack_identifiers_set_t()) const;

        /*!
         * \brief Returns datapack IDs of this DataPack that should be requested from the engines. TODO: Make protected
         */
        virtual datapack_identifiers_set_t getRequestedDataPackIDs() const;

        /*!
         * \brief Set global TF Interpreter. All Transceiver Functions will register themselves with it upon creation
         * \param interpreter Interpreter to use
         */
        static void setTFInterpreter(FunctionManager *interpreter);
        static const FunctionManager * getFunctionManager()
        {
            return TransceiverDataPackInterface::_functionManager;
        }

    protected:
        /*!
         * \brief Pointer to TF Interpreter. Will be used to register a new TF function
         */
        static FunctionManager *_functionManager;

        template<class TRANSCEIVER_DATAPACK>
        typename PtrTemplates<TRANSCEIVER_DATAPACK>::shared_ptr moveToSharedPtr()
        {
            static_assert((std::is_base_of_v<TransceiverDataPackInterface, TRANSCEIVER_DATAPACK>) || (std::is_same_v<TransceiverDataPackInterface, TRANSCEIVER_DATAPACK>),"Parameter TRANSCEIVER_DATAPACK must derive from TransceiverDataPackInterface or be same");
            return typename PtrTemplates<TRANSCEIVER_DATAPACK>::shared_ptr(new TRANSCEIVER_DATAPACK(std::move(static_cast<TRANSCEIVER_DATAPACK&>(*this))));
        }

        /*!
         * \brief Gets address of ptr under which the _functionManager has registered this TF. Mainly used during setup
         * \return Returns address of ptr under which the _functionManager has registered this TF
         */
        virtual TransceiverDataPackInterface::shared_ptr *getTFInterpreterRegistry();

    private:
        /*!
         * \brief Function to execute
         */
        TransceiverDataPackInterface::shared_ptr _nextDecorator;
};

using TransceiverDataPackInterfaceSharedPtr = TransceiverDataPackInterface::shared_ptr;
using TransceiverDataPackInterfaceConstSharedPtr = TransceiverDataPackInterface::const_shared_ptr;

#endif
