# The recording test extends the existing range target; native GLX covers the
# real delayed transparent traversal and the action-owned write policy.
if(COIN_BUILD_TESTS AND COIN_BUILD_LEGACY_GL_RENDERER)
  add_test(NAME SoDepthBuffer_ignored_engines_are_lazy
    COMMAND CoinSoDepthBufferTest --run_test=depth_ignored_engines_are_lazy)
  set_tests_properties(SoDepthBuffer_ignored_engines_are_lazy PROPERTIES
    LABELS "legacy-gl;depth-buffer;engine")
  add_test(NAME SoDepthBuffer_ignored_components_capture_cache_dependencies
    COMMAND CoinSoDepthBufferTest --run_test=depth_ignored_components_capture_cache_dependencies)
  set_tests_properties(SoDepthBuffer_ignored_components_capture_cache_dependencies PROPERTIES
    LABELS "legacy-gl;depth-buffer;engine")
  if(UNIX AND NOT APPLE AND HAVE_GLX)
    find_package(X11 QUIET)
    if(X11_FOUND)
      add_executable(CoinSoDepthBufferTransparencyTest
        ${PROJECT_SOURCE_DIR}/testsuite/SoDepthBufferTransparencyTest.cpp)
      target_link_libraries(CoinSoDepthBufferTransparencyTest PRIVATE
        Coin ${COIN_TARGET_LINK_LIBRARIES} ${X11_LIBRARIES})
      target_include_directories(CoinSoDepthBufferTransparencyTest PRIVATE
        ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
        ${COIN_TARGET_INCLUDE_DIRECTORIES} ${X11_INCLUDE_DIR})
      add_test(NAME SoDepthBuffer_transparent_write_engine_is_lazy
        COMMAND CoinSoDepthBufferTransparencyTest
          --run_test=depth_transparent_write_engine_is_lazy)
      set_tests_properties(SoDepthBuffer_transparent_write_engine_is_lazy PROPERTIES
        SKIP_RETURN_CODE 77 LABELS "legacy-gl;depth-buffer;engine;native-glx")
    endif()
  endif()
endif()
