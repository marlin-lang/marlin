cmake_minimum_required(VERSION 3.14)

if(DEFINED PROJECT_NAME)
  set(SUBPROJECT ON)
endif()

project(marlin LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(exec)

add_library(${PROJECT_NAME}.marlin INTERFACE)
target_link_libraries(${PROJECT_NAME}.marlin INTERFACE ${PRPJECT_NAME}.exec)

if(NOT SUBPROJECT)
  add_executable(marlin_main main.cpp)
  set_target_properties(marlin_main PROPERTIES OUTPUT_NAME marlin)
endif()
