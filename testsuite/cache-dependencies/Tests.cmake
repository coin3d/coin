add_executable(CoinCacheDependencyTest cache-dependencies/DependencyTest.cpp)
target_link_libraries(CoinCacheDependencyTest Coin)
add_test(NAME CacheAncestorDependencies COMMAND CoinCacheDependencyTest)
set_tests_properties(CacheAncestorDependencies PROPERTIES TIMEOUT 20 LABELS cache)

if(COIN_BUILD_LEGACY_GL_RENDERER AND UNIX AND NOT APPLE AND HAVE_GLX)
  add_executable(CoinDefaultTextureDependencyTest
    reproducers/issue-722-texture-vbo/cache-dependency.cpp)
  target_link_libraries(CoinDefaultTextureDependencyTest Coin)
  find_program(COIN_DEPENDENCY_XVFB_RUN xvfb-run)
  if(COIN_DEPENDENCY_XVFB_RUN)
    add_test(NAME DefaultTextureCacheAncestorDependencies
      COMMAND ${COIN_DEPENDENCY_XVFB_RUN} -a
        -s "-screen 0 1280x1024x24 -noreset" $<TARGET_FILE:CoinDefaultTextureDependencyTest>)
  else()
    add_test(NAME DefaultTextureCacheAncestorDependencies COMMAND CoinDefaultTextureDependencyTest)
  endif()
  set_tests_properties(DefaultTextureCacheAncestorDependencies PROPERTIES TIMEOUT 60
    LABELS cache
    ENVIRONMENT "COIN_GLXGLUE_NO_PBUFFERS=1;COIN_GLX_PIXMAP_DIRECT_RENDERING=1")
endif()
