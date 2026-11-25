if(COIN_BUILD_LEGACY_GL_RENDERER AND UNIX AND NOT APPLE)
  find_package(X11 QUIET)
  if(X11_FOUND AND HAVE_GLX)
    add_executable(CoinAlphaTestGLXTest SoAlphaTestGLXTest.cpp)
    target_include_directories(CoinAlphaTestGLXTest PRIVATE
      ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
      ${COIN_TARGET_INCLUDE_DIRECTORIES} ${X11_INCLUDE_DIR})
    target_link_libraries(CoinAlphaTestGLXTest
      Coin ${COIN_TARGET_LINK_LIBRARIES} ${X11_LIBRARIES})
    foreach(alpha_case controls ignored cache float)
      add_test(NAME AlphaTestGLX_${alpha_case} COMMAND CoinAlphaTestGLXTest ${alpha_case})
      set_tests_properties(AlphaTestGLX_${alpha_case} PROPERTIES
        SKIP_RETURN_CODE 77 LABELS "legacy-gl;alpha-test" TIMEOUT 30)
    endforeach()
  endif()
endif()
