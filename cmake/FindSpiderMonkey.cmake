# - Try to find the SpiderMonkey library
# Once done this will define
#  SPIDERMONKEY_FOUND - True if SpiderMonkey found.
#  SPIDERMONKEY_INCLUDE_DIRS - The SpiderMonkey include directories.
#  SPIDERMONKEY_LIBRARIES - The libraries needed to use SpiderMonkey
#  SPIDERMONKEY_DEFINITIONS - Compiler switches required for using SpiderMonkey
#  SPIDERMONKEY_VERSION - Version of the SpiderMonkey library

if(SPIDERMONKEY_INCLUDE_DIRS AND SPIDERMONKEY_LIBRARIES)
  set(SpiderMonkey_FIND_QUIETLY TRUE)
endif()

if(SPIDERMONKEY_ROOT)
  set(include_PATHS PATHS "${SPIDERMONKEY_ROOT}/include" NO_DEFAULT_PATH)
  set(library_PATHS PATHS "${SPIDERMONKEY_ROOT}/lib" NO_DEFAULT_PATH)
else()
  set(include_PATHS PATHS /usr/include /usr/local/include)
endif()

find_path(
  SPIDERMONKEY_INCLUDE_DIR
  NAMES jsapi.h
  ${include_PATHS}
  PATH_SUFFIXES
    js-1.7.0 js
)

# Everything below needs a real SPIDERMONKEY_INCLUDE_DIR to look at. When
# jsapi.h isn't found anywhere, find_path() leaves it set to
# "SPIDERMONKEY_INCLUDE_DIR-NOTFOUND" (a false value in CMake's boolean
# context), and none of the version detection below is meaningful -- skip
# straight to find_package_handle_standard_args() reporting NOT FOUND.
if(SPIDERMONKEY_INCLUDE_DIR)
  string(REGEX REPLACE "^.*js-([0-9]+[.]?[0-9]?[.]?[0-9]?).*" "\\1" SPIDERMONKEY_VERSION ${SPIDERMONKEY_INCLUDE_DIR})
  # dedicated workaround to find version string when not embedded in the includedir (<= 1.8.5)
  if(${SPIDERMONKEY_VERSION} STREQUAL ${SPIDERMONKEY_INCLUDE_DIR})
    # The path suffix didn't carry a version, so the regex above left
    # SPIDERMONKEY_VERSION as a plain copy of SPIDERMONKEY_INCLUDE_DIR --
    # useless (and actively wrong) as a library-name suffix in the
    # find_library() call below. Clear it so an unresolved version falls
    # back to the plain, unversioned names already in that NAMES list,
    # rather than searching for a library literally named e.g.
    # "mozjs-/usr/include/mozjs-102".
    set(SPIDERMONKEY_VERSION "")
    unset(VERS_FILE)
    if(EXISTS ${SPIDERMONKEY_INCLUDE_DIR}/jsversion.h)
      set(VERS_FILE "${SPIDERMONKEY_INCLUDE_DIR}/jsversion.h")
    elseif(EXISTS ${SPIDERMONKEY_INCLUDE_DIR}/jsconfig.h)
      set(VERS_FILE "${SPIDERMONKEY_INCLUDE_DIR}/jsconfig.h")
    else()
      message(WARNING "SpiderMonkey found at ${SPIDERMONKEY_INCLUDE_DIR}, but its version could not be determined (no js-<version> path suffix, and no jsversion.h/jsconfig.h to read JS_VERSION from). Falling back to unversioned library names.")
    endif()
    if(VERS_FILE)
      file(STRINGS ${VERS_FILE} VERS_STRING REGEX "#define JS_VERSION .*")
      if(VERS_STRING)
        string(REGEX REPLACE "#define JS_VERSION \(.*\)" "\\1" SPIDERMONKEY_VERSION ${VERS_STRING})
      else()
        message(WARNING "SpiderMonkey found at ${SPIDERMONKEY_INCLUDE_DIR}, but ${VERS_FILE} does not define JS_VERSION. Falling back to unversioned library names.")
      endif()
    endif()
  endif()

  find_library(
    SPIDERMONKEY_LIBRARY
    NAMES mozjs-${SPIDERMONKEY_VERSION} mozjs${SPIDERMONKEY_VERSION} mozjs js js${SPIDERMONKEY_VERSION}
    ${library_PATHS}
  )
endif()


include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set SPIDERMONKEY_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
  SpiderMonkey
  DEFAULT_MSG
  SPIDERMONKEY_LIBRARY
  SPIDERMONKEY_INCLUDE_DIR)

mark_as_advanced(SPIDERMONKEY_INCLUDE_DIR SPIDERMONKEY_LIBRARY)

set(SPIDERMONKEY_LIBRARIES ${SPIDERMONKEY_LIBRARY})
set(SPIDERMONKEY_INCLUDE_DIRS ${SPIDERMONKEY_INCLUDE_DIR})

