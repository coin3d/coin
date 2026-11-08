add_executable(CcRbptreeParallelTest CcRbptreeParallelTest.cpp)
target_link_libraries(CcRbptreeParallelTest Coin Threads::Threads ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CcRbptreeParallelTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
add_test(NAME CcRbptreeParallel COMMAND CcRbptreeParallelTest)
set_tests_properties(CcRbptreeParallel PROPERTIES TIMEOUT 30)
