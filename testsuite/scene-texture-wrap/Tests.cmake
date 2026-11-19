if(COIN_BUILD_LEGACY_GL_RENDERER AND UNIX AND NOT APPLE)
  find_package(X11 QUIET)
  if(X11_FOUND AND HAVE_GLX)
    add_executable(CoinSceneTextureWrapTest scene-texture-wrap/WrapTest.cpp)
    target_include_directories(CoinSceneTextureWrapTest PRIVATE ${X11_INCLUDE_DIR})
    target_link_libraries(CoinSceneTextureWrapTest PRIVATE
      Coin ${COIN_TARGET_LINK_LIBRARIES} ${X11_LIBRARIES})
    foreach(axis S T)
      add_test(NAME SceneTexture2WrapFBO_${axis}
        COMMAND CoinSceneTextureWrapTest ${axis})
      set_tests_properties(SceneTexture2WrapFBO_${axis} PROPERTIES
        SKIP_RETURN_CODE 77 TIMEOUT 20
        ENVIRONMENT "COIN_EGL=0;COIN_DONT_USE_FBO=0;COIN_GLERROR_DEBUGGING=0")
    endforeach()
  endif()
endif()
