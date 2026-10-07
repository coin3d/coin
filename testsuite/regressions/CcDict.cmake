# Compile the production dictionary into an isolated executable so bucket
# allocation failures can be injected without changing the library's API.
add_executable(CcDictResizeTest CcDictResizeTest.cpp)
target_compile_definitions(CcDictResizeTest PRIVATE COIN_INTERNAL HAVE_CONFIG_H)
target_link_libraries(CcDictResizeTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CcDictResizeTest PRIVATE
  ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}/src ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
add_test(NAME CcDictResizeRelink COMMAND CcDictResizeTest relink)
add_test(NAME CcDictResizeAllocationFailure COMMAND CcDictResizeTest failure)
add_test(NAME CcDictNumericLoadfactor COMMAND CcDictResizeTest numeric)
add_test(NAME CcDictAllocationFailures COMMAND CcDictResizeTest oom)
add_test(NAME CcDictApplyRemoveCurrent COMMAND CcDictResizeTest apply)
add_test(NAME CcDictHashException COMMAND CcDictResizeTest exception)
add_test(NAME CcDictCapacityBoundary COMMAND CcDictResizeTest capacity)
add_test(NAME CcDictHashRebuildAllocationFailure COMMAND CcDictResizeTest rebuild-oom)
