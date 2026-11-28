add_executable(CoinEngineEvaluateRecoveryTest regressions/nodeengine/Evaluate.cpp)
target_link_libraries(CoinEngineEvaluateRecoveryTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CoinEngineEvaluateRecoveryTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
foreach(kind node engine)
  foreach(case normal throw shared shared-throw dirty)
    add_test(NAME EngineEvaluateRecovery_${kind}_${case}
      COMMAND CoinEngineEvaluateRecoveryTest ${kind} ${case})
  endforeach()
endforeach()
if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND COIN_BUILD_SHARED_LIBS)
  add_executable(CoinEngineEvaluateAllocationTest regressions/nodeengine/EvaluateAllocation.cpp)
  target_link_libraries(CoinEngineEvaluateAllocationTest Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(CoinEngineEvaluateAllocationTest PRIVATE
    ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
  foreach(kind node engine)
    foreach(ordinal RANGE 0 12)
      add_test(NAME EngineEvaluateAllocation_${kind}_${ordinal}
        COMMAND CoinEngineEvaluateAllocationTest ${kind} ${ordinal})
    endforeach()
  endforeach()
endif()
