include(FetchContent)

fetchcontent_declare(jscutils
                     GIT_REPOSITORY
                     https://github.com/beanandbean/jscutils.git
                     GIT_TAG
                     ed042d5788d3893dfad28c3e6eae9769f14e4d1a)

fetchcontent_getproperties(jscutils)
if(NOT jscutils_POPULATED)
  message("-- Populating jscutils")
  fetchcontent_populate(jscutils)
  add_subdirectory(${jscutils_SOURCE_DIR} ${jscutils_BINARY_DIR}
                   EXCLUDE_FROM_ALL)
endif()
