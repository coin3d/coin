# Compile the production allocator in isolation to inject malloc failures.
add_executable(CcMemallocFailureTest CcMemallocFailureTest.cpp)
target_compile_definitions(CcMemallocFailureTest PRIVATE
  COIN_INTERNAL HAVE_CONFIG_H COIN_BUILDING_COIN)
target_include_directories(CcMemallocFailureTest PRIVATE
  ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}/src ${PROJECT_BINARY_DIR}/include)
add_test(NAME CcMemallocFailure COMMAND CcMemallocFailureTest)
