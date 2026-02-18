cmake_minimum_required(VERSION 4.2)

project(LazyLS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main Executable
add_executable(${PROJECT_NAME} src/main.cpp)

# Library for the logic
add_library(LazyLS_Lib src/util.cpp src/objects.cpp src/file.cpp src/input.cpp)
target_include_directories(LazyLS_Lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/inc")
target_precompile_headers(LazyLS_Lib PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/inc/pch.h")
target_link_libraries(${PROJECT_NAME} PRIVATE LazyLS_Lib)

# Google Test Setup
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

# For Windows compatibility
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add the Test Executable
enable_testing()

# Find the Test Files
file(GLOB TEST_SOURCES tests/*.cpp)

add_executable(run_tests ${TEST_SOURCES})
target_link_libraries(run_tests PRIVATE LazyLS_Lib GTest::gtest_main)

# Discover tests automatically
include(GoogleTest)
gtest_discover_tests(run_tests)