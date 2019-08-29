
include_directories(${PROJECT_SOURCE_DIR}/include/)

set(CMAKE_BINARY_DIR "${PROJECT_SOURCE_DIR}/bin")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_executable(gas-steady-state
    main.cpp 
) 

target_link_libraries(gas-steady-state ${GRAVITY_LIBRARIES})
