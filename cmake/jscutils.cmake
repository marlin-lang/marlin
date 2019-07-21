include(FetchContent)

fetchcontent_declare(jscutils
                     GIT_REPOSITORY
                     https://github.com/beanandbean/jscutils.git
                     GIT_TAG
                     a8b7c1bcc91ec13544666397909e3e152a98f6d6
                     GIT_SHALLOW
                     ON)

fetchcontent_getproperties(jscutils)
if(NOT jscutils_POPULATED)
  message("-- Downloading jscutils")
  fetchcontent_populate(jscutils)
  add_subdirectory(${jscutils_SOURCE_DIR} ${jscutils_BINARY_DIR}
                   EXCLUDE_FROM_ALL)
endif()
