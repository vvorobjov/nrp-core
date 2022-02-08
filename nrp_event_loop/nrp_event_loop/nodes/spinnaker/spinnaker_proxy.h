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

#ifndef SPINNAKER_PROXY_H
#define SPINNAKER_PROXY_H

#include <boost/python.hpp>
#include <functional>
#include <nlohmann/json.hpp>
#include <thread>
#include <future>
#include <vector>
#include <iostream>
#include <SpynnakerLiveSpikesConnection.h>

#include "nrp_general_library/utils/nrp_logger.h"

namespace bpy = boost::python;

#define use(x) do {} while ((x)!=(x))

// Conversion factor from an integer containing an S1615 value to a float
#define VOLTAGE_INT_TO_S1615 32768.0f

class SpiNNakerJsonReceiveCallbackInterface {
public:
    virtual void new_msg_callback(nlohmann::json msg) = 0;
    virtual ~SpiNNakerJsonReceiveCallbackInterface() {};
};

class NRPSpinnakerProxy : SpikeReceiveCallbackInterface,
    SpikesStartCallbackInterface, PayloadReceiveCallbackInterface {

public:

    ~NRPSpinnakerProxy()
    { this->stopSpinnaker(); }

    // Delete move and copy operators. This ensures this class is a singleton
    NRPSpinnakerProxy(const NRPSpinnakerProxy &) = delete;
    NRPSpinnakerProxy(NRPSpinnakerProxy &&) = delete;

    NRPSpinnakerProxy &operator=(const NRPSpinnakerProxy &) = delete;
    NRPSpinnakerProxy &operator=(NRPSpinnakerProxy &&) = delete;

    /*!
     * \brief Get singleton instance of NRPSpinnakerProxy
     */
    static NRPSpinnakerProxy &getInstance();

    /*!
     * \brief Reset singleton instance
     */
    static NRPSpinnakerProxy &resetInstance();

    void startSpinnaker()
    {
        std::lock_guard<std::mutex> lk(runningMutex);
        if (!started) {
            started = true;
            _runFuture = std::async(&NRPSpinnakerProxy::runSpinnaker, this);
        }
    }

    void spikes_start(char *label, SpynnakerLiveSpikesConnection *connection) {
        use(label);
        use(connection);
        NRPLogger::info("SpiNNaker has started!");
        running = true;
        if (n_send_warnings > 0) {
            NRPLogger::warn("Ignored {} warnings before start", n_send_warnings);
        }
    }

    void runSpinnaker()
    {
        char **charReceiveLabels = new char*[receiveLabels.size()];
        char **charSendLabels = new char*[sendLabels.size()];
        char *a_label = NULL;
        for (std::size_t i = 0; i < receiveLabels.size(); i++) {
            charReceiveLabels[i] = (char *) receiveLabels[i].c_str();
            a_label = charReceiveLabels[i];
        }
        for (std::size_t i = 0; i < sendLabels.size(); i++) {
            charSendLabels[i] = (char *) sendLabels[i].c_str();
            a_label = charSendLabels[i];
        }

        if (a_label == NULL) {
            NRPLogger::warn("No senders or receivers have been detected!");
        } else {

            connection = new SpynnakerLiveSpikesConnection(
                    receiveLabels.size(), charReceiveLabels,
                    sendLabels.size(), charSendLabels);
            for (std::size_t i = 0; i < receiveLabels.size(); i++) {
                connection->add_receive_callback(
                    (char *) receiveLabels[i].c_str(), (SpikeReceiveCallbackInterface *) this);
                connection->add_receive_callback(
                    (char *) receiveLabels[i].c_str(), (PayloadReceiveCallbackInterface *) this);
            }
            connection->add_start_callback(a_label, this);
        }

        NRPLogger::debug("Waiting for GIL");
        PyGILState_STATE gstate = PyGILState_Ensure();
        try
        {
            if (connection != NULL) {
                NRPLogger::debug("Adding database socket address");
                pySpinnakerExt.attr("add_database_socket_address")(
                        NULL, connection->get_local_port(), NULL);
            }
            NRPLogger::info("Running forever!");
            pySpinnakerExt.attr("run_forever")();
            NRPLogger::info("Finished running forever!");
        }
        catch (bpy::error_already_set const &)
        {
            PyErr_Print();
        }
        NRPLogger::debug("Releasing GIL");
        PyGILState_Release(gstate);
    }

    void stopSpinnaker()
    {
        std::lock_guard<std::mutex> lk(runningMutex);
        if(!running)
        {
           return;
        }
        running = false;
        PyGILState_STATE gstate = PyGILState_Ensure();
        try
        {
            pySpinnakerExt.attr("request_stop")();
        }
        catch (bpy::error_already_set const &) {
            PyErr_Print();
        }
        PyGILState_Release(gstate);
        _runFuture.wait();
        gstate = PyGILState_Ensure();
        try
        {
            pySpinnaker.attr("end")();
        }
        catch (bpy::error_already_set const &) {
            PyErr_Print();
        }
        PyGILState_Release(gstate);
        started = false;
    }

    void addSender(std::string &label)
    {
        if (started) {
            NRPLogger::error("A new sender cannot be added when SpiNNaker is already running!");
        }
        if (std::find(sendLabels.begin(), sendLabels.end(), label) == sendLabels.end()) {
            sendLabels.push_back(label);
        }
    }

    //  Also adapt 'callback' signature as needed
    void addReceiver(std::string &label, SpiNNakerJsonReceiveCallbackInterface *callback)
    {
        if (started) {
            NRPLogger::error("A new receiver cannot be added when SpiNNaker is already running!");
        }
        auto label_callbacks = callbacks.find(label);
        if (label_callbacks == callbacks.end()) {
            callbacks[label] = std::vector<SpiNNakerJsonReceiveCallbackInterface *>();
            receiveLabels.push_back(label);
        }
        callbacks[label].push_back(callback);
    }

    void send(std::string &label, const nlohmann::json* data)
    {
        if (!running) {
            if (n_send_warnings == 0) {
                NRPLogger::warn("Not sending until SpiNNaker is running...");
            }
            n_send_warnings++;
            return;
        }
        std::vector<int> spikes;
        if (data->contains("neuron_ids")) {
            nlohmann::json neuron_ids = data->at("neuron_ids");
            for (size_t i = 0; i < neuron_ids.size(); i++) {
                spikes.push_back(neuron_ids[i]);
            }
        }
        std::vector<rate_details> rates_to_send;
        if (data->contains("rates")) {
            nlohmann::json rates = data->at("rates");
            for (size_t i = 0; i < rates.size(); i++) {
                int neuron_id = rates[i].at("neuron_id");
                float rate = rates[i].at("rate");
                rates_to_send.push_back({.neuron_id=neuron_id, .rate=rate});
                NRPLogger::debug("Sending rate of neuron {} to {} in {}", neuron_id, rate, label);
            }
        }
        if ((spikes.size() > 0 && rates_to_send.size() > 0)
                || (spikes.size() == 0 && rates_to_send.size() == 0)) {
            NRPLogger::error("Both neuron_ids and rates found in data.  Please"
                    " use neuron_ids to send spikes to a SpikeInjector or"
                    " rates to set the rates of a SpikeSourcePoisson");
        }
        if (spikes.size() > 0) {
            connection->send_spikes((char *) label.c_str(), spikes);
        } else {
            connection->send_rates((char *) label.c_str(), rates_to_send);
        }
    }

    void receive_spikes(char *label, int time, int n_spikes, int* spikes)
    {
        std::string strLabel(label);
        auto recvCallbacks = callbacks[strLabel];
        nlohmann::json keyArray = nlohmann::json::array();
        for (int i = 0; i < n_spikes; i++) {
            keyArray.push_back(spikes[i]);
        }
        nlohmann::json data = {
                {"label", strLabel},
                {"time", time},
                {"spikes", keyArray}
        };
        for (std::size_t i = 0; i < recvCallbacks.size(); i++) {
            recvCallbacks[i]->new_msg_callback(data);
        }
    }

    void receive_payloads(char *label, int n_payloads, payload_details *payloads) {
        std::string strLabel(label);
        auto recvCallbacks = callbacks[strLabel];
        nlohmann::json payloadArray = nlohmann::json::array();
        for (int i = 0; i < n_payloads; i++) {
            nlohmann::json payload = {
                    {"neuron_id", payloads[i].neuron_id},
                    {"voltage", (float) payloads[i].payload / VOLTAGE_INT_TO_S1615}
            };
            payloadArray.push_back(payload);
        }
        nlohmann::json data = {
                {"label", strLabel},
                {"voltages", payloadArray}
        };
        for (std::size_t i = 0; i < recvCallbacks.size(); i++) {
            recvCallbacks[i]->new_msg_callback(data);
        }
    }

private:
    bpy::object pySpinnaker;
    bpy::object pySpinnakerExt;
    std::future<void> _runFuture;
    bool started = false;
    bool running = false;
    std::mutex runningMutex;
    std::map<std::string,
             std::vector<SpiNNakerJsonReceiveCallbackInterface *>> callbacks;
    SpynnakerLiveSpikesConnection *connection = NULL;
    std::vector<std::string> receiveLabels;
    std::vector<std::string> sendLabels;
    int n_send_warnings = 0;

    NRPSpinnakerProxy()
    {
        try
        {
            pySpinnaker = bpy::import("pyNN.spiNNaker");
            pySpinnakerExt = pySpinnaker.attr("external_devices");
        }
        catch (bpy::error_already_set const &)
        {
            PyErr_Print();
        }
    }

    static std::unique_ptr<NRPSpinnakerProxy> _instance;

};

#endif //SPINNAKER_PROXY_H
