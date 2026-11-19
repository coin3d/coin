add_executable(CoinAnalyticShapeTexturePickTest AnalyticShapeTexturePickTest.cpp)
target_include_directories(CoinAnalyticShapeTexturePickTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
target_link_libraries(CoinAnalyticShapeTexturePickTest Coin ${COIN_TARGET_LINK_LIBRARIES})
if(HAVE_VRML97)
  target_compile_definitions(CoinAnalyticShapeTexturePickTest PRIVATE ANALYTIC_PICK_HAVE_VRML97)
endif()
foreach(analytic_pick_case functions legacy generators boundaries scopes excluded)
  add_test(NAME AnalyticShapeTexturePick_${analytic_pick_case}
    COMMAND CoinAnalyticShapeTexturePickTest ${analytic_pick_case})
  set_tests_properties(AnalyticShapeTexturePick_${analytic_pick_case} PROPERTIES
    LABELS "texture-coordinates;picking" TIMEOUT 30)
endforeach()
