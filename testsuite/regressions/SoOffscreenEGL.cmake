if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND HAVE_EGL)
  add_executable(CoinSoOffscreenEGLTest SoOffscreenEGLTest.cpp)
  target_include_directories(CoinSoOffscreenEGLTest PRIVATE ${COIN_TARGET_INCLUDE_DIRECTORIES})
  target_link_libraries(CoinSoOffscreenEGLTest PRIVATE Coin ${COIN_TARGET_LINK_LIBRARIES_GL})
  set(egl_test_env ${CMAKE_COMMAND} -E env
    --unset=DISPLAY --unset=WAYLAND_DISPLAY --unset=COIN_EGL --unset=COIN_EGLGLUE_NO_PBUFFERS
    EGL_PLATFORM=surfaceless LIBGL_ALWAYS_SOFTWARE=1)
  if(HAVE_GLX)
    set(test_name SoOffscreenEGLAtStartup)
    add_test(NAME SoOffscreenEGLAfterFailedProbe
      COMMAND ${egl_test_env} $<TARGET_FILE:CoinSoOffscreenEGLTest> after-failed-probe)
    set_tests_properties(SoOffscreenEGLAfterFailedProbe PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 30)
  else()
    set(test_name SoOffscreenEGL)
  endif()
  add_test(NAME ${test_name} COMMAND ${egl_test_env} $<TARGET_FILE:CoinSoOffscreenEGLTest>)
  set_tests_properties(${test_name} PROPERTIES TIMEOUT 30)
endif()
