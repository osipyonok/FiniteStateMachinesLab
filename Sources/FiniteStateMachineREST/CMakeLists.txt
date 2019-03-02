cmake_minimum_required(VERSION 3.0.0)

project(FiniteStateMachineREST)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")

set(CMAKE_AUTOMOC ON)

file(GLOB FiniteStateMachineREST_SRC
    "*.h"
    "*.cpp"
    "*.ui"
)

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
add_executable(FiniteStateMachineREST ${FiniteStateMachineREST_SRC})

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

target_link_libraries(FiniteStateMachineREST Qt5::Core Qt5::Network)
target_link_libraries(FiniteStateMachineREST FiniteStateMachine)
target_link_libraries(FiniteStateMachineREST QtWebApp)