
include_directories(${PROJECT_SOURCE_DIR}/include/)

set(CMAKE_BINARY_DIR "${PROJECT_SOURCE_DIR}/bin")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

file(GLOB PROJECT_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB PROJECT_HEADERS ${PROJECT_SOURCE_DIR}/include/*.h)
add_executable(gas-steady-state ${PROJECT_SOURCES} ${PROJECT_HEADERS}) 

target_link_libraries(gas-steady-state ${IPOPT_LIBRARIES} ${CPP_OPTION_PARSER})
