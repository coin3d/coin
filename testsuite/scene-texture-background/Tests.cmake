if(COIN_BUILD_LEGACY_GL_RENDERER AND UNIX AND NOT APPLE)
  find_package(X11 QUIET)
  if(X11_FOUND AND HAVE_GLX)
    add_executable(CoinSceneTextureBackgroundTest scene-texture-background/BackgroundTest.cpp)
    target_include_directories(CoinSceneTextureBackgroundTest PRIVATE ${X11_INCLUDE_DIR})
    target_link_libraries(CoinSceneTextureBackgroundTest Coin ${COIN_TARGET_LINK_LIBRARIES} ${X11_LIBRARIES})
    add_test(NAME SceneTexture2BackgroundFBO COMMAND CoinSceneTextureBackgroundTest fbo)
    add_test(NAME SceneTexture2BackgroundPbuffer COMMAND CoinSceneTextureBackgroundTest pbuffer)
    set_tests_properties(SceneTexture2BackgroundFBO SceneTexture2BackgroundPbuffer
      PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 20)
    set_tests_properties(SceneTexture2BackgroundFBO PROPERTIES
      ENVIRONMENT "COIN_EGL=0;COIN_DONT_USE_FBO=0;COIN_GLERROR_DEBUGGING=0")
    # Coin probes pbuffer support using a temporary GLX pixmap context.
    # Direct rendering also lets this probe run on Xvfb without indirect GLX.
    set_property(TEST SceneTexture2BackgroundPbuffer PROPERTY ENVIRONMENT
      "COIN_EGL=0" "COIN_DONT_USE_FBO=1" "COIN_GLERROR_DEBUGGING=0"
      "COIN_GLXGLUE_NO_PBUFFERS=0" "COIN_GLX_PIXMAP_DIRECT_RENDERING=1"
      "COIN_OFFSCREEN_STENCIL_BITS=1")
  endif()
endif()
