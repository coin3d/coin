# Linux executable symbol interposition covers Coin's ordinary global new.
# Other platforms require a separately verified injection mechanism.
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_executable(CallbackLateTypesAllocationTest CallbackLateTypesAllocationTest.cpp)
  target_link_libraries(CallbackLateTypesAllocationTest Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(CallbackLateTypesAllocationTest PRIVATE ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
  foreach(callback_family RANGE 0 4)
    # 0 is the no-failure control. Each ordinal starts in a fresh process.
    foreach(allocation_ordinal RANGE 0 16)
      add_test(NAME CallbackLateTypesAllocation_${callback_family}_${allocation_ordinal}
        COMMAND CallbackLateTypesAllocationTest ${callback_family} ${allocation_ordinal})
    endforeach()
  endforeach()
endif()
