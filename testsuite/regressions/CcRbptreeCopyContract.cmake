add_executable(CoinRbptreeCopyContractTest CcRbptreeCopyContractTest.cpp)
target_link_libraries(CoinRbptreeCopyContractTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CoinRbptreeCopyContractTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
add_test(NAME RbptreeCopyContractCpp COMMAND CoinRbptreeCopyContractTest)

add_executable(CoinRbptreeCContractTest CcRbptreeCContractTest.c)
target_link_libraries(CoinRbptreeCContractTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CoinRbptreeCContractTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
add_test(NAME RbptreeCopyContractC COMMAND CoinRbptreeCContractTest)
