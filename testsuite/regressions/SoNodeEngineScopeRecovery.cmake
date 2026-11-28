add_executable(CoinNodeEngineMultipleActiveTest regressions/nodeengine/MultipleActive.cpp)
target_link_libraries(CoinNodeEngineMultipleActiveTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CoinNodeEngineMultipleActiveTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
foreach(operation add remove replace)
  foreach(depth 2 3)
    foreach(receiver active external nested)
      foreach(slaves separate shared)
        add_test(NAME NodeEngineMultipleActive_${operation}_${depth}_${receiver}_${slaves}
          COMMAND CoinNodeEngineMultipleActiveTest ${operation} ${depth} ${receiver} ${slaves})
        set_tests_properties(NodeEngineMultipleActive_${operation}_${depth}_${receiver}_${slaves}
          PROPERTIES TIMEOUT 15)
      endforeach()
    endforeach()
  endforeach()
endforeach()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND COIN_BUILD_SHARED_LIBS)
  add_executable(CoinNodeEngineCaughtResumeAllocationTest regressions/nodeengine/CaughtResumeAllocation.cpp)
  target_link_libraries(CoinNodeEngineCaughtResumeAllocationTest Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(CoinNodeEngineCaughtResumeAllocationTest PRIVATE
    ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
  foreach(ordinal RANGE 0 1)
    foreach(failure once persistent)
      foreach(evaluation normal recursive chained)
        add_test(NAME NodeEngineCaughtResumeAllocation_${ordinal}_${failure}_${evaluation}
          COMMAND CoinNodeEngineCaughtResumeAllocationTest ${ordinal} ${failure} ${evaluation})
        set_tests_properties(NodeEngineCaughtResumeAllocation_${ordinal}_${failure}_${evaluation}
          PROPERTIES TIMEOUT 15)
      endforeach()
    endforeach()
  endforeach()
endif()

add_executable(CoinNodeEngineMixedActiveTest regressions/nodeengine/MixedActive.cpp)
target_link_libraries(CoinNodeEngineMixedActiveTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CoinNodeEngineMixedActiveTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
foreach(operation add remove replace)
  foreach(outer node engine)
    foreach(receiver active external)
      add_test(NAME NodeEngineMixedActive_${operation}_${outer}_${receiver}
        COMMAND CoinNodeEngineMixedActiveTest ${operation} ${outer} ${receiver})
      set_tests_properties(NodeEngineMixedActive_${operation}_${outer}_${receiver}
        PROPERTIES TIMEOUT 15)
    endforeach()
  endforeach()
endforeach()
