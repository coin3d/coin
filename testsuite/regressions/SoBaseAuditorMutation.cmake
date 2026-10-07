add_executable(SoBaseAuditorMutationTest SoBaseAuditorMutationTest.cpp)
target_link_libraries(SoBaseAuditorMutationTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(SoBaseAuditorMutationTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
add_test(NAME SoBaseAuditorMutation COMMAND SoBaseAuditorMutationTest)
