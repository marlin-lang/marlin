include(jscutils)

add_library(${PROJECT_NAME}.exec INTERFACE)
target_compile_features(${PROJECT_NAME}.exec INTERFACE cxx_std_17)

target_include_directories(${PROJECT_NAME}.exec INTERFACE include)

target_link_libraries(${PROJECT_NAME}.exec INTERFACE jscutils.jsc)
