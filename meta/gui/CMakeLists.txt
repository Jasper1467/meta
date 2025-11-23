add_library(meta_gui INTERFACE)
target_include_directories(meta_gui INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(meta_gui INTERFACE meta_base)  # depends on meta-base

# Optional: link to a backend like SDL2 or GLFW
# find_package(SDL2 REQUIRED)
# target_link_libraries(meta_gui INTERFACE SDL2::SDL2)

