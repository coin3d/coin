if(COIN_BUILD_LEGACY_GL_RENDERER AND UNIX AND NOT APPLE)
  find_package(X11 QUIET)
  if(X11_FOUND AND HAVE_GLX)
    add_executable(CoinSceneTextureTransparencyTest scene-texture-transparency/TransparencyTest.cpp)
    target_include_directories(CoinSceneTextureTransparencyTest PRIVATE ${X11_INCLUDE_DIR})
    target_link_libraries(CoinSceneTextureTransparencyTest PRIVATE
      Coin ${COIN_TARGET_LINK_LIBRARIES} ${X11_LIBRARIES})
    foreach(scenario policy scene)
      add_test(NAME SceneTexture2TransparencyFBO_${scenario}
        COMMAND CoinSceneTextureTransparencyTest ${scenario})
      set_tests_properties(SceneTexture2TransparencyFBO_${scenario} PROPERTIES
        SKIP_RETURN_CODE 77 TIMEOUT 20
        ENVIRONMENT "COIN_EGL=0;COIN_DONT_USE_FBO=0;COIN_GLERROR_DEBUGGING=0")
    endforeach()
  endif()
endif()
