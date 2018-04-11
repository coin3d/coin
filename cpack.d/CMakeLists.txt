find_program(HG_LOCATION NAMES hg)
if (HG_LOCATION)
  # calculates the build number as the number of commits on the current branch
  execute_process(
    COMMAND ${HG_LOCATION} id --num --rev tip
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    OUTPUT_VARIABLE PACKAGE_BUILD_NUMBER
  )
  string(REGEX REPLACE "\n$" "" PACKAGE_BUILD_NUMBER "${PACKAGE_BUILD_NUMBER}")
  message(STATUS "PACKAGE_BUILD_NUMBER set to ${PACKAGE_BUILD_NUMBER}")
else()
  set(PACKAGE_BUILD_NUMBER 1)
  message(WARNING "Mercurial not found, PACKAGE_BUILD_NUMBER set to ${PACKAGE_BUILD_NUMBER}")
endif()
  
file(GLOB CONFIGS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cmake.in)

foreach(FILE ${CONFIGS})
  string(REGEX REPLACE ".cmake.in$" ".cmake" CFG ${FILE})
  configure_file(${FILE} ${CFG} @ONLY)
endforeach()
