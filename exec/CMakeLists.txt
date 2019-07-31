include(jscutils)

add_library(${PROJECT_NAME}.exec src/stacktrace.cpp)
target_compile_features(${PROJECT_NAME}.exec PUBLIC cxx_std_17)

target_include_directories(${PROJECT_NAME}.exec
                           INTERFACE include
                           PRIVATE include/marlin/details)

target_link_libraries(${PROJECT_NAME}.exec
                      jscutils.jsc
                      jscutils.jsast
                      ${PROJECT_NAME}.ast)
