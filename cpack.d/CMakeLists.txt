if (CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "x86_64")
  set(DEB_ARCH "amd64")
endif()

file(GLOB CONFIGS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cmake.in)
dump_variable(
CONFIGS
)

foreach(FILE ${CONFIGS})
  string(REGEX REPLACE ".cmake.in$" ".cmake" CFG ${FILE})
  configure_file(${FILE} ${CFG} @ONLY)
endforeach()
