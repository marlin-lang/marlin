add_executable(${PROJECT_NAME}.cli.main marlin.cpp)
set_target_properties(${PROJECT_NAME}.cli.main PROPERTIES OUTPUT_NAME marlin)
target_link_libraries(${PROJECT_NAME}.cli.main ${PROJECT_NAME}.marlin)
