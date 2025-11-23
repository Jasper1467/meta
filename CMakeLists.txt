cmake_minimum_required(VERSION 3.21)
project(MetaProject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(META_BUILD_SHARED "Build Meta libraries as shared libraries" ON)
option(ENABLE_SANITIZERS "Enable Address/Undefined Behavior sanitizers in Debug" ON)
option(ENABLE_LTO "Enable link-time optimization in Release/RelWithDebInfo" ON)

set(BIN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin)

# Single-config generators
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR})

# Multi-config generators (Visual Studio, Xcode)
set(CONFIGS Debug Release RelWithDebInfo)
foreach(CONFIG IN LISTS CONFIGS)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG}   ${BIN_OUTPUT_DIR}/${CONFIG})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG}   ${BIN_OUTPUT_DIR}/${CONFIG})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG}   ${BIN_OUTPUT_DIR}/${CONFIG})
endforeach()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    set(WARNINGS "-Wall -Wextra -Wpedantic")
elseif(MSVC)
    set(WARNINGS "/W4")
endif()

# Debug
set(CMAKE_CXX_FLAGS_DEBUG "${WARNINGS} -O0 -g")
if(ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address,undefined")
endif()

# Release
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
if(ENABLE_LTO)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")
endif()

# RelWithDebInfo
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
if(ENABLE_LTO)
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -flto")
endif()

if(META_BUILD_SHARED)
    set(BUILD_SHARED_LIBS ON)
else()
    set(BUILD_SHARED_LIBS OFF)
endif()

add_subdirectory(meta)
add_subdirectory(examples)
add_subdirectory(tests)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "MetaProject: C++23 Desktop Framework")
message(STATUS "Shared Libraries: ${BUILD_SHARED_LIBS}")
message(STATUS "Sanitizers Enabled: ${ENABLE_SANITIZERS}")
message(STATUS "LTO Enabled: ${ENABLE_LTO}")

