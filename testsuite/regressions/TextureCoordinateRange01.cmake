add_executable(CoinTextureCoordinateRange01Test TextureCoordinateRange01Test.cpp)
target_include_directories(CoinTextureCoordinateRange01Test PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
target_link_libraries(CoinTextureCoordinateRange01Test Coin ${COIN_TARGET_LINK_LIBRARIES})
foreach(mode repair legacy)
  add_test(NAME M07_01_${mode} COMMAND CoinTextureCoordinateRange01Test ${mode})
  set_tests_properties(M07_01_${mode} PROPERTIES LABELS texture-coordinates TIMEOUT 30)
endforeach()
