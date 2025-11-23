add_executable(test_main
    test_main.cpp
)

# Add project root so that #include <meta/...> works
target_include_directories(test_main PRIVATE
    ${CMAKE_SOURCE_DIR}
)

# meta_base is header-only interface library
target_link_libraries(test_main PRIVATE meta_base)

set_target_properties(test_main PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

