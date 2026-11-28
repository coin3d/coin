add_executable(CcDictConsumersTest CcDictConsumersTest.cpp)
target_compile_definitions(CcDictConsumersTest PRIVATE
  COIN_INTERNAL HAVE_CONFIG_H COIN_BUILDING_COIN)
if(WIN32)
  # This target compiles copies of library sources and must therefore retain
  # COIN_BUILDING_COIN declarations. Link the Coin artifact without inheriting
  # its application-side COIN_DLL / COIN_NOT_DLL usage requirements.
  add_dependencies(CcDictConsumersTest Coin)
  target_link_libraries(CcDictConsumersTest
    "$<TARGET_LINKER_FILE:Coin>" ${COIN_TARGET_LINK_LIBRARIES})
else()
  target_link_libraries(CcDictConsumersTest
    Coin ${COIN_TARGET_LINK_LIBRARIES})
endif()
target_include_directories(CcDictConsumersTest PRIVATE
  ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}/src ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
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
