add_library(meta_math INTERFACE)

target_include_directories(meta_math
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(meta_math
    INTERFACE
        meta_core
)
