from nrp_core.data.nrp_ros.geometry_msgs import Twist
from nrp_core.event_loop import *


@RosPublisher(keyword="msg", address="/husky/cmd_vel", type=Twist)
@FromEngine(keyword='actors', address='/nest/actors')
@FunctionalNode(name="robot_control", outputs=['msg'])
def robot_control(actors):
    left_voltage = actors.data[0]['V_m']
    right_voltage = actors.data[1]['V_m']

    msg = Twist()
    msg.linear.x = 20.0 * min(left_voltage, right_voltage)
    msg.linear.y = 0.0
    msg.linear.z = 0.0
    msg.angular.x = 0.0
    msg.angular.y = 0.0
    msg.angular.z = 100.0 * (right_voltage - left_voltage)

    return [msg]

