add_library(meta_serialization INTERFACE)

target_include_directories(meta_serialization INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(meta_serialization INTERFACE
    meta_core
)

