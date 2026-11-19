add_executable(CoinTextureCoordinateM0803Test TextureCoordinateM0803Test.cpp)
target_include_directories(CoinTextureCoordinateM0803Test PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
target_link_libraries(CoinTextureCoordinateM0803Test Coin ${COIN_TARGET_LINK_LIBRARIES})
foreach(mode repair legacy)
  add_test(NAME M08_03_${mode} COMMAND CoinTextureCoordinateM0803Test ${mode})
  set_tests_properties(M08_03_${mode} PROPERTIES LABELS texture-coordinates TIMEOUT 30)
endforeach()
