# husky_braitenberg_spinnaker_ros

**Status: not runnable as shipped. The Gazebo side of this example is still ROS 1 and has to be ported to ROS 2 Humble; the brain additionally needs SpiNNaker hardware (sPyNNaker).**

nrp-core itself is ROS 2 (Humble) only: the `ROSNode` connector, the Computational Graph `RosSubscriber` / `RosPublisher` nodes and the `nrp_core.data.nrp_ros.*` message bindings used by `cam_fn.py` and `mot_fn.py` were ported in EBR2-26, and the ROS 1 (Noetic / Ubuntu 20.04) chain was removed in EBR2-81. The Gazebo side of this example was not ported:

- `husky_gazebo_plugins.tar.gz` is a catkin package (`roscpp`, the ROS 1 `gazebo_ros` plugin API, `message_generation` for its `msg`/`srv` files);
- `husky_world.launch` is a `roslaunch` XML file that includes `gazebo_ros/launch/empty_world.launch` and spawns the robot with `spawn_model`; `simulation_config.json` starts it through `"ProcCmd": "roslaunch husky_world.launch"`.

Neither builds nor starts in a ROS 2 Humble environment, so the experiment does not run until the steps below are done. The porting work is the same as for `husky_braitenberg_ros`; the two examples ship the same plugin archive and launch file.

## Porting checklist

1. Convert `husky_gazebo_plugins` to an `ament_cmake` package: port `husky_plugin.cpp` to the ROS 2 `gazebo_ros` API (`ros-humble-gazebo-ros-pkgs`, Gazebo 11 Classic) and generate `WheelSpeeds.msg` and the `srv` files with `rosidl_default_generators`.
2. Replace `husky_world.launch` with a ROS 2 launch file (`gazebo_ros` `gzserver.launch.py` plus `spawn_entity.py`, world `${HOME}/.gazebo/models/virtual_room/virtual_room.sdf`, model `${HOME}/.gazebo/models/husky_model/model.sdf`) and change `ProcCmd` to `ros2 launch <launch file>`.
3. Build the package in a colcon workspace (`source /opt/ros/humble/setup.bash`, `colcon build`, `source <workspace>/install/setup.bash`) and add the directory containing `libhusky_gazebo_plugins.so` to `GAZEBO_PLUGIN_PATH`.

## Running (after porting)

Source `/opt/ros/humble/setup.bash` and the colcon workspace, make sure the SpiNNaker board and the sPyNNaker installation used by `braitenberg.py` are reachable, then start the experiment normally with `NRPCoreSim -c simulation_config.json`.

Optional: edit `${HOME}/.gazebo/models/virtual_room/virtual_room.sdf` and change the material of the `right_vr_screen` model from `BlueGlow` to `RedGlow` to check that the robot moves towards the red screen.
