if(COIN_BUILD_LEGACY_GL_RENDERER)
  add_executable(CoinOffscreenFrameClearTest offscreen/FrameClearTest.cpp)
  target_link_libraries(CoinOffscreenFrameClearTest Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(CoinOffscreenFrameClearTest PRIVATE
    ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
    ${COIN_TARGET_INCLUDE_DIRECTORIES})
  add_test(NAME OffscreenFrameClear COMMAND CoinOffscreenFrameClearTest)
  add_test(NAME OffscreenFrameClearTiled COMMAND CoinOffscreenFrameClearTest --tiled)
  set_tests_properties(OffscreenFrameClear OffscreenFrameClearTiled PROPERTIES
    SKIP_RETURN_CODE 77 LABELS "legacy-gl;offscreen" TIMEOUT 30)
  set_tests_properties(OffscreenFrameClear PROPERTIES ENVIRONMENT
    "COIN_FORCE_TILED_OFFSCREENRENDERING=0;COIN_OFFSCREENRENDERER_TILEWIDTH=0;COIN_OFFSCREENRENDERER_TILEHEIGHT=0;COIN_OFFSCREENRENDERER_MAX_TILESIZE=0")
  set_tests_properties(OffscreenFrameClearTiled PROPERTIES ENVIRONMENT
    "COIN_FORCE_TILED_OFFSCREENRENDERING=0;COIN_OFFSCREENRENDERER_TILEWIDTH=16;COIN_OFFSCREENRENDERER_TILEHEIGHT=11;COIN_OFFSCREENRENDERER_MAX_TILESIZE=0")

  # ELF interposition directly observes clears and temporary registration.
  if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND COIN_BUILD_SHARED_LIBS)
    target_compile_definitions(CoinOffscreenFrameClearTest PRIVATE COIN_OFFSCREEN_OBSERVE_CLEAR)
    target_link_libraries(CoinOffscreenFrameClearTest ${CMAKE_DL_LIBS})
    add_executable(CoinOffscreenCallbackLifetimeTest offscreen/CallbackLifetimeTest.cpp)
    target_link_libraries(CoinOffscreenCallbackLifetimeTest Coin
      ${COIN_TARGET_LINK_LIBRARIES} ${CMAKE_DL_LIBS})
    target_include_directories(CoinOffscreenCallbackLifetimeTest PRIVATE
      ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
      ${COIN_TARGET_INCLUDE_DIRECTORIES})
    foreach(lifetime_case success node-before path-before node-callback path-callback
                          node-switch path-switch node-switch-throw path-switch-throw)
      add_test(NAME OffscreenCallbackLifetime_${lifetime_case}
        COMMAND CoinOffscreenCallbackLifetimeTest ${lifetime_case})
      set_tests_properties(OffscreenCallbackLifetime_${lifetime_case} PROPERTIES
        SKIP_RETURN_CODE 77 LABELS "legacy-gl;offscreen" TIMEOUT 30)
    endforeach()
  endif()
endif()
