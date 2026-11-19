add_executable(CoinTextureCoordinateM0801Test TextureCoordinateM0801Test.cpp)
target_include_directories(CoinTextureCoordinateM0801Test PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
target_link_libraries(CoinTextureCoordinateM0801Test Coin ${COIN_TARGET_LINK_LIBRARIES})
foreach(mode repair legacy)
  add_test(NAME M08_01_${mode} COMMAND CoinTextureCoordinateM0801Test ${mode})
  set_tests_properties(M08_01_${mode} PROPERTIES LABELS texture-coordinates TIMEOUT 30)
endforeach()
