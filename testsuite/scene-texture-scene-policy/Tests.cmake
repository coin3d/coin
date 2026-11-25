if(COIN_BUILD_LEGACY_GL_RENDERER AND UNIX AND NOT APPLE)
  find_package(X11 QUIET)
  if(X11_FOUND AND HAVE_GLX)
    add_executable(CoinSceneTexturePolicyTest scene-texture-scene-policy/TransparencyTest.cpp)
    target_include_directories(CoinSceneTexturePolicyTest PRIVATE ${X11_INCLUDE_DIR})
    target_link_libraries(CoinSceneTexturePolicyTest Coin ${COIN_TARGET_LINK_LIBRARIES} ${X11_LIBRARIES})

    foreach(scenario initial replace edit null invalid)
      set(test_name SceneTexture2ScenePolicyOffscreen_${scenario})
      add_test(NAME ${test_name} COMMAND CoinSceneTexturePolicyTest ${scenario})
      set_tests_properties(${test_name} PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 20)
      # GLXPixmap is the software offscreen fallback used by the same
      # scene texture path as native pbuffers. A direct GL context lets
      # the fallback run on Xvfb without indirect GLX.
      set_property(TEST ${test_name} PROPERTY ENVIRONMENT
        "COIN_EGL=0" "COIN_DONT_USE_FBO=1" "COIN_GLERROR_DEBUGGING=0"
        "COIN_GLXGLUE_NO_PBUFFERS=1" "COIN_GLX_PIXMAP_DIRECT_RENDERING=1"
        "COIN_OFFSCREEN_STENCIL_BITS=1")
    endforeach()

    add_test(NAME SceneTexture2ScenePolicyFBO COMMAND CoinSceneTexturePolicyTest fbo)
    set_tests_properties(SceneTexture2ScenePolicyFBO PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 20
      ENVIRONMENT "COIN_EGL=0;COIN_DONT_USE_FBO=0;COIN_GLERROR_DEBUGGING=0")
  endif()
endif()
