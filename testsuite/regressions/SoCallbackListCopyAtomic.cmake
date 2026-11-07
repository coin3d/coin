if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND COIN_BUILD_SHARED_LIBS)
  add_test(NAME SoCallbackListCopyAtomic
    COMMAND sh "${PROJECT_SOURCE_DIR}/testsuite/reproducers/socallbacklist-copy-exception-safety/run.sh" "$<TARGET_FILE_DIR:Coin>")
  set_tests_properties(SoCallbackListCopyAtomic PROPERTIES
    ENVIRONMENT "COIN_CALLBACK_WARM_HASH=1" TIMEOUT 120)
endif()
