if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND COIN_BUILD_LEGACY_GL_RENDERER)
  find_package(X11 QUIET)
  if(X11_FOUND AND HAVE_GLX)
    # Compile the actual four consumers with the existing deterministic map hook.
    add_executable(GLContextMapsTest gl-context-maps/GLContextMapsTest.cpp
      ${PROJECT_SOURCE_DIR}/src/rendering/SoVBO.cpp
      ${PROJECT_SOURCE_DIR}/src/shaders/SoGLSLShaderProgram.cpp
      ${PROJECT_SOURCE_DIR}/src/shaders/SoShaderObject.cpp
      ${PROJECT_SOURCE_DIR}/src/shaders/SoShaderParameter.cpp)
    target_compile_definitions(GLContextMapsTest PRIVATE
      COIN_INTERNAL HAVE_CONFIG_H COIN_BUILDING_COIN COIN_SMALLMAP_TESTING GL_GLEXT_PROTOTYPES)
    target_include_directories(GLContextMapsTest PRIVATE
      ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include
      ${PROJECT_BINARY_DIR}/src ${PROJECT_BINARY_DIR}/include
      ${PROJECT_SOURCE_DIR}/include/Inventor/annex
      ${COIN_TARGET_INCLUDE_DIRECTORIES} ${X11_INCLUDE_DIR})
    target_link_libraries(GLContextMapsTest Coin ${COIN_TARGET_LINK_LIBRARIES} ${X11_LIBRARIES})
    add_test(NAME GLContextMaps COMMAND GLContextMapsTest)
    set_tests_properties(GLContextMaps PROPERTIES TIMEOUT 60 SKIP_RETURN_CODE 77)
  endif()
endif()
