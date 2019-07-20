include(FetchContent)

fetchcontent_declare(jscutils
                     GIT_REPOSITORY
                     https://github.com/beanandbean/jscutils.git
                     GIT_TAG
                     50c56bb0f551574652d786cf333b48f500a1b552
                     GIT_SHALLOW
                     ON)

fetchcontent_getproperties(jscutils)
if(NOT jscutils_POPULATED)
  message("-- Downloading jscutils")
  fetchcontent_populate(jscutils)
  add_subdirectory(${jscutils_SOURCE_DIR} ${jscutils_BINARY_DIR}
                   EXCLUDE_FROM_ALL)
endif()

add_library(${PROJECT_NAME}.exec INTERFACE)
target_compile_features(${PROJECT_NAME}.exec INTERFACE cxx_std_17)

target_include_directories(${PROJECT_NAME}.exec INTERFACE include)

target_link_libraries(${PROJECT_NAME}.exec INTERFACE jscutils.jsc)
