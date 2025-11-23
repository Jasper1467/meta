# Find SDL2
find_package(SDL2 REQUIRED)
if(NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 not found!")
endif()

# Use pkg-config to find SDL2_ttf
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)

# Meta GUI library
add_library(meta_gui INTERFACE)
target_include_directories(meta_gui INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)
target_link_libraries(meta_gui INTERFACE meta_base ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})

