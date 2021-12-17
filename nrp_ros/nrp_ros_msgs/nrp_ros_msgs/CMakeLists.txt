set(PROJECT_NAME "nrp_ros_msgs")
project("${PROJECT_NAME}" VERSION 1.0)

##########################################
## Dependencies
find_package(catkin REQUIRED COMPONENTS
  std_msgs
  geometry_msgs
  message_generation
  )

##########################################
## Generate msgs
add_message_files(
  FILES
  Test.msg
  )

generate_messages(DEPENDENCIES
  std_msgs
  geometry_msgs
  )

catkin_package(
  CATKIN_DEPENDS
  message_runtime
  std_msgs
  geometry_msgs
  )
