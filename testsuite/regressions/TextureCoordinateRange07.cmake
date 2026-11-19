add_executable(CoinTextureCoordinateRange07Test TextureCoordinateRange07Test.cpp)
target_include_directories(CoinTextureCoordinateRange07Test PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
target_link_libraries(CoinTextureCoordinateRange07Test Coin ${COIN_TARGET_LINK_LIBRARIES})
foreach(mode repair legacy)
  add_test(NAME M07_07_${mode} COMMAND CoinTextureCoordinateRange07Test ${mode})
  set_tests_properties(M07_07_${mode} PROPERTIES LABELS texture-coordinates TIMEOUT 30)
endforeach()
