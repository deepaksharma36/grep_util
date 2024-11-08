cmake_minimum_required(VERSION 3.10)
# Set the project name
project(GrepUtil)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories for the project
include_directories(include)

# Specify the source files for the GrepUtil project
set(SOURCE_FILES
    src/grep_util.cpp
    src/thread_pool.cpp
    src/main.cpp
)

# Specify the directories for test files
set(TEST_FILES
    tests/grep_util_tests.cpp
)

# Create static lib of grep util
add_library(grep_util STATIC ${SOURCE_FILES})

# Add the executable for the main GrepUtil program
add_executable(grep_util_exec ${SOURCE_FILES})

# Add the test executables (GrepUtil Tests)
add_executable(grep_util_tests ${TEST_FILES})

# Include directory for header files
target_include_directories(grep_util PRIVATE include)

# Link the test executable with the GrepUtil library and GoogleTest
target_link_libraries(grep_util_tests PRIVATE grep_util GTest::gtest GTest::gtest_main)
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0  
)

FetchContent_MakeAvailable(googletest)
add_test(NAME GrepUtilTests COMMAND grep_util_tests)
enable_testing()