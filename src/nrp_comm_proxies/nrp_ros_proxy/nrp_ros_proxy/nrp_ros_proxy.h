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

#include "ros/ros.h"
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <functional>

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
     * \brief Subscribe to ROS topic 'address' with callback function 'callback'
     */
    template<class MSG_TYPE>
    void subscribe(const std::string& address, const boost::function<void (const boost::shared_ptr<MSG_TYPE const>&)>& callback, size_t queueSize = 10)
    { _subscribers.push_back(_node->subscribe<MSG_TYPE>(address, queueSize, callback)); }

    /*!
     * \brief Publishes 'msg' to ROS topic 'address'
     */
    template<class MSG_TYPE>
    void publish(const std::string& address, const MSG_TYPE& msg, size_t queueSize = 10)
    {
        if(!_publishers.count(address))
            _publishers.emplace(address, _node->advertise<MSG_TYPE>(address, queueSize));

        _publishers[address].publish(msg);
    }

private:

    std::shared_ptr<ros::NodeHandle> _node;
    std::map<std::string, ros::Publisher> _publishers;
    std::vector<ros::Subscriber> _subscribers;

    /*!
     * Constructor
     */
    NRPROSProxy()
    { _node.reset(new ros::NodeHandle()); }

    static std::unique_ptr<NRPROSProxy> _instance;

};

#endif //ROS_PROXY_H
