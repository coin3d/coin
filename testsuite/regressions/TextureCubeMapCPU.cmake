# Pure CPU traversal must work without CoinRender or a display server.
add_executable(CoinTextureCubeMapCPUTest TextureCubeMapCPUTest.cpp)
target_include_directories(CoinTextureCubeMapCPUTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
target_link_libraries(CoinTextureCubeMapCPUTest Coin ${COIN_TARGET_LINK_LIBRARIES})
foreach(cubemap_action callback pick)
  foreach(cubemap_case mesh state reuse)
    add_test(NAME TextureCubeMapCPU_${cubemap_action}_${cubemap_case}
      COMMAND CoinTextureCubeMapCPUTest ${cubemap_action} ${cubemap_case})
    set_tests_properties(TextureCubeMapCPU_${cubemap_action}_${cubemap_case}
      PROPERTIES LABELS texture-coordinates TIMEOUT 30)
  endforeach()
endforeach()
