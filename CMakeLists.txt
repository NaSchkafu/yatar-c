cmake_minimum_required(VERSION 3.5)
project(yatar_c)

include_directories(inc)

# Shell Hook dll
set (SHELL_HOOK_SOURCE_FILES src/ShellHook.cpp)
set (SHELL_HOOK_HEADER_FILES inc/ShellHook.h)
add_library (yatar_shell_hook SHARED ${SHELL_HOOK_SOURCE_FILES} ${SHELL_HOOK_HEADER_FILES})
target_compile_definitions(yatar_shell_hook PRIVATE -DDLL_BUILD)

# Main exe
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES src/main.cpp src/MainWindow.cpp src/AppBar.cpp src/WindowList.cpp inc/WindowList.h inc/common.h)
add_executable(yatar_c ${SOURCE_FILES})
target_link_libraries(yatar_c
  yatar_shell_hook)



if(MSVC) # Check if we are using the Visual Studio compiler
	set_target_properties(yatar_c PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
	add_definitions(-DUNICODE)
endif()
