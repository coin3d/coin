# These tests exercise the native fixed-function context, independent of
# offscreen renderer selection and optional scene-rendering frontends.
if(COIN_BUILD_LEGACY_GL_RENDERER AND UNIX AND NOT APPLE)
  find_package(X11 QUIET)
  if(X11_FOUND AND HAVE_GLX)
  add_executable(CoinSuperimpositionDepthGLXTest superimposition/DepthGLXTest.cpp)
  target_include_directories(CoinSuperimpositionDepthGLXTest PRIVATE
    ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
    ${COIN_TARGET_INCLUDE_DIRECTORIES} ${X11_INCLUDE_DIR})
  target_link_libraries(CoinSuperimpositionDepthGLXTest
    Coin ${COIN_TARGET_LINK_LIBRARIES} ${X11_LIBRARIES})
  set(superimposition_depth_cases off on explicit_on explicit_off native_tuple
    transparent_off transparent_on transparent_writes clear_disabled cache_shared
    foreground background abort_reuse delayed_abort_reuse transparent_abort_reuse callback_lifetime retained_path_lifetime clear_color)
  if(USE_EXCEPTIONS)
    target_compile_definitions(CoinSuperimpositionDepthGLXTest PRIVATE COIN_SUPER_TEST_EXCEPTIONS)
    list(APPEND superimposition_depth_cases exception_cleanup delayed_exception_reuse transparent_exception_reuse)
  endif()
  foreach(depth_case ${superimposition_depth_cases})
    add_test(NAME SuperimpositionDepth_${depth_case}
      COMMAND CoinSuperimpositionDepthGLXTest ${depth_case})
    set_tests_properties(SuperimpositionDepth_${depth_case} PROPERTIES
      SKIP_RETURN_CODE 77 LABELS "superimposition;glx" TIMEOUT 30)
  endforeach()
endif()
endif()
