cmake_minimum_required(VERSION 3.16)
project(MetaProject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Top-level option for shared or static libraries
option(META_BUILD_SHARED "Build Meta libraries as shared libraries" ON)

# Directory for all final binaries
set(BIN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin)

# Single-config generator (Makefile, Ninja)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR})

# Multi-config generator (Visual Studio, Xcode)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${BIN_OUTPUT_DIR}/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${BIN_OUTPUT_DIR}/Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   ${BIN_OUTPUT_DIR}/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${BIN_OUTPUT_DIR}/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${BIN_OUTPUT_DIR}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_OUTPUT_DIR}/Release)

# Add subdirectories
add_subdirectory(meta)
add_subdirectory(examples)
add_subdirectory(tests)

