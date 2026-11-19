if(HAVE_VRML97)
  add_executable(CoinVRMLAnalyticTexturePickTest VRMLAnalyticTexturePickTest.cpp)
  target_include_directories(CoinVRMLAnalyticTexturePickTest PRIVATE
    ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
    ${COIN_TARGET_INCLUDE_DIRECTORIES})
  target_link_libraries(CoinVRMLAnalyticTexturePickTest Coin ${COIN_TARGET_LINK_LIBRARIES})
  foreach(vrml_pick_case functions legacy generators boundaries scopes)
    add_test(NAME VRMLAnalyticTexturePick_${vrml_pick_case}
      COMMAND CoinVRMLAnalyticTexturePickTest ${vrml_pick_case})
    set_tests_properties(VRMLAnalyticTexturePick_${vrml_pick_case} PROPERTIES
      LABELS "texture-coordinates;picking;vrml" TIMEOUT 30)
  endforeach()
endif()
