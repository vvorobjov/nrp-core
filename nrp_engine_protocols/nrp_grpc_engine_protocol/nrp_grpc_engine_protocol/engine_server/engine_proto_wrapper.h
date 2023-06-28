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

#ifndef ENGINE_PROTO_WRAPPER_H
#define ENGINE_PROTO_WRAPPER_H

#include <string>
#include <map>
#include <type_traits>

#include <nlohmann/json.hpp>

#include "nrp_protobuf/engine_grpc.pb.h"
#include "nrp_general_library/engine_interfaces/datapack_controller.h"
#include "nrp_general_library/utils/time_utils.h"
#include "nrp_protobuf/config/cmake_constants.h"
#include "nrp_protobuf/proto_python_bindings/proto_field_ops.h"
#include "nrp_protobuf/proto_ops/protobuf_ops.h"
#include "nrp_protobuf/proto_ops/proto_ops_manager.h"


using ProtoDataPackController = DataPackController<google::protobuf::Message>;

/*!
 * \brief Abstract class defining an interface to interact with an Engine with data exchange via protobuf messages
 *
 * Derived classes are responsible
 * for implementing simulation initialization, shutdown and run step methods.
 */
class EngineProtoWrapper
{
    public:

        using mutex_t = std::timed_mutex;
        using lock_t  = std::unique_lock<EngineProtoWrapper::mutex_t>;

        /*!
         * \brief No dummy wrappers, only those with name
         */
        EngineProtoWrapper() = delete;

        /*!
         * \brief Constructor
         *
         * \param[in] engineName    Name of the simulation engine
         */
        EngineProtoWrapper(const std::string &engineName,
                         const std::string &protobufPluginsPath,
                         const nlohmann::json &protobufPlugins)
                : _loggerCfg(engineName), _engineName(engineName)
        {
            NRPLogger::info("EngineProtoWrapper {} has been created", engineName);

            ProtoOpsManager::getInstance().addPluginPath(protobufPluginsPath);
            for (const auto &packageName: protobufPlugins) {
                auto packageNameStr = packageName.template get<std::string>();
                _protoOpsStr += packageNameStr + ",";
                std::stringstream pluginLibName;
                pluginLibName << "lib" << NRP_PROTO_OPS_LIB_PREFIX << packageNameStr <<
                              NRP_PROTO_OPS_LIB_SUFIX << ".so";
                auto pluginLib = ProtoOpsManager::getInstance().loadProtobufPlugin(pluginLibName.str());
                if(pluginLib)
                    _protoOps.template emplace_back(std::move(pluginLib));
                else
                    throw NRPException::logCreate("Failed to load ProtobufPackage \""+packageNameStr+"\"");
            }

            if(!_protoOpsStr.empty())
                _protoOpsStr.pop_back();
        }

        /*!
         * \brief Destructor
         */
        virtual ~EngineProtoWrapper() = default;

        /*!
         * \brief Registers a datapack controller with the given name in the engine
         *
         * \param[in] datapackName       Name of the datapack to be registered
         * \param[in] datapackController Pointer to the datapack controller object that's supposed to be
         *                             registered in the engine
         */
        void registerDataPack(const std::string & datapackName, ProtoDataPackController *interface)
        {
            this->_datapacksControllers.emplace(datapackName, interface);
        }

        // TODO used only in tests, try to remove it?
        unsigned getNumRegisteredDataPacks()
        {
            return this->_datapacksControllers.size();
        }

        /*!
         * \brief Get the names of registered datapacks
         */
        std::vector<std::string> getNamesRegisteredDataPacks()
        {
            std::vector<std::string> dpNames;
            for(auto const& dpCons : this->_datapacksControllers)
                dpNames.push_back(dpCons.first);
            return dpNames;
        }

        /*!
         * \brief Get the Engine name
         */
        const std::string &getEngineName() { return _engineName; }


    /*!
     * \brief Indicates if the simulation was initialized and is running
     */
    virtual bool initRunFlag() const  = 0;

    /*!
     * \brief Indicates if shutdown was requested by the client
     */
    virtual bool shutdownFlag() const  = 0;

    /*!
     * \brief Initializes the simulation
     *
     * \param[in] data       Simulation configuration data
     * \param[in] datapackLock ???
     */
    virtual void initialize(const nlohmann::json &data) = 0;

    /*!
     * \brief Resets the simulation
     */
    virtual void reset() = 0;

    /*!
     * \brief Shutdowns the simulation
     */
    virtual void shutdown() = 0;

    /*!
     * \brief Runs a single simulation loop step
     *
     * \param[in] timeStep Time step by which the simulation should be advanced
     *
     * \return Engine time after running the step
     */
    virtual SimulationTime runLoopStep(const SimulationTime timeStep) = 0;

    /*!
     * \brief Let the Engine perform corrective actions to handle deviations with realtime constraints
     *
     * It is only called when the Engine is run async in realtime. It is called after each runLoopStep
     *
     * \param[in] timeDelta Difference between the time performed to execute the last runLoopStep and the expected time
     *                      given the Engine configuration timestep. Positive values means slower than realtime
     */
    virtual void handleRTDelta(const SimulationTime /*timeDelta*/)
    {}

    /*!
    * \brief callback called each step with the value of the realtime clock
    *
    * It is only called when the Engine is run async in realtime. It is called before each runLoopStep
    *
    * \param[in] newClock current value of the realtime clock
    */
    virtual void rtClockUpdate(const SimulationTime /*newClock*/)
    {}

    void setDataPacks(const EngineGrpc::SetDataPacksRequest & data)
    {
        const auto numDataPacks = data.datapacks_size();

        for(int i = 0; i < numDataPacks; i++)
            setDataPack(data.datapacks(i));
    }

    void setDataPack(const EngineGrpc::DataPackMessage & dataPack)
    {
        const auto &name = dataPack.datapackid().datapackname();
        const auto &devInterface = this->_datapacksControllers.find(name);

        if(devInterface != _datapacksControllers.end()) {
            if(_handleDataPackMessage)
                devInterface->second->handleDataPackData(dataPack);
            else {
                auto protoMsg = unpackFromAny(dataPack.data());
                if(protoMsg)
                    devInterface->second->handleDataPackData(*protoMsg);
                else
                    throw NRPException::logCreate("In Engine \"" + this->getEngineName() + "\", unable to deserialize datapack \"" +
                                                  dataPack.datapackid().datapackname() + "\" using any of the NRP-Core Protobuf plugins" +
                                                  "specified in the engine configuration: [" + _protoOpsStr + "]. Ensure that the parameter " +
                                                  "\"ProtobufPackages\" is properly set in the Engine configuration");
            }
        }
        else
        {
            const auto errorMessage = "DataPack " + name + " is not registered in engine " + this->_engineName;
            throw std::invalid_argument(errorMessage);
        }
    }

    std::unique_ptr<gpb::Message> unpackFromAny(const gpb::Any& data)
    {
        for(auto& mod : _protoOps) {
            auto protoMsg = mod->unpackProtoAny(data);
            if(protoMsg)
                return protoMsg;
        }

        return std::unique_ptr<gpb::Message>();
    }

    virtual void getDataPacks(const EngineGrpc::GetDataPacksRequest & request, EngineGrpc::GetDataPacksReply * reply)
    {
        const auto numDataPacks = request.datapackids_size();

        for(int i = 0; i < numDataPacks; i++)
        {
            auto * protoDataPack = reply->add_datapacks();
            getDataPack(request.datapackids(i).datapackname(), protoDataPack);
        }
    }

    bool getDataPack(const std::string& name, EngineGrpc::DataPackMessage* dpMsg)
    {
        dpMsg->mutable_datapackid()->set_datapackname(name);
        dpMsg->mutable_datapackid()->set_enginename(this->_engineName);

        // ask controller to fetch datapack data. nullptr means there is no new data available
        const auto &devInterface = this->_datapacksControllers.find(name);
        gpb::Message* data;
        if(devInterface != _datapacksControllers.end())
            data = devInterface->second->getDataPackInformation();
        else {
            const auto errorMessage = "DataPack " + name + " is not registered in engine " + this->_engineName;
            throw std::invalid_argument(errorMessage);
        }
        // set DataPackMessage data
        if(data) {
            setDataPackMessageData(data, dpMsg);
            return true;
        }
        else
            return false;
    }


    void setDataPackMessageData(gpb::Message* data, EngineGrpc::DataPackMessage* dpMsg)
    {
        bool isSet = false;
        for (auto &mod: _protoOps) {
            try {
                mod->setDataPackMessageData(*data, dpMsg);
                isSet = true;
            }
            catch (NRPException &) {
                // this just means that the module couldn't process the request, try with the next one
            }
        }

        if (!isSet)
            throw NRPException::logCreate("In Engine \"" + this->_engineName +
                                          "\", unable to serialize datapack \"" + dpMsg->datapackid().datapackname() +
                                          "\" using any of the NRP-Core Protobuf plugins specified in the"
                                          " engine configuration: [" + _protoOpsStr +
                                          "]. Ensure that the parameter "
                                          "\"ProtobufPackages\" is properly set in the Engine configuration");
    }

    protected:

        void clearRegisteredDataPacks()
        {
            this->_datapacksControllers.clear();
        }

        /*!
        * \brief Returns the pointer to the DataPackController of the Data Pack with the specified name
        */
        ProtoDataPackController* getDataPackController(const std::string & datapackName)
        {
            return this->_datapacksControllers.find(datapackName)->second;
        }


        /*!
        * \brief If true controllers are sent incoming DataPackMessages, if false only the contained data
        */
        bool _handleDataPackMessage = false;

    private:

        /*!
         * \brief Conbfiguration of the process logger
         */
        NRPLogger _loggerCfg;

        /*!
         * \brief Name of the simulation engine
         *
         * Must be the same on the server and the client side. It should be imprinted
         * in the datapack metadata, which allows for additional consistency checks.
         */
        std::string _engineName;


        /*!
         * \brief Map of datapack names and datapack controllers used by the engine
         */
         std::map<std::string, ProtoDataPackController*> _datapacksControllers;

    protected:
        std::vector<std::unique_ptr<protobuf_ops::NRPProtobufOpsIface>> _protoOps;
        std::string _protoOpsStr = "";
};

#endif // ENGINE_PROTO_WRAPPER_H
