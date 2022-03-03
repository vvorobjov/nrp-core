# Joint movement range

joint_min_position   = -0.261799
joint_max_position   = 1.5708
joint_position_range = joint_max_position - joint_min_position

# TVB displacement range

tvb_max_displacement   = 1.6
tvb_min_displacement   = -1.6
tvb_displacement_range = tvb_max_displacement - tvb_min_displacement


def convert_to_joint_range(value_tvb):
    return (((value_tvb  - tvb_min_displacement) * joint_position_range) / tvb_displacement_range) + joint_min_position


def convert_to_tvb_range(value_joint):
    return (((value_joint  - joint_min_position) * tvb_displacement_range) / joint_position_range) + tvb_min_displacement