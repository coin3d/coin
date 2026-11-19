add_executable(CoinEngineMutedFanInTest regressions/nodeengine/MutedFanIn.cpp)
target_link_libraries(CoinEngineMutedFanInTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CoinEngineMutedFanInTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
foreach(source engine node)
  foreach(other engine node)
    foreach(mode nested outside disabled)
      add_test(NAME EngineMutedFanIn_${source}_${other}_${mode}
        COMMAND CoinEngineMutedFanInTest ${source} ${other} ${mode})
      set_tests_properties(EngineMutedFanIn_${source}_${other}_${mode} PROPERTIES TIMEOUT 15)
    endforeach()
  endforeach()
endforeach()
