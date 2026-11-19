add_executable(CoinTextureCoordinateRange02Test TextureCoordinateRange02Test.cpp)
target_include_directories(CoinTextureCoordinateRange02Test PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
target_link_libraries(CoinTextureCoordinateRange02Test Coin ${COIN_TARGET_LINK_LIBRARIES})
foreach(mode repair legacy)
  add_test(NAME M07_02_${mode} COMMAND CoinTextureCoordinateRange02Test ${mode})
  set_tests_properties(M07_02_${mode} PROPERTIES LABELS texture-coordinates TIMEOUT 30)
endforeach()
