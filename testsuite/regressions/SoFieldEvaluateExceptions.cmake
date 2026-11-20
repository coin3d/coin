find_package(Threads REQUIRED)
add_executable(CoinFieldEvaluateExceptionsTest regressions/fielddb/Evaluate.cpp)
target_link_libraries(CoinFieldEvaluateExceptionsTest Coin Threads::Threads ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CoinFieldEvaluateExceptionsTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
foreach(case override override-write recursive-throw copy-notify copy-silent engine nodeengine converter success recursive-success disabled cycle)
  add_test(NAME FieldEvaluateExceptions_${case} COMMAND CoinFieldEvaluateExceptionsTest ${case})
  set_tests_properties(FieldEvaluateExceptions_${case} PROPERTIES TIMEOUT 15)
endforeach()
