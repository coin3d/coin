add_executable(SbSmallMapFailureTest SbSmallMapFailureTest.cpp)
target_compile_definitions(SbSmallMapFailureTest PRIVATE COIN_INTERNAL HAVE_CONFIG_H)
target_include_directories(SbSmallMapFailureTest PRIVATE
  ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}/src ${PROJECT_BINARY_DIR}/include)
add_test(NAME SbSmallMapFailure COMMAND SbSmallMapFailureTest)
