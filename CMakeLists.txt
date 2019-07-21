cmake_minimum_required(VERSION 3.14)

if(DEFINED PROJECT_NAME)
  set(SUBPROJECT ON)
endif()

project(marlin LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(exec)

add_library(${PROJECT_NAME}.marlin INTERFACE)
target_link_libraries(${PROJECT_NAME}.marlin INTERFACE ${PROJECT_NAME}.exec)

if(NOT SUBPROJECT)
  add_executable(marlin_main main.cpp)
  set_target_properties(marlin_main PROPERTIES OUTPUT_NAME marlin)
  target_link_libraries(marlin_main ${PROJECT_NAME}.marlin)
endif()
