# Source files CMakeList.txt

cmake_minimum_required (VERSION 3.19)

include_directories(${CMAKE_SOURCE_DIR}/include/simpleini)
include_directories(${CMAKE_SOURCE_DIR}/src/headers)

add_executable(hlotify main.cpp notification.cpp notificationmanager.cpp config.cpp)