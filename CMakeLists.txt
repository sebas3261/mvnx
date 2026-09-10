cmake_minimum_required(VERSION 3.20)

project(mvnx VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    replxx
    GIT_REPOSITORY https://github.com/AmokHuginnsson/replxx.git
    GIT_TAG master
)

FetchContent_MakeAvailable(replxx)

add_executable(mvnx
    src/main.cpp

    src/commands/init/InitCommand.cpp
    src/commands/add/AddCommand.cpp

    src/project/ProjectNaming.cpp
    src/project/ProjectValidator.cpp
    src/project/ProjectGenerator.cpp

    src/cli/arguments/ArgumentParser.cpp
    src/cli/prompt/Prompt.cpp
)

target_include_directories(mvnx PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(mvnx PRIVATE
    replxx
)

include(CTest)

if(BUILD_TESTING)
    add_executable(mvnx_project_validator_tests
        tests/ProjectValidatorTests.cpp
        src/project/ProjectValidator.cpp
        src/project/ProjectNaming.cpp
    )

    target_include_directories(mvnx_project_validator_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

    add_test(NAME project_validator COMMAND mvnx_project_validator_tests)
endif()
