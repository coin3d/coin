# Fixture replacements require executable interposition, as their owner tests do.
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  find_package(Threads REQUIRED)
  set(allocation_hook_standards 11)
  if("cxx_std_17" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
    list(APPEND allocation_hook_standards 17)
  endif()
  foreach(hook Bump Path TempPath Sensor)
    if(hook STREQUAL "Bump")
      set(hook_definition COIN_HOOK_BUMP)
    elseif(hook STREQUAL "Path")
      set(hook_definition COIN_HOOK_PATH)
    elseif(hook STREQUAL "TempPath")
      set(hook_definition COIN_HOOK_TEMP_PATH)
    else()
      set(hook_definition COIN_HOOK_SENSOR)
    endif()
    foreach(hook_standard IN LISTS allocation_hook_standards)
      set(hook_target CoinAllocationHook${hook}Cxx${hook_standard}Test)
      add_executable(${hook_target} AllocationHookContractTest.cpp)
      target_compile_definitions(${hook_target} PRIVATE ${hook_definition})
      set_target_properties(${hook_target} PROPERTIES
        CXX_STANDARD ${hook_standard} CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)
      target_link_libraries(${hook_target} Coin ${COIN_TARGET_LINK_LIBRARIES}
        Threads::Threads)
      target_include_directories(${hook_target} PRIVATE
        ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
        ${COIN_TARGET_INCLUDE_DIRECTORIES})
      # Clang does not enable sized deallocation by default in every mode.
      if(hook_standard EQUAL 17 AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${hook_target} PRIVATE -fsized-deallocation)
      endif()
      foreach(hook_case success budget constructor isolation)
        add_test(NAME AllocationHook_${hook}_cxx${hook_standard}_${hook_case}
          COMMAND ${hook_target} ${hook_case})
        set_tests_properties(AllocationHook_${hook}_cxx${hook_standard}_${hook_case}
          PROPERTIES TIMEOUT 10)
      endforeach()
    endforeach()
  endforeach()
endif()
