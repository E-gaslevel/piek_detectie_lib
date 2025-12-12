cmake_minimum_required(VERSION 3.10)
project(peakdetect C)

# Library
add_library(peakdetect src/peakdetect.c)
target_include_directories(peakdetect PUBLIC include)

# Tests
enable_testing()
add_subdirectory(tests)

# Examples
add_subdirectory(examples)
