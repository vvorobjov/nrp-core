Steps to start the experiment:

1. Install required packages: `sudo apt install ros-noetic-gazebo-ros ros-noetic-gazebo-ros-control ros-noetic-gazebo-plugins`
1. Compile the husky gazebo plugin:
  1. extract `husky_gazebo_plugins.tar.gz` and move its content to your catkin workspace. Create one if you don't have it.
  2. build your catkin workspace (using the `catkin_make` command) and make the built file `${CATKIN_WS}/devel/lib/libhusky_gazebo_plugins.so`, being `${CATKIN_WS}` the path to your catkin workspace, available to gazebo by placing it in a folder in the `GAZEBO_PLUGIN_PATH` env variable.
  3. setup your catkin workspace`source ${CATKIN_WS}/devel/setup.bash`.
2. Edit the file `${HOME}/.gazebo/models/virtual_room/virtual_room.sdf` and change the material of `right_vr_screen` model from `BlueGlow` to `RedGlow`. This step is optional. Only required to check how the robot moves towards the red screen.
4. Start the experiment normally with NRPCoreSim.
