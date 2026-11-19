add_executable(CoinTextureCoordinateM0802Test TextureCoordinateM0802Test.cpp)
target_include_directories(CoinTextureCoordinateM0802Test PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
target_link_libraries(CoinTextureCoordinateM0802Test Coin ${COIN_TARGET_LINK_LIBRARIES})
foreach(mode repair legacy)
  add_test(NAME M08_02_${mode} COMMAND CoinTextureCoordinateM0802Test ${mode})
  set_tests_properties(M08_02_${mode} PROPERTIES LABELS texture-coordinates TIMEOUT 30)
endforeach()
