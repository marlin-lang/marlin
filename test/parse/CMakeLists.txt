add_test_files(file_tests.cpp numeric_tests.cpp string_tests.cpp)

add_custom_target(${PROJECT_NAME}.test.parse.copy_files ALL
                  DEPENDS ${MARLIN_TEST_DIR}/existent_file.mar)
add_custom_command(OUTPUT ${MARLIN_TEST_DIR}/existent_file.mar
                   COMMAND ${CMAKE_COMMAND} -E copy
                           ${CMAKE_CURRENT_SOURCE_DIR}/existent_file.mar
                           ${MARLIN_TEST_DIR}/existent_file.mar
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/existent_file.mar)
