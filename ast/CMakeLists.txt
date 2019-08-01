include(jscutils)

add_library(${PROJECT_NAME}.ast src/base.cpp src/node.cpp)
target_compile_features(${PROJECT_NAME}.ast PUBLIC cxx_std_17)

target_link_libraries(${PROJECT_NAME}.ast jscutils.jsast)

target_include_directories(${PROJECT_NAME}.ast
                           INTERFACE include
                           PRIVATE include/marlin/details)
