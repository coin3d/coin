if(UNIX AND NOT APPLE)
  find_package(X11 QUIET)
  if(X11_FOUND AND HAVE_GLX)
    add_executable(CoinSceneTextureTypeTest scene-texture-type/TypeTest.cpp)
    target_include_directories(CoinSceneTextureTypeTest PRIVATE ${X11_INCLUDE_DIR})
    target_link_libraries(CoinSceneTextureTypeTest PRIVATE
      Coin ${COIN_TARGET_LINK_LIBRARIES} ${X11_LIBRARIES})
    foreach(texture_type rgba16f depth rgba4)
      add_test(NAME SceneTexture2TypeFBO_${texture_type}
        COMMAND CoinSceneTextureTypeTest ${texture_type})
      set_tests_properties(SceneTexture2TypeFBO_${texture_type} PROPERTIES
        SKIP_RETURN_CODE 77 TIMEOUT 20
        ENVIRONMENT "COIN_EGL=0;COIN_DONT_USE_FBO=0;COIN_GLERROR_DEBUGGING=0")
    endforeach()
  endif()
endif()
