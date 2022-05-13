# -*- coding: utf-8 -*-
"""
This file contains the setup of the neuronal network running the Husky experiment with neuronal image recognition
"""
# pragma: no cover

import pyNN.spiNNaker as sim
from spynnaker.pyNN.external_devices_models\
    .abstract_multicast_controllable_device import (
        AbstractMulticastControllableDevice, SendType)
from spynnaker.pyNN.spynnaker_external_device_plugin_manager import (
    SpynnakerExternalDevicePluginManager as Ext)
from spinnman.messages.eieio import EIEIOType
from data_specification.enums.data_type import DataType


class LiveVoltageDevice(AbstractMulticastControllableDevice):

    def __init__(self, key, time_betweeen_sending, partition):
        self.__key = key
        self.__time_between_sending = time_betweeen_sending
        self.__partition = partition

    @property
    def device_control_key(self):
        return self.__key

    @property
    def device_control_max_value(self):
        return DataType.S1615.max

    @property
    def device_control_min_value(self):
        return DataType.S1615.min

    @property
    def device_control_partition_id(self):
        return self.__partition

    @property
    def device_control_scaling_factor(self):
        return 1.0

    @property
    def device_control_send_type(self):
        return SendType.SEND_TYPE_ACCUM

    @property
    def device_control_timesteps_between_sending(self):
        return self.__time_between_sending

    @property
    def device_control_uses_payload(self):
        return True

SENSORPARAMS = {'v_rest': -60.5,
                'cm': 0.025,
                'tau_m': 10.,
                'tau_refrac': 10.0,
                'tau_syn_E': 2.5,
                'tau_syn_I': 2.5,
                'e_rev_E': 0.0,
                'e_rev_I': -75.0,
                'v_thresh': -60.0,
                'v_reset': -60.5}

GO_ON_PARAMS = {'v_rest': -60.5,
                'cm': 0.025,
                'tau_m': 10.0,
                'e_rev_E': 0.0,
                'e_rev_I': -75.0,
                'v_reset': -61.6,
                'v_thresh': -60.51,
                'tau_refrac': 10.0,
                'tau_syn_E': 2.5,
                'tau_syn_I': 2.5}

sim.setup(1.0)

# POPULATION_PARAMS = SENSORPARAMS * 5 + GO_ON_PARAMS + SENSORPARAMS * 2
red_left_eye = sim.Population(1, sim.IF_cond_exp(**SENSORPARAMS), label="red_left_eye")
red_right_eye = sim.Population(1, sim.IF_cond_exp(**SENSORPARAMS), label="red_right_eye")
green_blue_eye = sim.Population(1, sim.IF_cond_exp(**SENSORPARAMS), label="green_blue_eye")
go_on = sim.Population(1, sim.IF_cond_exp(**GO_ON_PARAMS), label="go_on")
left_wheel_motor = sim.Population(1, sim.IF_cond_exp(**SENSORPARAMS), label="left_wheel_motor")
right_wheel_motor = sim.Population(1, sim.IF_cond_exp(**SENSORPARAMS), label="right_wheel_motor")

# Inputs
red_left_eye_poisson = sim.Population(1, sim.SpikeSourcePoisson(rate=0), label="red_left_eye_poisson")
red_right_eye_poisson = sim.Population(1, sim.SpikeSourcePoisson(rate=0), label="red_right_eye_poisson")
green_blue_eye_poisson = sim.Population(1, sim.SpikeSourcePoisson(rate=0), label="green_blue_eye_poisson")
sim.Projection(red_left_eye_poisson, red_left_eye, sim.AllToAllConnector(), sim.StaticSynapse(weight=0.00015))
sim.Projection(red_right_eye_poisson, red_right_eye, sim.AllToAllConnector(), sim.StaticSynapse(weight=0.00015))
sim.Projection(green_blue_eye_poisson, green_blue_eye, sim.AllToAllConnector(), sim.StaticSynapse(weight=0.00015))
sim.external_devices.add_poisson_live_rate_control(red_left_eye_poisson, notify=False)
sim.external_devices.add_poisson_live_rate_control(red_right_eye_poisson, notify=False)
sim.external_devices.add_poisson_live_rate_control(green_blue_eye_poisson, notify=False)

# Outputs - each needs a different "key"
left_wheel_motor_voltage = sim.Population(1, sim.external_devices.ExternalDeviceLifControl(
    [LiveVoltageDevice(0, 100, "voltage")], False), label="left_wheel_motor_voltage")
right_wheel_motor_voltage = sim.Population(1, sim.external_devices.ExternalDeviceLifControl(
    [LiveVoltageDevice(1, 100, "voltage")], False), label="right_wheel_motor_voltage")
sim.Projection(left_wheel_motor, left_wheel_motor_voltage, sim.OneToOneConnector(), sim.StaticSynapse(weight=0.002))
sim.Projection(right_wheel_motor, right_wheel_motor_voltage, sim.OneToOneConnector(), sim.StaticSynapse(weight=0.002))
Ext.update_live_packet_gather_tracker(
    left_wheel_motor_voltage._vertex, "LiveSpikeReceiver",
    message_type=EIEIOType.KEY_PAYLOAD_32_BIT,
    payload_as_time_stamps=False, use_payload_prefix=False,
    partition_ids=["voltage"], port=10000, hostname="localhost")
Ext.update_live_packet_gather_tracker(
    right_wheel_motor_voltage._vertex, "LiveSpikeReceiver",
    message_type=EIEIOType.KEY_PAYLOAD_32_BIT,
    payload_as_time_stamps=False, use_payload_prefix=False,
    partition_ids=["voltage"], port=10000, hostname="localhost")


# Synaptic weights
WEIGHT_RED_TO_ACTOR = 1.5e-3
WEIGHT_RED_TO_GO_ON = 1.2e-2
WEIGHT_GREEN_BLUE_TO_ACTOR = 1.05e-3
WEIGHT_GO_ON_TO_RIGHT_ACTOR = 1.4e-3
DELAY = 1.0

# Connect neurons
sim.Projection(
    red_left_eye, right_wheel_motor, sim.AllToAllConnector(),
    sim.StaticSynapse(weight=WEIGHT_RED_TO_ACTOR, delay=DELAY),
    receptor_type='excitatory')
sim.Projection(
    red_right_eye, left_wheel_motor, sim.AllToAllConnector(),
    sim.StaticSynapse(weight=WEIGHT_RED_TO_ACTOR, delay=DELAY),
    receptor_type='excitatory')

sim.Projection(
    red_left_eye, green_blue_eye, sim.AllToAllConnector(),
    sim.StaticSynapse(weight=WEIGHT_RED_TO_GO_ON, delay=DELAY),
    receptor_type='inhibitory')
sim.Projection(
    red_left_eye, go_on, sim.AllToAllConnector(),
    sim.StaticSynapse(weight=WEIGHT_RED_TO_GO_ON, delay=DELAY),
    receptor_type='inhibitory')

sim.Projection(
    green_blue_eye, right_wheel_motor, sim.AllToAllConnector(),
    sim.StaticSynapse(weight=WEIGHT_GREEN_BLUE_TO_ACTOR, delay=DELAY),
    receptor_type='excitatory')

sim.Projection(
    go_on, right_wheel_motor, sim.AllToAllConnector(),
    sim.StaticSynapse(weight=WEIGHT_GO_ON_TO_RIGHT_ACTOR, delay=DELAY),
    receptor_type='excitatory')
