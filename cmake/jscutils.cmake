include(FetchContent)

fetchcontent_declare(jscutils
                     GIT_REPOSITORY
                     https://github.com/beanandbean/jscutils.git
                     GIT_TAG
                     752432e874bfb92cd3007d88d2bddb5abd49e88a
                     GIT_SHALLOW
                     ON)

fetchcontent_getproperties(jscutils)
if(NOT jscutils_POPULATED)
  message("-- Populating jscutils")
  fetchcontent_populate(jscutils)
  add_subdirectory(${jscutils_SOURCE_DIR} ${jscutils_BINARY_DIR}
                   EXCLUDE_FROM_ALL)
endif()
