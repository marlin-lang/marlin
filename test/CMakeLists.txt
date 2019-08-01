include(catch2)

set(MARLIN_TEST_DIR "${CMAKE_CURRENT_BINARY_DIR}")
list(APPEND SOURCES main.cpp)
macro(add_test_files)
  set(__ARGN__ "${ARGN}")
  foreach(ARG IN LISTS __ARGN__)
    if(IS_ABSOLUTE "${ARG}")
      list(APPEND SOURCES "${ARG}")
    else()
      list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${ARG}")
    endif()
  endforeach()
  set(SOURCES "${SOURCES}" PARENT_SCOPE)
endmacro()

add_subdirectory(ast)
add_subdirectory(parse)
add_subdirectory(exec)
add_subdirectory(format)

add_executable(${PROJECT_NAME}.test ${SOURCES})
set_target_properties(${PROJECT_NAME}.test PROPERTIES OUTPUT_NAME test_marlin)
target_link_libraries(${PROJECT_NAME}.test ${PROJECT_NAME}.marlin
                      Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(${PROJECT_NAME}.test)
