cmake_minimum_required(VERSION 2.8.3)
project(obstacle_detector)

set(CMAKE_CXX_FLAGS "-std=c++11 -fpermissive ${CMAKE_CXX_FLAGS} -Wfatal-errors\ ")

find_package(catkin REQUIRED COMPONENTS roscpp roslaunch std_msgs std_srvs geometry_msgs sensor_msgs tf laser_geometry message_generation)
find_package(Armadillo REQUIRED)
find_package(Boost 1.54.0 REQUIRED system)

add_message_files(FILES CircleObstacle.msg SegmentObstacle.msg Obstacles.msg)
generate_messages(DEPENDENCIES std_msgs geometry_msgs)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES scans_merger obstacle_extractor obstacle_tracker obstacle_publisher
  CATKIN_DEPENDS roscpp std_msgs std_srvs geometry_msgs sensor_msgs tf laser_geometry message_runtime
)

include_directories(include ${catkin_INCLUDE_DIRS} ${ARMADILLO_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

#
# Build libs
#
add_library(scans_merger src/scans_merger.cpp)
target_link_libraries(scans_merger ${catkin_LIBRARIES})
add_dependencies(scans_merger ${catkin_EXPORTED_TARGETS})

add_library(obstacle_extractor src/obstacle_extractor.cpp)
target_link_libraries(obstacle_extractor ${catkin_LIBRARIES} ${ARMADILLO_LIBRARIES})
add_dependencies(obstacle_extractor ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})

add_library(obstacle_tracker src/obstacle_tracker.cpp)
target_link_libraries(obstacle_tracker ${catkin_LIBRARIES} ${ARMADILLO_LIBRARIES})
add_dependencies(obstacle_tracker ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})

add_library(obstacle_publisher src/obstacle_publisher.cpp)
target_link_libraries(obstacle_publisher ${catkin_LIBRARIES})
add_dependencies(obstacle_publisher ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})

#
# Build nodes
#
add_executable(scans_merger_node src/nodes/scans_merger_node.cpp)
target_link_libraries(scans_merger_node scans_merger)

add_executable(obstacle_extractor_node src/nodes/obstacle_extractor_node.cpp)
target_link_libraries(obstacle_extractor_node obstacle_extractor)

add_executable(obstacle_tracker_node src/nodes/obstacle_tracker_node.cpp)
target_link_libraries(obstacle_tracker_node obstacle_tracker)

add_executable(obstacle_publisher_node src/nodes/obstacle_publisher_node.cpp)
target_link_libraries(obstacle_publisher_node obstacle_publisher)

add_executable(testing src/test.cpp)
target_link_libraries(testing ${catkin_LIBRARIES})

#
# Install libraries
#
install(TARGETS scans_merger obstacle_extractor obstacle_tracker obstacle_publisher
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION})

#
# Install nodes
#
install(TARGETS scans_merger_node obstacle_extractor_node obstacle_tracker_node obstacle_publisher_node
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

#
# Install header files
#
install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})

#
# Install launch files
#
install(DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})

