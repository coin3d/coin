# Executable allocation interposition is supported by the Linux shared build.
if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND COIN_BUILD_SHARED_LIBS)
  add_executable(CoinEngineOutputPrepareTest regressions/nodeengine/Prepare.cpp)
  target_link_libraries(CoinEngineOutputPrepareTest Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(CoinEngineOutputPrepareTest PRIVATE
    ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
  foreach(ordinal RANGE 0 8)
    add_test(NAME EngineOutputPrepare_${ordinal} COMMAND CoinEngineOutputPrepareTest ${ordinal})
  endforeach()
endif()
