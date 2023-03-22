# Opensim action range [N * m]

action_min_value = 0.02
action_max_value = 0.1
action_range     = action_max_value - action_min_value

# TVB voltage range [V]

tvb_min_voltage   = -3.0
tvb_max_voltage   = 3.0
tvb_voltage_range = tvb_max_voltage - tvb_min_voltage

# Opensim joint displacement range [rad]

opensim_min_displacement   = 0.0
opensim_max_displacement   = 3.0
opensim_displacement_range = opensim_max_displacement - opensim_min_displacement

def tvb_voltage_to_opensim_action(value):
    return (((value - tvb_min_voltage) * action_range) / tvb_voltage_range) + action_min_value


def opensim_displacement_to_tvb_voltage(value):
    return (((value - opensim_min_displacement) * tvb_voltage_range) / opensim_displacement_range) + tvb_min_voltage

# EOF
