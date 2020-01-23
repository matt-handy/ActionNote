cmake_minimum_required(VERSION 2.8.9)
set(CMAKE_CXX_STANDARD 17)
project(ActionNote)

include_directories(include)
include_directories(../CppCommons/include)
include_directories(../Common/tiny-AES-c)

file(GLOB SOURCES "src/*.cpp")

add_executable(action_note ${SOURCES} ../Common/tiny-AES-c/aes.c)
add_library(lib_action_note STATIC src/action_item.cpp src/main.cpp src/meeting.cpp src/meeting_manager.cpp ../Common/tiny-AES-c/aes.c)

find_library(CPPCOMMON_LIB cpp_common HINTS lib)
target_link_libraries (action_note ${CPPCOMMON_LIB})
target_link_libraries (lib_action_note ${CPPCOMMON_LIB})

if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
