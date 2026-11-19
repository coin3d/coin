add_executable(CoinNodeEngineSynchronousConnectionsTest
  regressions/nodeengine/SynchronousConnections.cpp)
target_link_libraries(CoinNodeEngineSynchronousConnectionsTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CoinNodeEngineSynchronousConnectionsTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
foreach(operation add remove replace)
  foreach(order before after)
    foreach(flags normal mixed throw)
      add_test(NAME NodeEngineSynchronousConnections_${operation}_${order}_${flags}
        COMMAND CoinNodeEngineSynchronousConnectionsTest ${operation} ${order} ${flags})
      set_tests_properties(NodeEngineSynchronousConnections_${operation}_${order}_${flags}
        PROPERTIES TIMEOUT 15)
    endforeach()
  endforeach()
endforeach()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND COIN_BUILD_SHARED_LIBS)
  add_executable(CoinNodeEngineSynchronousConnectionsAllocationTest
    regressions/nodeengine/SynchronousConnectionsAllocation.cpp)
  target_link_libraries(CoinNodeEngineSynchronousConnectionsAllocationTest Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(CoinNodeEngineSynchronousConnectionsAllocationTest PRIVATE
    ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
  foreach(ordinal RANGE 0 1)
    add_test(NAME NodeEngineSynchronousConnections_allocation_${ordinal}
      COMMAND CoinNodeEngineSynchronousConnectionsAllocationTest ${ordinal})
    set_tests_properties(NodeEngineSynchronousConnections_allocation_${ordinal}
      PROPERTIES TIMEOUT 15)
  endforeach()
endif()
