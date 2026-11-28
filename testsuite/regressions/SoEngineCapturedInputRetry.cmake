add_executable(CoinEngineCapturedInputRetryTest regressions/nodeengine/CapturedInputRetry.cpp)
target_link_libraries(CoinEngineCapturedInputRetryTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CoinEngineCapturedInputRetryTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
foreach(driver node engine)
  foreach(output first second)
    foreach(reads once twice)
      foreach(flags normal mixed)
        foreach(callback normal throw)
          add_test(NAME EngineCapturedInputRetry_${driver}_${output}_${reads}_${flags}_${callback}
            COMMAND CoinEngineCapturedInputRetryTest ${driver} ${output} ${reads} ${flags} ${callback})
          set_tests_properties(EngineCapturedInputRetry_${driver}_${output}_${reads}_${flags}_${callback}
            PROPERTIES TIMEOUT 15)
        endforeach()
      endforeach()
    endforeach()
  endforeach()
endforeach()
