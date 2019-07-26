add_library(${PROJECT_NAME}.parse src/interpreter.cpp src/scanner.cpp)
set_target_properties(${PROJECT_NAME}.parse PROPERTIES OUTPUT_NAME marlinparse)
target_compile_features(${PROJECT_NAME}.parse PUBLIC cxx_std_17)

target_include_directories(${PROJECT_NAME}.parse
                           INTERFACE include
                           PRIVATE include/marlin/details)

target_link_libraries(${PROJECT_NAME}.parse ${PROJECT_NAME}.ast)
