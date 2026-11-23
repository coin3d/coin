if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND COIN_BUILD_SHARED_LIBS)
  add_executable(CoinStdFdsFdopenFailureTest CoinStdFdsFdopenFailureTest.cpp)
  target_compile_definitions(CoinStdFdsFdopenFailureTest PRIVATE COIN_INTERNAL)
  target_link_libraries(CoinStdFdsFdopenFailureTest Coin ${COIN_TARGET_LINK_LIBRARIES} ${CMAKE_DL_LIBS})
  target_include_directories(CoinStdFdsFdopenFailureTest PRIVATE
    ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include)
  add_test(NAME CoinStdFdsFdopenFailure COMMAND CoinStdFdsFdopenFailureTest)
endif()
