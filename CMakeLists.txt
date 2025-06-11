cmake_minimum_required(VERSION 3.27)
project(sw_battle_test)

option(BUILD_TESTING "Build tests for sw_battle_test" OFF)

set(CMAKE_CXX_STANDARD 20)

if(BUILD_TESTING)
    include(FetchContent)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
    )

    FetchContent_MakeAvailable(googletest)

    include(CTest)
    enable_testing()
    add_subdirectory(tests)
    message(STATUS "Google Test found and configured.")
else()
    message(STATUS "Building tests is disabled by BUILD_TESTING option.")
endif()

file(GLOB_RECURSE SOURCES src/*.cpp src/*.hpp)
add_executable(sw_battle_test ${SOURCES})

target_include_directories(sw_battle_test PUBLIC src/)
