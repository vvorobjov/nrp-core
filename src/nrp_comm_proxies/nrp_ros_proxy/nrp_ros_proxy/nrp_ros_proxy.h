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

#include <functional>
#include <map>
#include <memory>
#include <string>
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
     */
    void spinSome()
    { rclcpp::spin_some(_node); }

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
     */
    NRPROSProxy()
    { _node = rclcpp::Node::make_shared("nrp_core"); }

    static std::unique_ptr<NRPROSProxy> _instance;

};

#endif //ROS_PROXY_H
