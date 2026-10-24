add_executable(CallbackTailOccurrenceTest CallbackTailOccurrenceTest.cpp)
target_link_libraries(CallbackTailOccurrenceTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CallbackTailOccurrenceTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
if(HAVE_NODEKITS)
  target_compile_definitions(CallbackTailOccurrenceTest PRIVATE TEST_NODEKITS)
endif()
foreach(callback_case single path-list)
  add_test(NAME CallbackTailOccurrence_${callback_case}
    COMMAND CallbackTailOccurrenceTest ${callback_case})
endforeach()
