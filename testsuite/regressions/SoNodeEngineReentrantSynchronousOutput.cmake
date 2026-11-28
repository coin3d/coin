add_executable(CoinNodeEngineReentrantSynchronousOutputTest
  regressions/nodeengine/ReentrantSynchronousOutput.cpp)
target_link_libraries(CoinNodeEngineReentrantSynchronousOutputTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CoinNodeEngineReentrantSynchronousOutputTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
foreach(mode normal mixed throw)
  add_test(NAME NodeEngineReentrantSynchronousOutput_${mode}
    COMMAND CoinNodeEngineReentrantSynchronousOutputTest ${mode})
  set_tests_properties(NodeEngineReentrantSynchronousOutput_${mode} PROPERTIES TIMEOUT 15)
endforeach()
