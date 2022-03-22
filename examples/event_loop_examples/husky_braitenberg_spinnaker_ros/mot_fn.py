from nrp_core.data.nrp_ros.geometry_msgs import Twist
from nrp_core.event_loop import *


@RosPublisher(keyword="msg", address="/husky/cmd_vel", type=Twist)
@FromSpinnaker(keyword="right_voltage_msg", address="right_wheel_motor_voltage")
@FromSpinnaker(keyword="left_voltage_msg", address="left_wheel_motor_voltage")
@FunctionalNode(name="robot_control", outputs=['msg'])
def robot_control(left_voltage_msg, right_voltage_msg):

    msg = Twist()

    if left_voltage_msg is None or right_voltage_msg is None:
        return [msg]

    left_voltage = left_voltage_msg["voltages"][0]["voltage"]
    right_voltage = right_voltage_msg["voltages"][0]["voltage"]

    msg.linear.x = 20.0 * min(left_voltage, right_voltage)
    msg.linear.y = 0.0
    msg.linear.z = 0.0
    msg.angular.x = 0.0
    msg.angular.y = 0.0
    msg.angular.z = 100.0 * (right_voltage - left_voltage)

    return [msg]

