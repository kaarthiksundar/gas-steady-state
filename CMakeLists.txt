cmake_minimum_required(VERSION 3.11)

project("gas-steady-state")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

# option(debug "Turn on debug mode" OFF)

# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# set(CMAKE_BUILD_TYPE "Release")
# if (debug)
#set(CMAKE_BUILD_TYPE "Release")
#endif()

# Other library settings 
include_directories(${PROJECT_SOURCE_DIR}/third_party/cpp_option_parser)
add_subdirectory(third_party/cpp_option_parser)
set(LIBS ${LIBS} option_parser)
set(CPP_OPTION_PARSER option_parser)

include_directories(${PROJECT_SOURCE_DIR}/third_party/csv)

# Ipopt settings
message("Enabling IPOPT")
add_definitions(-DUSE_IPOPT)
find_package(IPOPT REQUIRED)
include_directories(${IPOPT_INCLUDE_DIRS}/..)
message("Ipopt include directories: ${IPOPT_INCLUDE_DIRS}")
set(LIBS ${LIBS} ${IPOPT_LIBRARIES})

add_subdirectory(src)
