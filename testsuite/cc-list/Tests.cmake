# Compile the actual implementation in all four diagnostic configurations.
# These tiny executables do not link Coin, so a Release host build can still
# test assertions and a Debug host build can test NDEBUG independently.
foreach(mode IN ITEMS plain_assert plain_ndebug extra_assert extra_ndebug)
  set(target CcList_${mode})
  add_executable(${target}
    ${PROJECT_SOURCE_DIR}/src/base/list.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cc-list/Contracts.cpp)
  target_compile_definitions(${target} PRIVATE
    COIN_INTERNAL COIN_BUILDING_COIN HAVE_CONFIG_H)
  target_include_directories(${target} PRIVATE
    ${PROJECT_BINARY_DIR}/include ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/src ${PROJECT_SOURCE_DIR}/src)
  if(mode MATCHES "^extra_")
    target_compile_definitions(${target} PRIVATE COIN_EXTRA_DEBUG)
  elseif(MSVC)
    target_compile_options(${target} PRIVATE /UCOIN_EXTRA_DEBUG)
  else()
    target_compile_options(${target} PRIVATE -UCOIN_EXTRA_DEBUG)
  endif()
  if(mode MATCHES "_ndebug$")
    target_compile_definitions(${target} PRIVATE NDEBUG)
  elseif(MSVC)
    target_compile_options(${target} PRIVATE /UNDEBUG)
  else()
    target_compile_options(${target} PRIVATE -UNDEBUG)
  endif()
endforeach()

add_test(NAME CcListDiagnosticContracts COMMAND ${CMAKE_COMMAND}
  -Dplain_assert=$<TARGET_FILE:CcList_plain_assert>
  -Dplain_ndebug=$<TARGET_FILE:CcList_plain_ndebug>
  -Dextra_assert=$<TARGET_FILE:CcList_extra_assert>
  -Dextra_ndebug=$<TARGET_FILE:CcList_extra_ndebug>
  -P ${CMAKE_CURRENT_SOURCE_DIR}/cc-list/Run.cmake)
set_tests_properties(CcListDiagnosticContracts PROPERTIES TIMEOUT 60)
