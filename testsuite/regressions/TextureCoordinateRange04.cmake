add_executable(CoinTextureCoordinateRange04Test TextureCoordinateRange04Test.cpp)
target_include_directories(CoinTextureCoordinateRange04Test PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
target_link_libraries(CoinTextureCoordinateRange04Test Coin ${COIN_TARGET_LINK_LIBRARIES})
foreach(mode repair legacy)
  add_test(NAME M07_04_${mode} COMMAND CoinTextureCoordinateRange04Test ${mode})
  set_tests_properties(M07_04_${mode} PROPERTIES LABELS texture-coordinates TIMEOUT 30)
endforeach()
