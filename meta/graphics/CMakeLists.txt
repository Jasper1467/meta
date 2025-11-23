if(META_BUILD_SHARED)
    set(META_GRAPHICS_TYPE SHARED)
else()
    set(META_GRAPHICS_TYPE STATIC)
endif()

# Find SDL2
find_package(SDL2 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

# Graphics library
add_library(meta_graphics ${META_GRAPHICS_TYPE}
    backend/SDL2Texture.cpp
)

target_include_directories(meta_graphics PUBLIC
    ${CMAKE_SOURCE_DIR}                # <- parent of meta
    ${CMAKE_CURRENT_SOURCE_DIR}        # graphics headers
    ${CMAKE_CURRENT_SOURCE_DIR}/backend
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
)


# Link libraries
target_link_libraries(meta_graphics PUBLIC
    meta_base
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
)

target_compile_definitions(meta_graphics PUBLIC META_GRAPHICS_SHARED=$<BOOL:${META_BUILD_SHARED}>)

