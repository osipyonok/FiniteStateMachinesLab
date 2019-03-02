cmake_minimum_required(VERSION 3.0.0)

project(QtWebApp)

add_definitions(-DQTWEBAPPLIB_EXPORT)
add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
set(CMAKE_AUTOMOC ON)

file(GLOB QtWebApp_SRC
    "*.h"
    "*.cpp"
    "*.rc"
    "httpserver/*.h"
    "httpserver/*.cpp"
    "logging/*.h"
    "logging/*.cpp"
    "templateengine/*.h"
    "templateengine/*.cpp"
)

set(QtService_SRC qtservice.h qtservice_p.h qtservice.cpp)
if(WIN32)
  set(QtService_SRC ${QtService_SRC} qtservice_win.cpp)
elseif(UNIX)
  set(QtService_SRC ${QtService_SRC} qtunixserversocket.h qtunixsocket.h qtunixsocket.cpp qtunixserversocket.cpp qtservice_unix.cpp)
elseif()
  message("Can not configure qtservice for your system")
endif()

foreach(src_file ${QtService_SRC})
  set(QtWebApp_SRC "${QtWebApp_SRC};${CMAKE_CURRENT_SOURCE_DIR}/qtservice/${src_file}")
endforeach()


add_library(QtWebApp SHARED ${QtWebApp_SRC})

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

target_link_libraries(QtWebApp Qt5::Core Qt5::Network)

target_include_directories(QtWebApp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/httpserver)
target_include_directories(QtWebApp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/logging)
target_include_directories(QtWebApp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/templateengine)
target_include_directories(QtWebApp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/qtservice)