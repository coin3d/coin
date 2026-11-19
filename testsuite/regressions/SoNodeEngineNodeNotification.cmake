if(HAVE_VRML97)
  add_executable(CoinNodeEngineNodeNotificationTest regressions/nodeengine/NodeNotification.cpp)
  target_link_libraries(CoinNodeEngineNodeNotificationTest Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(CoinNodeEngineNodeNotificationTest PRIVATE
    ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
  foreach(case field touch missing disabled self feedback type)
    add_test(NAME NodeEngineNodeNotification_${case}
      COMMAND CoinNodeEngineNodeNotificationTest ${case})
    set_tests_properties(NodeEngineNodeNotification_${case} PROPERTIES TIMEOUT 15)
  endforeach()
endif()
