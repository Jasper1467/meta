add_executable(example_main example_main.cpp)

# Include project root so that <meta/...> works
target_include_directories(example_main PRIVATE
    ${CMAKE_SOURCE_DIR}   # <-- parent of meta/
)

# Link header-only library
target_link_libraries(example_main PRIVATE meta_base)

# Optional: link GUI if needed
# target_link_libraries(example_main PRIVATE meta_gui)

# C++ standard
set_target_properties(example_main PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

