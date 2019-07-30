cmake_minimum_required(VERSION 3.14)

if(DEFINED PROJECT_NAME)
  set(SUBPROJECT ON)
endif()

project(marlin LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# common dependency of remaining modules
add_subdirectory(ast)

add_subdirectory(parse)
add_subdirectory(lint)
add_subdirectory(format)
add_subdirectory(exec)

add_library(${PROJECT_NAME}.marlin INTERFACE)
target_link_libraries(${PROJECT_NAME}.marlin
                      INTERFACE ${PROJECT_NAME}.ast
                                ${PROJECT_NAME}.parse
                                ${PROJECT_NAME}.lint
                                ${PROJECT_NAME}.format
                                ${PROJECT_NAME}.exec)

if(NOT SUBPROJECT)
  add_subdirectory(cli)

  add_subdirectory(test)
  enable_testing()
endif()
