# Real node traversal with a recording element; requires no GL context.
add_executable(CoinSoDepthBufferTest SoDepthBufferTest.cpp)
target_link_libraries(CoinSoDepthBufferTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CoinSoDepthBufferTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
foreach(depth_case ignored_fields_inherit_independently ignored_range_is_read_only
                   shared_ignored_node_and_separator_restore explicit_function_and_range_values
                   raw_ranges_are_preserved)
  add_test(NAME SoDepthBuffer_${depth_case}
    COMMAND CoinSoDepthBufferTest --run_test=depth_${depth_case})
  set_tests_properties(SoDepthBuffer_${depth_case} PROPERTIES LABELS "legacy-gl;depth-buffer")
endforeach()
