include(jscutils)

add_library(${PROJECT_NAME}.ast INTERFACE)
target_compile_features(${PROJECT_NAME}.ast INTERFACE cxx_std_17)

target_link_libraries(${PROJECT_NAME}.ast INTERFACE jscutils.jsast)

target_include_directories(${PROJECT_NAME}.ast INTERFACE include)
