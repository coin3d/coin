add_executable(CoinTriangleTextureInterpolationTest TriangleTextureInterpolationTest.cpp)
target_include_directories(CoinTriangleTextureInterpolationTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
target_link_libraries(CoinTriangleTextureInterpolationTest Coin ${COIN_TARGET_LINK_LIBRARIES})
foreach(mode repair legacy)
  add_test(NAME M09_triangle_${mode} COMMAND CoinTriangleTextureInterpolationTest ${mode})
  set_tests_properties(M09_triangle_${mode} PROPERTIES LABELS texture-coordinates TIMEOUT 30)
endforeach()
