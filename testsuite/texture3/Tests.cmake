set(COIN_TEXTURE3_OVERRIDE_CASES plain locked-zero cross-unit nonzero-override
  separators quality-flags dispatch reuse)
add_executable(CoinTexture3OverrideCPUTest texture3/OverrideTest.cpp)
target_link_libraries(CoinTexture3OverrideCPUTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CoinTexture3OverrideCPUTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
foreach(texture3_case ${COIN_TEXTURE3_OVERRIDE_CASES} raypick)
  add_test(NAME Texture3OverrideCPU_${texture3_case}
    COMMAND CoinTexture3OverrideCPUTest ${texture3_case})
  set_tests_properties(Texture3OverrideCPU_${texture3_case} PROPERTIES LABELS texture3-override)
endforeach()

if(COIN_BUILD_LEGACY_GL_RENDERER AND UNIX AND NOT APPLE)
  find_package(X11 QUIET)
  if(X11_FOUND AND HAVE_GLX)
    add_executable(CoinTexture3OverrideGLXTest texture3/OverrideTest.cpp)
    target_compile_definitions(CoinTexture3OverrideGLXTest PRIVATE COIN_TEXTURE3_GLX)
    target_include_directories(CoinTexture3OverrideGLXTest PRIVATE
      ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
      ${COIN_TARGET_INCLUDE_DIRECTORIES} ${X11_INCLUDE_DIR})
    target_link_libraries(CoinTexture3OverrideGLXTest Coin
      ${COIN_TARGET_LINK_LIBRARIES} ${X11_LIBRARIES})
    foreach(texture3_case ${COIN_TEXTURE3_OVERRIDE_CASES})
      add_test(NAME Texture3OverrideGLX_${texture3_case}
        COMMAND CoinTexture3OverrideGLXTest ${texture3_case})
      set_tests_properties(Texture3OverrideGLX_${texture3_case} PROPERTIES
        SKIP_RETURN_CODE 77 LABELS "texture3-override;glx" TIMEOUT 30)
    endforeach()
  else()
    message(STATUS "SoTexture3 real GL tests unavailable: X11/GLX development support missing")
  endif()
endif()
