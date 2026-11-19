add_executable(CoinEngineNotifyRecoveryTest regressions/nodeengine/Notify.cpp)
target_link_libraries(CoinEngineNotifyRecoveryTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CoinEngineNotifyRecoveryTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
foreach(kind node engine)
  foreach(case input slave normal)
    add_test(NAME EngineNotifyRecovery_${kind}_${case}
      COMMAND CoinEngineNotifyRecoveryTest ${kind} ${case})
  endforeach()
endforeach()
