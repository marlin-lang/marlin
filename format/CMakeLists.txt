add_library(${PROJECT_NAME}.format INTERFACE)
target_compile_features(${PROJECT_NAME}.format INTERFACE cxx_std_17)

target_include_directories(${PROJECT_NAME}.format INTERFACE include)

target_link_libraries(${PROJECT_NAME}.format INTERFACE ${PROJECT_NAME}.ast)
