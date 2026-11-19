add_executable(CoinTextureCoordinateM0804Test TextureCoordinateM0804Test.cpp)
target_include_directories(CoinTextureCoordinateM0804Test PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
target_link_libraries(CoinTextureCoordinateM0804Test Coin ${COIN_TARGET_LINK_LIBRARIES})
foreach(mode repair legacy)
  add_test(NAME M08_04_${mode} COMMAND CoinTextureCoordinateM0804Test ${mode})
  set_tests_properties(M08_04_${mode} PROPERTIES LABELS texture-coordinates TIMEOUT 30)
endforeach()
