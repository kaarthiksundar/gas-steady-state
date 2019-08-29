cmake_minimum_required(VERSION 3.11)

project("gas-steady-state")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

option(debug "Turn on debug mode" ON)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast")

set(CMAKE_BUILD_TYPE "Release")
if (debug)
    set(CMAKE_BUILD_TYPE "Debug")
endif()



# LEMON settings
find_package(GRAVITY)
include_directories(${GRAVITY_INCLUDE_DIRS})
set(LIBS ${LIBS} ${GRAVITY_LIBRARIES})

add_subdirectory(src)