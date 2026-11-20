find_package(Threads REQUIRED)
add_executable(CoinDBNotifyExceptionsTest regressions/fielddb/Sessions.cpp)
target_link_libraries(CoinDBNotifyExceptionsTest Coin Threads::Threads ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CoinDBNotifyExceptionsTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
foreach(case field-failure base-failure end-field end-base end-direct nested-field-success nested-base-success nested-field-caught nested-base-caught nested-field-escape nested-base-escape normal)
  add_test(NAME DBNotifyExceptions_${case} COMMAND CoinDBNotifyExceptionsTest ${case})
  set_tests_properties(DBNotifyExceptions_${case} PROPERTIES TIMEOUT 15)
endforeach()
