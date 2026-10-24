if(HAVE_VRML97)
  add_executable(VRMLScriptNotificationTest VRMLScriptNotificationTest.cpp)
  target_link_libraries(VRMLScriptNotificationTest Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(VRMLScriptNotificationTest PRIVATE ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
  # The baseline may assert on a fieldless notification. Isolate each case.
  foreach(script_case touch fields)
    add_test(NAME VRMLScriptNotification_${script_case} COMMAND VRMLScriptNotificationTest ${script_case})
  endforeach()
endif()
