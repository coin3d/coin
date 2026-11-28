add_executable(CoinFieldNotifyExceptionsTest regressions/fielddb/Notify.cpp)
target_link_libraries(CoinFieldNotifyExceptionsTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CoinFieldNotifyExceptionsTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
foreach(case record container auditor value value-reentrant cycle silent)
  add_test(NAME FieldNotifyExceptions_${case} COMMAND CoinFieldNotifyExceptionsTest ${case})
  set_tests_properties(FieldNotifyExceptions_${case} PROPERTIES TIMEOUT 15)
endforeach()
