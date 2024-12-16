# Source files CMakeList.txt

cmake_minimum_required (VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/src/headers)

cmake_minimum_required(VERSION 3.10)

# Find the required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(XCB REQUIRED xcb xcb-util)
pkg_check_modules(DBUS REQUIRED dbus-1)

# Include directories
include_directories(${XCB_INCLUDE_DIRS} ${DBUS_INCLUDE_DIRS})

add_executable(hlotify ${SOURCES})

# Link the libraries
target_link_libraries(hlotify ${XCB_LIBRARIES} ${DBUS_LIBRARIES})
