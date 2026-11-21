if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND HAVE_EGL AND NOT HAVE_GLX)
  add_executable(CoinSoOffscreenEGLTest SoOffscreenEGLTest.cpp)
  target_include_directories(CoinSoOffscreenEGLTest PRIVATE ${COIN_TARGET_INCLUDE_DIRECTORIES})
  target_link_libraries(CoinSoOffscreenEGLTest PRIVATE Coin ${COIN_TARGET_LINK_LIBRARIES_GL})
  add_test(NAME SoOffscreenEGL COMMAND CoinSoOffscreenEGLTest)
  set_tests_properties(SoOffscreenEGL PROPERTIES
    ENVIRONMENT "EGL_PLATFORM=surfaceless;LIBGL_ALWAYS_SOFTWARE=1;DISPLAY=;WAYLAND_DISPLAY="
    TIMEOUT 30)
endif()
