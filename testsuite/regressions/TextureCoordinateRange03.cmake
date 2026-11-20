add_executable(CoinTextureCoordinateRange03Test TextureCoordinateRange03Test.cpp)
target_include_directories(CoinTextureCoordinateRange03Test PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
target_link_libraries(CoinTextureCoordinateRange03Test Coin ${COIN_TARGET_LINK_LIBRARIES})
foreach(mode repair legacy)
  add_test(NAME M07_03_${mode} COMMAND CoinTextureCoordinateRange03Test ${mode})
  set_tests_properties(M07_03_${mode} PROPERTIES LABELS texture-coordinates TIMEOUT 30)
endforeach()
