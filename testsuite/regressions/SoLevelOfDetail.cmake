# Issue #374: path traversal and bounding-box cache semantics without GL.
add_executable(CoinLevelOfDetailBoundingBoxTest SoLevelOfDetailBoundingBoxTest.cpp)
target_link_libraries(CoinLevelOfDetailBoundingBoxTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CoinLevelOfDetailBoundingBoxTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
add_test(NAME LevelOfDetailBoundingBox COMMAND CoinLevelOfDetailBoundingBoxTest)
