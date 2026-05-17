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

#ifndef ROS_PROXY_H
#define ROS_PROXY_H

// ─────────────────────────────────────────────────────────────────────
// NRP ROS 2 proxy (rclcpp)
//
// Singleton wrapping a rclcpp::Node with ROS 2 publishers and
// subscriptions used by the NRPCore event-loop ROS nodes. Replaces
// the previous ROS 1 roscpp implementation (ros::NodeHandle +
// ros::Publisher / ros::Subscriber).
//
// ROS 2 publishers are typed (rclcpp::Publisher<MsgT>), unlike the
// untyped ros::Publisher. We erase the type by storing a
// PublisherBase::SharedPtr plus a std::function closure that knows
// how to publish the concrete message type.
// ─────────────────────────────────────────────────────────────────────

#include <chrono>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "rclcpp/rclcpp.hpp"

class NRPROSProxy {

public:

    // Delete move and copy operators. This ensures this class is a singleton
    NRPROSProxy(const NRPROSProxy &) = delete;
    NRPROSProxy(NRPROSProxy &&) = delete;

    NRPROSProxy &operator=(const NRPROSProxy &) = delete;
    NRPROSProxy &operator=(NRPROSProxy &&) = delete;

    /*!
     * \brief Get singleton instance of NRPROSProxy
     */
    static NRPROSProxy &getInstance();

    /*!
     * \brief Reset singleton instance
     */
    static NRPROSProxy &resetInstance();

    /*!
     * \brief Subscribe to ROS 2 topic 'address' with callback 'callback'.
     *
     * The callback signature matches rclcpp: it receives a
     * std::shared_ptr<const MSG_TYPE>.
     */
    template<class MSG_TYPE>
    void subscribe(const std::string& address,
                   const std::function<void(std::shared_ptr<const MSG_TYPE>)>& callback,
                   size_t queueSize = 10)
    {
        auto sub = _node->create_subscription<MSG_TYPE>(address, queueSize, callback);
        _subscribers.push_back(sub);
    }

    /*!
     * \brief Publishes 'msg' to ROS 2 topic 'address'.
     *
     * The first call for a given address creates a typed
     * rclcpp::Publisher<MSG_TYPE> and stores both its
     * PublisherBase::SharedPtr and a std::function closure that
     * captures the typed pointer. Subsequent calls reuse the closure.
     */
    template<class MSG_TYPE>
    void publish(const std::string& address, const MSG_TYPE& msg, size_t queueSize = 10)
    {
        if(!_publishers.count(address)) {
            auto pub = _node->create_publisher<MSG_TYPE>(address, queueSize);
            _publishers.emplace(address, PublisherEntry{
                pub,
                [pub](const void* m) { pub->publish(*static_cast<const MSG_TYPE*>(m)); }
            });
        }
        _publishers[address].publish_fn(&msg);
    }

    /*!
     * \brief Process pending subscription callbacks (ROS 2 equivalent
     * of the ROS 1 ros::spinOnce). Drains any messages that arrived
     * since the last call and invokes their callbacks once.
     *
     * rclcpp::spin_some only drains messages that are present in the
     * subscription history *at the moment of the call*: rcl_wait is
     * invoked with timeout=0 and returns as soon as nothing is ready,
     * even if a DDS receiver thread is one millisecond away from
     * pushing the next in-flight message into the same history. With
     * a back-to-back publish burst (test_python_ros_nodes.cpp
     * publishes "first" then "second" to the same topic and then
     * sleeps 1 s before calling spinSome) the result is that the
     * first spin_some only picks up "first"; "second" stays buffered
     * inside DDS and is processed by the next spin_some — which in
     * the test is one sleep(1) later, so the graph computes with the
     * older "first" instead of the intended "second" and the
     * assertion fails.
     *
     * Loop spin_some with a brief sleep so any message that lands in
     * the subscription history within the next ~100 ms is processed
     * before this call returns. 100 ms is well below the test's
     * sleep(1) granularity and small enough not to noticeably slow
     * down real event-loop iterations.
     */
    void spinSome()
    {
        const auto deadline = std::chrono::steady_clock::now()
                              + std::chrono::milliseconds(100);
        do {
            rclcpp::spin_some(_node);
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        } while(std::chrono::steady_clock::now() < deadline);
        rclcpp::spin_some(_node);
    }

    /*!
     * \brief Access the underlying rclcpp::Node (for tests / advanced users).
     */
    rclcpp::Node::SharedPtr node() const { return _node; }

private:

    /*!
     * Type-erased publisher entry. We keep the PublisherBase to own
     * the publisher, and a std::function closure that captures the
     * typed publisher and publishes the correct MSG_TYPE.
     */
    struct PublisherEntry {
        rclcpp::PublisherBase::SharedPtr pub;
        std::function<void(const void*)> publish_fn;
    };

    rclcpp::Node::SharedPtr _node;
    std::map<std::string, PublisherEntry> _publishers;
    std::vector<rclcpp::SubscriptionBase::SharedPtr> _subscribers;

    /*!
     * Constructor. Requires rclcpp::init() to have been called by
     * the executable before the singleton is instantiated.
     *
     * Registers an rclcpp on-shutdown callback that releases the
     * node, subscribers and publishers BEFORE rcl finishes shutting
     * down. Otherwise the static _instance destructor runs at
     * _dl_fini (process exit, after rclcpp::shutdown returned), the
     * rclcpp::Subscription destructors call into FastDDS to
     * deregister, and FastDDS's RTPSParticipant is already gone —
     * the destructor SIGSEGVs in eprosima::fastrtps::rtps::
     * RTPSParticipant::getGuid(). Clearing the resources while rcl
     * is still alive makes the static destructor a no-op.
     */
    NRPROSProxy()
    {
        _node = rclcpp::Node::make_shared("nrp_core");
        rclcpp::on_shutdown([this]() {
            _subscribers.clear();
            _publishers.clear();
            _node.reset();
        });
    }

    static std::unique_ptr<NRPROSProxy> _instance;

};

#endif //ROS_PROXY_H
