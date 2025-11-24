if(META_BUILD_SHARED)
    set(META_BASE_TYPE SHARED)
else()
    set(META_BASE_TYPE STATIC)
endif()

add_library(meta_base ${META_BASE_TYPE}
    base.cpp
)

target_include_directories(meta_base PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/app
    ${CMAKE_CURRENT_SOURCE_DIR}/serialization
    ${CMAKE_CURRENT_SOURCE_DIR}/math
    ${CMAKE_CURRENT_SOURCE_DIR}/filesystem
    ${CMAKE_SOURCE_DIR}/external/magic_enum/include
)

