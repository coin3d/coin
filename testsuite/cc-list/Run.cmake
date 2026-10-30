cmake_policy(SET CMP0054 NEW)

set(invalid_cases
  get-negative get-end insert-negative insert-past-end
  remove-negative remove-end remove-fast-negative remove-fast-end
  truncate-negative truncate-past-end truncate-fit-negative truncate-fit-past-end
  remove-missing pop-empty)
set(null_handles
  null-clone null-destruct null-append null-find null-insert null-remove
  null-remove-item null-remove-fast null-fit null-truncate null-truncate-fit
  null-get-length null-get-array null-get null-push null-pop)

foreach(mode IN ITEMS plain_assert plain_ndebug extra_assert extra_ndebug)
  execute_process(COMMAND "${${mode}}" valid
    RESULT_VARIABLE result OUTPUT_VARIABLE out ERROR_VARIABLE err TIMEOUT 5)
  if(NOT "${result}" STREQUAL "0")
    message(FATAL_ERROR "${mode}: valid operations failed (${result}): ${out}${err}")
  endif()
  set(cases ${invalid_cases})
  if(mode STREQUAL "extra_assert")
    list(APPEND cases ${null_handles})
  endif()
  foreach(scenario IN LISTS cases)
    execute_process(COMMAND "${${mode}}" ${scenario}
      RESULT_VARIABLE result OUTPUT_VARIABLE out ERROR_VARIABLE err TIMEOUT 5)
    if(mode STREQUAL "extra_assert")
      # A crash or the test oracle's failure is not an expected assertion.
      if(NOT err MATCHES "[Aa]ssertion.*[Ff]ailed" OR "${result}" STREQUAL "0")
        message(FATAL_ERROR "${mode}/${scenario}: expected assertion (${result}): ${out}${err}")
      endif()
      if(UNIX AND NOT "${result}" MATCHES "[Aa]bort")
        message(FATAL_ERROR "${mode}/${scenario}: expected SIGABRT, got ${result}")
      endif()
    elseif(NOT "${result}" STREQUAL "0")
      message(FATAL_ERROR "${mode}/${scenario}: guard failed (${result}): ${out}${err}")
    endif()
  endforeach()
endforeach()
message(STATUS "cc_list: 4 valid-operation runs, 42 guarded invalid runs, 30 expected assertion aborts (14 bounds + 16 NULL handles)")
