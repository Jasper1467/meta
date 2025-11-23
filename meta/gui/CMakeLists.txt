# Determine library type
if(META_BUILD_SHARED)
    set(META_GUI_TYPE SHARED)
else()
    set(META_GUI_TYPE STATIC)
endif()

# Find SDL2
find_package(SDL2 REQUIRED)
if(NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 not found!")
endif()

# Find SDL2_ttf using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
if(NOT SDL2_TTF_FOUND)
    message(FATAL_ERROR "SDL2_ttf not found!")
endif()

# Create the library (header-only for now, but allows static/shared if needed later)
add_library(meta_gui ${META_GUI_TYPE}
    gui.cpp
)

# Include directories
target_include_directories(meta_gui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}        # Public GUI headers
    ${SDL2_INCLUDE_DIRS}               # SDL2 headers
    ${SDL2_TTF_INCLUDE_DIRS}           # SDL2_ttf headers
)

# Link libraries
target_link_libraries(meta_gui PUBLIC
    meta_base                          # Depend on base module
    ${SDL2_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
)

# Optionally propagate a macro to know if GUI is shared
target_compile_definitions(meta_gui PUBLIC META_GUI_SHARED=$<BOOL:${META_BUILD_SHARED}>)

