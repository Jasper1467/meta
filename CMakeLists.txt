cmake_minimum_required(VERSION 3.16)
project(MetaProject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add subdirectories
add_subdirectory(meta)
add_subdirectory(examples)
add_subdirectory(tests)

