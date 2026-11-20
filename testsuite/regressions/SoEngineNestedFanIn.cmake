add_executable(CoinEngineNestedFanInTest regressions/nodeengine/NestedFanIn.cpp)
target_link_libraries(CoinEngineNestedFanInTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CoinEngineNestedFanInTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
foreach(source engine node)
  foreach(other engine node)
    add_test(NAME EngineNestedFanIn_${source}_${other}
      COMMAND CoinEngineNestedFanInTest ${source} ${other})
    set_tests_properties(EngineNestedFanIn_${source}_${other} PROPERTIES TIMEOUT 15)
  endforeach()
endforeach()
