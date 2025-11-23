add_library(meta_app INTERFACE)
target_include_directories(meta_app INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(meta_app INTERFACE meta_core meta_serialization meta_filesystem)

