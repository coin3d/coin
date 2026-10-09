if(HAVE_NODEKITS)
  add_executable(SoNodeKitPathTest SoNodeKitPathTest.cpp)
  target_link_libraries(SoNodeKitPathTest Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(SoNodeKitPathTest PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/include
    ${COIN_TARGET_INCLUDE_DIRECTORIES})
  add_test(NAME SoNodeKitPathBehavior COMMAND SoNodeKitPathTest)
else()
  add_executable(SoNodeKitPathNoKitsTest SoNodeKitPathNoKitsTest.cpp)
  target_link_libraries(SoNodeKitPathNoKitsTest Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(SoNodeKitPathNoKitsTest PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/include
    ${COIN_TARGET_INCLUDE_DIRECTORIES})
  add_test(NAME SoNodeKitPathNoKits COMMAND SoNodeKitPathNoKitsTest)
endif()

if(HAVE_MANIPULATORS)
  add_executable(SoManipNestedKitTest SoManipNestedKitTest.cpp)
  target_link_libraries(SoManipNestedKitTest Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(SoManipNestedKitTest PRIVATE
    ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
    ${COIN_TARGET_INCLUDE_DIRECTORIES})
  add_test(NAME SoManipNestedKit COMMAND SoManipNestedKitTest)
endif()
