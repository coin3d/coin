if(HAVE_NODEKITS)
  add_executable(SoNodeKitPathTest SoNodeKitPathTest.cpp)
  target_link_libraries(SoNodeKitPathTest Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(SoNodeKitPathTest PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/include
    ${COIN_TARGET_INCLUDE_DIRECTORIES})
  add_test(NAME SoNodeKitPathBehavior COMMAND SoNodeKitPathTest)
endif()

if(HAVE_MANIPULATORS)
  add_executable(SoManipNestedKitTest SoManipNestedKitTest.cpp)
  target_link_libraries(SoManipNestedKitTest Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(SoManipNestedKitTest PRIVATE
    ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
    ${COIN_TARGET_INCLUDE_DIRECTORIES})
  add_test(NAME SoManipNestedKit COMMAND SoManipNestedKitTest)
endif()
