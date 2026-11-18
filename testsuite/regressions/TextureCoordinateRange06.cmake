add_executable(CoinTextureCoordinateRange06Test TextureCoordinateRange06Test.cpp)
target_include_directories(CoinTextureCoordinateRange06Test PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
target_link_libraries(CoinTextureCoordinateRange06Test Coin ${COIN_TARGET_LINK_LIBRARIES})
foreach(mode repair legacy)
  add_test(NAME M07_06_${mode} COMMAND CoinTextureCoordinateRange06Test ${mode})
  set_tests_properties(M07_06_${mode} PROPERTIES LABELS texture-coordinates TIMEOUT 30)
endforeach()
