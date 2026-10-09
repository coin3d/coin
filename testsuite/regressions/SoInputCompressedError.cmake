if(UNIX)
  add_executable(CoinSoInputCompressedErrorTest CoinSoInputCompressedErrorTest.cpp)
  target_link_libraries(CoinSoInputCompressedErrorTest Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(CoinSoInputCompressedErrorTest PRIVATE
    ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include)
  add_test(NAME CoinSoInputCompressedError COMMAND CoinSoInputCompressedErrorTest)
endif()
