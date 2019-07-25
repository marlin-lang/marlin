include(FetchContent)

fetchcontent_declare(jscutils
                     GIT_REPOSITORY
                     https://github.com/beanandbean/jscutils.git
                     GIT_TAG
                     6febf2753066fa7c8b9820a8874d73c6b37e7082
                     GIT_SHALLOW
                     ON)

fetchcontent_getproperties(jscutils)
if(NOT jscutils_POPULATED)
  message("-- Populating jscutils")
  fetchcontent_populate(jscutils)
  add_subdirectory(${jscutils_SOURCE_DIR} ${jscutils_BINARY_DIR}
                   EXCLUDE_FROM_ALL)
endif()
