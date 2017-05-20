cmake_minimum_required(VERSION 3.5)
project(yatar_c)

include_directories(inc)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES src/main.cpp src/MainWindow.cpp src/AppBar.cpp src/WindowList.cpp src/Window.cpp inc/WindowList.h inc/common.h inc/Window.h)
add_executable(yatar_c ${SOURCE_FILES})


if(MSVC) # Check if we are using the Visual Studio compiler
	set_target_properties(yatar_c PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
	add_definitions(-DUNICODE)
endif()
