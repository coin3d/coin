add_executable(CallbackCurrentNodeTest CallbackCurrentNodeTest.cpp)
target_link_libraries(CallbackCurrentNodeTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CallbackCurrentNodeTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
foreach(callback_case tree nested nested-post nested-exception)
  add_test(NAME CallbackCurrentNode_${callback_case}
    COMMAND CallbackCurrentNodeTest ${callback_case})
endforeach()
