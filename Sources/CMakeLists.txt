cmake_minimum_required(VERSION 3.0.0)

project(FiniteStateMachinesSolution)

set(CMAKE_CONFIGURATION_TYPES "Debug;RelWithDebInfo" CACHE STRING 
	"Semicolon separated list of supported configuration types, only supports ${CMAKE_CONFIGURATION_TYPES} anything else will be ignored." FORCE)
set(BUILD_OUTPUT_DIR "${CMAKE_BINARY_DIR}/x64" CACHE STRING "Where to put you *.exe, *dll, etc." FORCE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_OUTPUT_DIR})

add_subdirectory(FiniteStateMachine)
add_subdirectory(FiniteStateMachinesLab)
add_subdirectory(ThirdParty)
add_subdirectory(FiniteStateMachineREST)


