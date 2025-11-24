# Add executables
add_executable(example_gui example_gui.cpp)
add_executable(example_cli example_cli.cpp)

# Include project root so that <meta/...> works
target_include_directories(example_gui PRIVATE
    ${CMAKE_SOURCE_DIR}
)
target_include_directories(example_cli PRIVATE
    ${CMAKE_SOURCE_DIR}
)

# Link header-only library
target_link_libraries(example_gui PRIVATE meta_base meta_gui)
target_link_libraries(example_cli PRIVATE meta_base)
