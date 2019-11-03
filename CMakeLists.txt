cmake_minimum_required(VERSION 2.8.9)
project(ActionNote)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(action_note ${SOURCES})