add_executable(CoinSoSphereTexturePickTest SoSphereTexturePickTest.cpp)
target_include_directories(CoinSoSphereTexturePickTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
target_link_libraries(CoinSoSphereTexturePickTest Coin ${COIN_TARGET_LINK_LIBRARIES})
if(HAVE_VRML97)
  target_compile_definitions(CoinSoSphereTexturePickTest PRIVATE SPHERE_PICK_HAVE_VRML97)
endif()
foreach(sphere_pick_case functions generators clipping legacy scopes)
  add_test(NAME SoSphereTexturePick_${sphere_pick_case}
    COMMAND CoinSoSphereTexturePickTest ${sphere_pick_case})
  set_tests_properties(SoSphereTexturePick_${sphere_pick_case} PROPERTIES
    LABELS "texture-coordinates;picking" TIMEOUT 30)
endforeach()
