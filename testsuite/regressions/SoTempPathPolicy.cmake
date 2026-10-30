add_executable(SoTempPathPolicyTest
  reproducers/sotemppath-policy-preservation/repro.cpp)
target_link_libraries(SoTempPathPolicyTest
  Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(SoTempPathPolicyTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
add_test(NAME SoTempPathPolicy_nonempty_destination
  COMMAND SoTempPathPolicyTest nonempty-destination)
add_test(NAME SoTempPathPolicy_empty_destination
  COMMAND SoTempPathPolicyTest empty-destination)
add_test(NAME SoTempPathPolicy_ordinary_empty_destination
  COMMAND SoTempPathPolicyTest ordinary-empty-destination)
add_test(NAME SoTempPathPolicy_temporary_source
  COMMAND SoTempPathPolicyTest temporary-source)


if(NOT WIN32)
  add_executable(SoTempPathPolicyAllocationFailureTest
    reproducers/sotemppath-policy-preservation/allocation_failure.cpp)
  target_link_libraries(SoTempPathPolicyAllocationFailureTest
    Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(SoTempPathPolicyAllocationFailureTest PRIVATE
    ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
    ${COIN_TARGET_INCLUDE_DIRECTORIES})
  add_test(NAME SoTempPathPolicyAllocationFailure
    COMMAND SoTempPathPolicyAllocationFailureTest)
endif()
