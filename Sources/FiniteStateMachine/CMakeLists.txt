cmake_minimum_required(VERSION 3.0.0)

project(FiniteStateMachine)

add_definitions(-DFINITESTATEMACHINE_LIB)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")

file(GLOB FiniteStateMachine_SRC
    "*.h"
    "*.cpp"
    "*.ui"
)

add_library(FiniteStateMachine SHARED ${FiniteStateMachine_SRC})

find_package(Qt5Core REQUIRED)

target_link_libraries(FiniteStateMachine Qt5::Core)

target_include_directories(FiniteStateMachine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)