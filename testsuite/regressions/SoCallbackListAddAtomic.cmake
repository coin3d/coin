if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND COIN_BUILD_SHARED_LIBS)
  add_test(NAME SoCallbackListAddAtomic
    COMMAND sh "${PROJECT_SOURCE_DIR}/testsuite/reproducers/socallbacklist-add-exception-safety/run.sh" "$<TARGET_FILE_DIR:Coin>")
  # Prepare hash buckets so injection exercises throwing C++ allocations;
  # the integrated OOM policy intentionally aborts on exhausted hash storage.
  set_tests_properties(SoCallbackListAddAtomic PROPERTIES
    ENVIRONMENT "COIN_CALLBACK_WARM_HASH=1" TIMEOUT 120)
endif()
