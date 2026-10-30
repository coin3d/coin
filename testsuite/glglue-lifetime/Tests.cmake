if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND COIN_BUILD_LEGACY_GL_RENDERER)
  find_package(X11 QUIET)
  if(X11_FOUND AND HAVE_GLX)
    find_package(Threads REQUIRED)
    add_executable(GLGlueLifetimeTest glglue-lifetime/LifetimeTest.cpp)
    target_compile_definitions(GLGlueLifetimeTest PRIVATE COIN_INTERNAL HAVE_CONFIG_H)
    target_include_directories(GLGlueLifetimeTest PRIVATE
      ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include
      ${PROJECT_BINARY_DIR}/src ${PROJECT_BINARY_DIR}/include
      ${COIN_TARGET_INCLUDE_DIRECTORIES} ${X11_INCLUDE_DIR})
    target_link_libraries(GLGlueLifetimeTest Coin Threads::Threads ${COIN_TARGET_LINK_LIBRARIES} ${X11_LIBRARIES})
    add_test(NAME GLGlueLifetime COMMAND GLGlueLifetimeTest)
    add_test(NAME GLGlueLifetimeNoCallbacks COMMAND GLGlueLifetimeTest --no-callbacks)
    set_tests_properties(GLGlueLifetime GLGlueLifetimeNoCallbacks PROPERTIES TIMEOUT 60 SKIP_RETURN_CODE 77)
  endif()
endif()
