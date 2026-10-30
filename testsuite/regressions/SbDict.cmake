add_executable(SbDictApiTest SbDictApiTest.cpp)
target_link_libraries(SbDictApiTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(SbDictApiTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
add_test(NAME SbDictApi COMMAND SbDictApiTest)

# Compile the private dictionary locally so this comparison does not require
# private cc_dict symbols to be exported by a Windows DLL.
add_executable(SbDictDifferentialTest SbDictDifferentialTest.cpp
  ${PROJECT_SOURCE_DIR}/src/base/dict.cpp)
target_compile_definitions(SbDictDifferentialTest PRIVATE COIN_INTERNAL HAVE_CONFIG_H)
target_link_libraries(SbDictDifferentialTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(SbDictDifferentialTest PRIVATE
  ${PROJECT_SOURCE_DIR}/src ${PROJECT_BINARY_DIR}/src
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
add_test(NAME SbDictDifferential COMMAND SbDictDifferentialTest)

# Executable allocation interposition and fork/SIGABRT probes are POSIX tests.
if(NOT WIN32)
  add_executable(SbDictListFailureTest SbDictListFailureTest.cpp)
  target_link_libraries(SbDictListFailureTest Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(SbDictListFailureTest PRIVATE
    ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
    ${COIN_TARGET_INCLUDE_DIRECTORIES})
  add_test(NAME SbDictListAllocationFailure COMMAND SbDictListFailureTest)

  add_executable(SbDictOomTest SbDictOomTest.cpp)
  target_compile_definitions(SbDictOomTest PRIVATE COIN_INTERNAL HAVE_CONFIG_H COIN_BUILDING_COIN)
  target_link_libraries(SbDictOomTest Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(SbDictOomTest PRIVATE
    ${PROJECT_SOURCE_DIR}/src ${PROJECT_BINARY_DIR}/src
    ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
    ${COIN_TARGET_INCLUDE_DIRECTORIES})
  add_test(NAME SbDictMandatoryOom COMMAND SbDictOomTest)
endif()
