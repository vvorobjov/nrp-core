# Canonical cmake preset for the Ubuntu 22.04 (jammy) nrp-nest-gazebo
# image. Identical settings to nest-gazebo.cmake — kept as a separate
# file so the devcontainer and the Jenkins matrix can refer to an
# unambiguous ubuntu22 target name.
set(COVERAGE ON CACHE BOOL "Evaluate coverage")
set(BUILD_RST OFF CACHE BOOL "Build rst files for the documentation")


set(ENABLE_GAZEBO ON CACHE BOOL "Enable gazebo support")
set(BUILD_GAZEBO_ENGINE_SERVER ON CACHE BOOL "Disable nest-simulator engines server side")
set(ENABLE_NEST ON CACHE BOOL "Enable nest support")
set(BUILD_NEST_ENGINE_SERVER ON CACHE BOOL  "Disable nest-simulator engines server side")


set(ENABLE_ROS ON CACHE BOOL "Enable ROS support")
set(ENABLE_MQTT ON CACHE BOOL "Enable MQTT support")
