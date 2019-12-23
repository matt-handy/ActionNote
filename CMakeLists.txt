cmake_minimum_required(VERSION 2.8.9)
set(CMAKE_CXX_STANDARD 17)
project(ActionNote)

include_directories(include)
include_directories(../CppCommons/include)

file(GLOB SOURCES "src/*.cpp")

add_executable(action_note ${SOURCES})

find_library(CPPCOMMON_LIB cpp_common HINTS lib)
target_link_libraries (action_note ${CPPCOMMON_LIB})

if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
