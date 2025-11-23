add_library(meta_base INTERFACE)
target_include_directories(meta_base INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/core
                                                  ${CMAKE_CURRENT_SOURCE_DIR}/app
                                                  ${CMAKE_CURRENT_SOURCE_DIR}/serialization
                                                ${CMAKE_CURRENT_SOURCE_DIR}/math
                                                                                                                                                                                                        ${CMAKE_CURRENT_SOURCE_DIR}/filesystem
)

