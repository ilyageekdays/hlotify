# Source files CMakeList.txt

cmake_minimum_required (VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES main.cpp notification.cpp notification_manager.cpp config.cpp notification_elements.cpp hlotify_exception.cpp hlotify_logger.cpp)

include_directories(${CMAKE_SOURCE_DIR}/include/simpleini)
include_directories(${CMAKE_SOURCE_DIR}/src/headers)

option(WAYLAND "Use Wayland for graphics" OFF)
option(GRAPHICS "Use Graphics libs" OFF)

if (WAYLAND AND GRAPHICS)
    find_package(Wayland REQUIRED)
    add_definitions(-DUSE_WAYLAND)
    set(SOURCES ${SOURCES} WaylandBackend.cpp)
    set(LIBS ${LIBS} ${WAYLAND_LIBRARIES})
    include_directories(${WAYLAND_INCLUDE_DIR})
elseif(GRAPHICS)
    find_package(X11 REQUIRED)
    add_definitions(-DUSE_X11)
    set(SOURCES ${SOURCES} X11Backend.cpp)
    set(LIBS ${LIBS} ${X11_LIBRARIES})
    include_directories(${X11_INCLUDE_DIR})
endif()

add_executable(hlotify ${SOURCES})