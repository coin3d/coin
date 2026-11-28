if(COIN_BUILD_LEGACY_GL_RENDERER)
  add_executable(CoinSoAnnotationDepthStateTest SoAnnotationDepthStateTest.cpp)
  target_link_libraries(CoinSoAnnotationDepthStateTest Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(CoinSoAnnotationDepthStateTest PRIVATE
    ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
    ${COIN_TARGET_INCLUDE_DIRECTORIES})
  foreach(annotation_case fields nested paths cache-policy interrupt)
    add_test(NAME SoAnnotationDepth_${annotation_case}
      COMMAND CoinSoAnnotationDepthStateTest --${annotation_case})
    set_tests_properties(SoAnnotationDepth_${annotation_case} PROPERTIES
      SKIP_RETURN_CODE 77 LABELS "annotation;legacy-gl" TIMEOUT 45)
  endforeach()
  if(USE_EXCEPTIONS)
    foreach(annotation_case throw-below throw-in)
      add_test(NAME SoAnnotationDepth_${annotation_case}
        COMMAND CoinSoAnnotationDepthStateTest --${annotation_case})
      set_tests_properties(SoAnnotationDepth_${annotation_case} PROPERTIES
        SKIP_RETURN_CODE 77 LABELS "annotation;legacy-gl" TIMEOUT 30)
    endforeach()
  endif()
  if(UNIX AND NOT APPLE)
    find_package(X11 QUIET)
    if(X11_FOUND AND HAVE_GLX AND USE_EXCEPTIONS)
      add_executable(CoinSoAnnotationRecoveryGLXTest AnnotationRecoveryGLXTest.cpp)
      target_link_libraries(CoinSoAnnotationRecoveryGLXTest
        Coin ${COIN_TARGET_LINK_LIBRARIES} ${X11_LIBRARIES})
      target_include_directories(CoinSoAnnotationRecoveryGLXTest PRIVATE
        ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
        ${COIN_TARGET_INCLUDE_DIRECTORIES} ${X11_INCLUDE_DIR})
      foreach(annotation_case below in)
        add_test(NAME SoAnnotationDepth_recovery_${annotation_case}
          COMMAND CoinSoAnnotationRecoveryGLXTest ${annotation_case})
        set_tests_properties(SoAnnotationDepth_recovery_${annotation_case} PROPERTIES
          SKIP_RETURN_CODE 77 LABELS "annotation;glx" TIMEOUT 30)
      endforeach()
    endif()
  endif()
endif()
