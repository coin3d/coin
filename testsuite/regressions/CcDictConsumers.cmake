add_executable(CcDictConsumersTest CcDictConsumersTest.cpp)
target_compile_definitions(CcDictConsumersTest PRIVATE
  COIN_INTERNAL HAVE_CONFIG_H COIN_BUILDING_COIN)
target_link_libraries(CcDictConsumersTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CcDictConsumersTest PRIVATE
  ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}/src ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
if(WIN32)
  # Linking Coin propagates its application-side DLL definition. This target
  # compiles copies of library sources, so COIN_BUILDING_COIN must control
  # their declarations without COIN_DLL / COIN_NOT_DLL from the link interface.
  if(MSVC)
    target_compile_options(CcDictConsumersTest PRIVATE /UCOIN_DLL /UCOIN_NOT_DLL)
  else()
    target_compile_options(CcDictConsumersTest PRIVATE -UCOIN_DLL -UCOIN_NOT_DLL)
  endif()
endif()
if(WIN32 AND COIN_BUILD_SHARED_LIBS)
  target_sources(CcDictConsumersTest PRIVATE CoinGeqPrimeNumberWin32TestSupport.cpp)
endif()
add_test(NAME CcDictConsumerConstructors COMMAND CcDictConsumersTest constructors)
add_test(NAME CcDictHeapEntryFailure COMMAND CcDictConsumersTest heap)
add_test(NAME CcDictHeapRebuild COMMAND CcDictConsumersTest rebuild)
add_test(NAME CcDictStorageFailure COMMAND CcDictConsumersTest storage)
set_tests_properties(CcDictConsumerConstructors CcDictHeapEntryFailure
  CcDictHeapRebuild CcDictStorageFailure PROPERTIES TIMEOUT 30)
if(HAVE_THREADS)
  add_test(NAME CcDictSchedulerFailure COMMAND CcDictConsumersTest scheduler)
  add_test(NAME CcDictSchedulerIdWrap COMMAND CcDictConsumersTest ids)
  set_tests_properties(CcDictSchedulerFailure CcDictSchedulerIdWrap
    PROPERTIES TIMEOUT 30)
endif()
