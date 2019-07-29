include(FetchContent)

fetchcontent_declare(jscutils
                     GIT_REPOSITORY
                     https://github.com/beanandbean/jscutils.git
                     GIT_TAG
                     991ab78d8655295710ea8d1e31c25ca1220437ce)

fetchcontent_getproperties(jscutils)
if(NOT jscutils_POPULATED)
  message("-- Populating jscutils")
  fetchcontent_populate(jscutils)
  add_subdirectory(${jscutils_SOURCE_DIR} ${jscutils_BINARY_DIR}
                   EXCLUDE_FROM_ALL)
endif()
