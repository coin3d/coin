cmake_minimum_required(VERSION 3.0...3.31)

if(NOT DEFINED COIN_CONFIG_TEMPLATE OR
   NOT DEFINED COIN_COMPAT_TEST_DIR)
  message(FATAL_ERROR "The Coin config template and test directory are required")
endif()

file(READ "${COIN_CONFIG_TEMPLATE}" _coin_config_template)
set(_coin_begin_marker "# BEGIN Coin compatibility checks")
set(_coin_end_marker "# END Coin compatibility checks")
string(FIND "${_coin_config_template}" "${_coin_begin_marker}" _coin_begin)
string(FIND "${_coin_config_template}" "${_coin_end_marker}" _coin_end)
if(_coin_begin LESS 0 OR _coin_end LESS 0 OR NOT _coin_begin LESS _coin_end)
  message(FATAL_ERROR "Could not locate the compatibility-check block")
endif()
string(LENGTH "${_coin_begin_marker}" _coin_marker_length)
math(EXPR _coin_begin "${_coin_begin} + ${_coin_marker_length}")
math(EXPR _coin_length "${_coin_end} - ${_coin_begin}")
string(SUBSTRING "${_coin_config_template}" ${_coin_begin} ${_coin_length}
  _coin_compatibility_template)

file(MAKE_DIRECTORY "${COIN_COMPAT_TEST_DIR}")
set(_coin_fragment_template
  "${COIN_COMPAT_TEST_DIR}/coin-compatibility.cmake.in")
file(WRITE "${_coin_fragment_template}" "${_coin_compatibility_template}")

macro(_coin_append_set _variable _value)
  if("${_value}" STREQUAL "__UNDEFINED__")
    set(_coin_case_script "${_coin_case_script}unset(${_variable})\n")
  else()
    string(REPLACE "|" ";" _coin_value "${_value}")
    set(_coin_case_script
      "${_coin_case_script}set(${_variable} [==[${_coin_value}]==])\n")
  endif()
endmacro()

function(_coin_run_case
    _name _expected_warnings _expected_text
    _built_system _built_pointer_size _built_msvc _built_compiler
    _built_msvc_version _built_static_runtime
    _consumer_system _consumer_pointer_size _consumer_msvc _consumer_compiler
    _consumer_msvc_version _consumer_runtime _consumer_build_type
    _consumer_configs _consumer_flags _consumer_debug_flags _opt_out)
  set(CMAKE_SYSTEM_NAME "${_built_system}")
  set(CMAKE_SIZEOF_VOID_P "${_built_pointer_size}")
  set(MSVC "${_built_msvc}")
  set(CMAKE_CXX_COMPILER_ID "${_built_compiler}")
  set(MSVC_VERSION "${_built_msvc_version}")
  set(COIN_BUILD_MSVC_STATIC_RUNTIME "${_built_static_runtime}")

  set(_coin_fragment "${COIN_COMPAT_TEST_DIR}/${_name}-fragment.cmake")
  configure_file("${_coin_fragment_template}" "${_coin_fragment}" @ONLY)

  set(_coin_case_script "")
  _coin_append_set(CMAKE_SYSTEM_NAME "${_consumer_system}")
  _coin_append_set(CMAKE_SIZEOF_VOID_P "${_consumer_pointer_size}")
  _coin_append_set(MSVC "${_consumer_msvc}")
  _coin_append_set(CMAKE_CXX_COMPILER_ID "${_consumer_compiler}")
  _coin_append_set(MSVC_VERSION "${_consumer_msvc_version}")
  _coin_append_set(CMAKE_MSVC_RUNTIME_LIBRARY "${_consumer_runtime}")
  _coin_append_set(CMAKE_BUILD_TYPE "${_consumer_build_type}")
  _coin_append_set(CMAKE_CONFIGURATION_TYPES "${_consumer_configs}")
  _coin_append_set(CMAKE_CXX_FLAGS "${_consumer_flags}")
  _coin_append_set(CMAKE_CXX_FLAGS_DEBUG "${_consumer_debug_flags}")
  _coin_append_set(COIN_NO_COMPATIBILITY_CHECK "${_opt_out}")

  set(_coin_case_trailer [=[
set(_coin_cfg "caller value")
set(_m "caller value")
include([==[@COIN_FRAGMENT@]==])
if(NOT _coin_cfg STREQUAL "caller value" OR NOT _m STREQUAL "caller value")
  message(FATAL_ERROR "Coin compatibility checks leaked temporary variables")
endif()
]=])
  string(REPLACE "@COIN_FRAGMENT@" "${_coin_fragment}"
    _coin_case_trailer "${_coin_case_trailer}")
  set(_coin_case_script "${_coin_case_script}${_coin_case_trailer}")
  set(_coin_case_file "${COIN_COMPAT_TEST_DIR}/${_name}.cmake")
  file(WRITE "${_coin_case_file}" "${_coin_case_script}")

  execute_process(
    COMMAND "${CMAKE_COMMAND}" -P "${_coin_case_file}"
    RESULT_VARIABLE _coin_result
    OUTPUT_VARIABLE _coin_stdout
    ERROR_VARIABLE _coin_stderr
    TIMEOUT 10)
  if(NOT _coin_result EQUAL 0)
    message(FATAL_ERROR
      "${_name}: child CMake failed (${_coin_result})\n${_coin_stdout}${_coin_stderr}")
  endif()

  string(REGEX MATCHALL "CMake Warning" _coin_warning_matches
    "${_coin_stderr}")
  list(LENGTH _coin_warning_matches _coin_warning_count)
  if(NOT _coin_warning_count EQUAL _expected_warnings)
    message(FATAL_ERROR
      "${_name}: expected ${_expected_warnings} warning(s), got "
      "${_coin_warning_count}\n${_coin_stdout}${_coin_stderr}")
  endif()
  if(NOT "${_expected_text}" STREQUAL "" AND
     NOT _coin_stderr MATCHES "${_expected_text}")
    message(FATAL_ERROR
      "${_name}: expected warning matching '${_expected_text}'\n${_coin_stderr}")
  endif()
endfunction()

# Arguments after the expected result describe producer metadata followed by
# consumer metadata. __UNDEFINED__ means that the consumer did not set a value;
# vertical bars encode CMake list separators in the multi-config case.
_coin_run_case(same 0 ""
  Windows 8 TRUE MSVC 1930 OFF
  Windows 8 TRUE MSVC 1930 __UNDEFINED__ __UNDEFINED__ __UNDEFINED__ "" "" OFF)
_coin_run_case(system_mismatch 1 "built for system"
  Linux 8 FALSE GNU "" OFF
  Darwin 8 FALSE GNU "" __UNDEFINED__ __UNDEFINED__ __UNDEFINED__ "" "" OFF)
_coin_run_case(pointer_mismatch 1 "32-bit architecture"
  Windows 4 TRUE MSVC 1930 OFF
  Windows 8 TRUE MSVC 1930 __UNDEFINED__ __UNDEFINED__ __UNDEFINED__ "" "" OFF)
_coin_run_case(runtime_literal 1 "dynamic runtime"
  Windows 8 TRUE MSVC 1930 OFF
  Windows 8 TRUE MSVC 1930 MultiThreaded __UNDEFINED__ __UNDEFINED__ "" "" OFF)
_coin_run_case(runtime_genex 1 "dynamic runtime"
  Windows 8 TRUE MSVC 1930 OFF
  Windows 8 TRUE MSVC 1930 "MultiThreaded$<$<CONFIG:Debug>:Debug>" Debug __UNDEFINED__ "" "" OFF)
_coin_run_case(runtime_default 1 "static runtime"
  Windows 8 TRUE MSVC 1930 ON
  Windows 8 TRUE MSVC 1930 __UNDEFINED__ __UNDEFINED__ __UNDEFINED__ "" "" OFF)
_coin_run_case(legacy_msvc 1 "prior to Visual Studio 2015"
  Windows 8 TRUE MSVC 1700 OFF
  Windows 8 TRUE MSVC 1800 __UNDEFINED__ __UNDEFINED__ __UNDEFINED__ "" "" OFF)
_coin_run_case(modern_to_legacy_msvc 1 "newer toolsets"
  Windows 8 TRUE MSVC 1930 OFF
  Windows 8 TRUE MSVC 1800 __UNDEFINED__ __UNDEFINED__ __UNDEFINED__ "" "" OFF)
_coin_run_case(older_modern_msvc 1 "equal to or newer"
  Windows 8 TRUE MSVC 1930 OFF
  Windows 8 TRUE MSVC 1920 __UNDEFINED__ __UNDEFINED__ __UNDEFINED__ "" "" OFF)
_coin_run_case(msvc_to_gnu 1 "runtime crashes"
  Windows 8 TRUE MSVC 1930 OFF
  Windows 8 FALSE GNU "" __UNDEFINED__ __UNDEFINED__ __UNDEFINED__ "" "" OFF)
_coin_run_case(gnu_to_msvc 1 "runtime crashes"
  Windows 8 FALSE GNU "" OFF
  Windows 8 TRUE MSVC 1930 __UNDEFINED__ __UNDEFINED__ __UNDEFINED__ "" "" OFF)
_coin_run_case(gnu_to_clang 0 ""
  Linux 8 FALSE GNU "" OFF
  Linux 8 FALSE Clang "" __UNDEFINED__ __UNDEFINED__ __UNDEFINED__ "" "" OFF)
_coin_run_case(apple_same_target 0 ""
  Darwin 8 FALSE AppleClang "" OFF
  Darwin 8 FALSE AppleClang "" __UNDEFINED__ __UNDEFINED__ __UNDEFINED__ "" "" OFF)
_coin_run_case(opt_out 0 ""
  Windows 8 TRUE MSVC 1930 OFF
  Darwin 4 TRUE MSVC 1800 MultiThreaded __UNDEFINED__ __UNDEFINED__ "" "" ON)
_coin_run_case(release_ignores_debug_flags 0 ""
  Windows 8 TRUE MSVC 1930 OFF
  Windows 8 TRUE MSVC 1930 __UNDEFINED__ Release __UNDEFINED__ /MD /MTd OFF)
_coin_run_case(debug_conditional_static 1 "dynamic runtime"
  Windows 8 TRUE MSVC 1930 OFF
  Windows 8 TRUE MSVC 1930
  "MultiThreaded$<$<CONFIG:Debug>:Debug>$<$<NOT:$<CONFIG:Debug>>:DLL>"
  Debug __UNDEFINED__ "" "" OFF)
_coin_run_case(debug_if_static 1 "dynamic runtime"
  Windows 8 TRUE MSVC 1930 OFF
  Windows 8 TRUE MSVC 1930
  "$<IF:$<CONFIG:Debug>,MultiThreadedDebug,MultiThreadedDLL>"
  Debug __UNDEFINED__ "" "" OFF)
_coin_run_case(empty_config_ignores_release_flags 0 ""
  Windows 8 TRUE MSVC 1930 OFF
  Windows 8 TRUE MSVC 1930 __UNDEFINED__ "" __UNDEFINED__ /MD /MT OFF)
_coin_run_case(lowercase_debug 1 "dynamic runtime"
  Windows 8 TRUE MSVC 1930 OFF
  Windows 8 TRUE MSVC 1930
  "MultiThreaded$<$<CONFIG:Debug>:Debug>$<$<NOT:$<CONFIG:Debug>>:DLL>"
  debug __UNDEFINED__ "" "" OFF)
_coin_run_case(nested_if 1 "dynamic runtime"
  Windows 8 TRUE MSVC 1930 OFF
  Windows 8 TRUE MSVC 1930
  "$<IF:$<CONFIG:Debug>,$<IF:$<CONFIG:Release>,MultiThreadedDLL,MultiThreadedDebug>,MultiThreadedDLL>"
  Debug __UNDEFINED__ "" "" OFF)
_coin_run_case(unsupported_expression 0 ""
  Windows 8 TRUE MSVC 1930 ON
  Windows 8 TRUE MSVC 1930
  "$<IF:$<BOOL:1>,MultiThreaded,MultiThreadedDLL>"
  Debug __UNDEFINED__ "" "" OFF)
_coin_run_case(inactive_nested_expression 0 ""
  Windows 8 TRUE MSVC 1930 ON
  Windows 8 TRUE MSVC 1930
  "MultiThreaded$<$<CONFIG:Release>:$<IF:$<BOOL:0>,Debug,DebugDLL>>"
  Debug __UNDEFINED__ "" "" OFF)
_coin_run_case(if_followed_by_suffix 0 ""
  Windows 8 TRUE MSVC 1930 OFF
  Windows 8 TRUE MSVC 1930
  "$<IF:$<CONFIG:Debug>,MultiThreaded,$<IF:$<BOOL:0>,MultiThreaded,MultiThreadedDLL>>$<$<CONFIG:Debug>:DebugDLL>"
  Debug __UNDEFINED__ "" "" OFF)
_coin_run_case(or_static 1 "dynamic runtime"
  Windows 8 TRUE MSVC 1930 OFF
  Windows 8 TRUE MSVC 1930
  "$<IF:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>,MultiThreaded,MultiThreadedDLL>"
  Debug __UNDEFINED__ "" "" OFF)
_coin_run_case(and_dynamic 0 ""
  Windows 8 TRUE MSVC 1930 OFF
  Windows 8 TRUE MSVC 1930
  "$<IF:$<AND:$<CONFIG:Debug>,$<CONFIG:Release>>,MultiThreaded,MultiThreadedDLL>"
  Debug __UNDEFINED__ "" "" OFF)
_coin_run_case(dash_static 1 "dynamic runtime"
  Windows 8 TRUE MSVC 1930 OFF
  Windows 8 TRUE MSVC 1930 __UNDEFINED__ Debug __UNDEFINED__ -MTd "" OFF)
_coin_run_case(last_flag_dynamic 0 ""
  Windows 8 TRUE MSVC 1930 OFF
  Windows 8 TRUE MSVC 1930 __UNDEFINED__ Debug __UNDEFINED__ "-MT /MDd" "" OFF)
_coin_run_case(multi_config 2 "configuration"
  Windows 8 TRUE MSVC 1930 ON
  Windows 8 TRUE MSVC 1930
  "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
  __UNDEFINED__ "Debug|Release" "" "" OFF)
_coin_run_case(non_msvc_compiler_mismatch 1 "ABI compatibility"
  Linux 8 FALSE IntelLLVM "" OFF
  Linux 8 FALSE GNU "" __UNDEFINED__ __UNDEFINED__ __UNDEFINED__ "" "" OFF)

message(STATUS "Coin config compatibility matrix passed")
