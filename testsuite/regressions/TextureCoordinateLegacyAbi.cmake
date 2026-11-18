# These historical private symbols were exported by the Linux shared library.
# They are not a promise of private imports from the Windows DLL.
if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND COIN_BUILD_SHARED_LIBS)
  add_executable(CoinTextureCoordinateLegacyAbiTest
    TextureCoordinateLegacyAbiTest.cpp)
  target_include_directories(CoinTextureCoordinateLegacyAbiTest PRIVATE
    ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
    ${COIN_TARGET_INCLUDE_DIRECTORIES})
  target_link_libraries(CoinTextureCoordinateLegacyAbiTest
    Coin ${COIN_TARGET_LINK_LIBRARIES})
  add_test(NAME TextureCoordinateLegacyAbi
    COMMAND CoinTextureCoordinateLegacyAbiTest)
  set_tests_properties(TextureCoordinateLegacyAbi PROPERTIES
    LABELS "texture-coordinates;abi" TIMEOUT 30)
endif()
