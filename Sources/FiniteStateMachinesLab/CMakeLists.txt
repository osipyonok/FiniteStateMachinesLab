cmake_minimum_required(VERSION 3.0.0)

project(FiniteStateMachineLab)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB FiniteStateMachineLab_SRC
    "*.h"
    "*.cpp"
    "*.ui"
    "*.qrc"
)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
add_executable(FiniteStateMachineLab ${FiniteStateMachineLab_SRC})

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

target_link_libraries(FiniteStateMachineLab Qt5::Core Qt5::Network Qt5::Gui Qt5::Widgets)
target_link_libraries(FiniteStateMachineLab FiniteStateMachine)