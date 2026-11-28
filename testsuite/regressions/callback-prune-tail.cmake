add_executable(CallbackPruneTailTest CallbackPruneTailTest.cpp)
target_link_libraries(CallbackPruneTailTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CallbackPruneTailTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
add_test(NAME CallbackPruneTail COMMAND CallbackPruneTailTest)
