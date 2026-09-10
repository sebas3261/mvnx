cmake_minimum_required(VERSION 3.20)

project(mvnx VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(mvnx
    src/main.cpp
    src/commands/init/InitCommand.cpp
    src/cli/arguments/ArgumentParser.cpp
)