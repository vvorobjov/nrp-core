# Smallest preset usable for "vanilla" CI on a fresh Ubuntu host:
# every optional simulator and transport is OFF, but unit testing and
# coverage stay ON. The matching apt set is requirements.basic.txt +
# requirements.cle.txt + requirements.tests.txt; no Gazebo, no NEST,
# no MQTT broker, no ROS distro have to be installed.

set(COVERAGE ON CACHE BOOL "Evaluate coverage")
set(BUILD_RST OFF CACHE BOOL "Build rst files for the documentation")

set(ENABLE_GAZEBO OFF CACHE BOOL "Enable gazebo support")
set(BUILD_GAZEBO_ENGINE_SERVER OFF CACHE BOOL "Disable gazebo engines server side")
set(ENABLE_NEST OFF CACHE BOOL "Enable nest support")
set(BUILD_NEST_ENGINE_SERVER OFF CACHE BOOL "Disable nest-simulator engines server side")
set(ENABLE_EDLUT OFF CACHE BOOL "Enable EDLUT support")
set(ENABLE_SPINNAKER OFF CACHE BOOL "Enable Spinnaker support")

set(ENABLE_ROS OFF CACHE BOOL "Enable ROS support")
set(ENABLE_MQTT OFF CACHE BOOL "Enable MQTT support")

set(ENABLE_TESTING ON CACHE BOOL "Build tests")
set(ENABLE_EXAMPLES_TESTING OFF CACHE BOOL "Run Tests with examples")
