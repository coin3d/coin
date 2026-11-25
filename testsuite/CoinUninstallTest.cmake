cmake_minimum_required(VERSION 3.0...3.31)

string(RANDOM LENGTH 12 suffix)
set(test_dir "${COIN_TEST_DIR}/coin uninstall ${suffix}")
set(prefix "${test_dir}/prefix")
set(stage "${test_dir}/stage")
set(otherstage "${test_dir}/other-stage")
set(manifest "${test_dir}/install_manifest.txt")
set(installed "${prefix}/share/Coin/coin.dat")
set(link "${prefix}/share/Coin/coin-link")
set(unrelated "${stage}${prefix}/share/Coin/keep.dat")

file(MAKE_DIRECTORY "${prefix}/share/Coin" "${stage}${prefix}/share/Coin"
  "${otherstage}${prefix}/share/Coin")
file(WRITE "${installed}" "unstaged file")
file(WRITE "${stage}${installed}" "installed file")
file(WRITE "${otherstage}${installed}" "file in another staging tree")
file(WRITE "${unrelated}" "unrelated file")
execute_process(COMMAND "${CMAKE_COMMAND}" -E create_symlink
  "${prefix}/missing" "${stage}${link}" RESULT_VARIABLE link_result)
if(NOT link_result EQUAL 0)
  message(FATAL_ERROR "Could not create a test symlink.")
endif()
file(WRITE "${manifest}" "${installed}\n${link}")
file(WRITE "${test_dir}/coin-install-destdir.txt" "${stage}")

foreach(wrong_stage IN ITEMS "" "${otherstage}")
  execute_process(COMMAND "${CMAKE_COMMAND}" -E env "DESTDIR=${wrong_stage}"
    "${CMAKE_COMMAND}" "-DCOIN_INSTALL_MANIFEST=${manifest}"
    -P "${COIN_UNINSTALL_SCRIPT}" RESULT_VARIABLE result ERROR_VARIABLE error)
  if(result EQUAL 0 OR NOT error MATCHES "DESTDIR differs"
      OR NOT EXISTS "${installed}" OR NOT EXISTS "${stage}${installed}"
      OR NOT EXISTS "${otherstage}${installed}" OR NOT IS_SYMLINK "${stage}${link}")
    message(FATAL_ERROR "Uninstall did not reject a mismatched DESTDIR safely.")
  endif()
endforeach()

foreach(attempt RANGE 1 2)
  execute_process(COMMAND "${CMAKE_COMMAND}" -E env "DESTDIR=${stage}"
    "${CMAKE_COMMAND}" "-DCOIN_INSTALL_MANIFEST=${manifest}"
    -P "${COIN_UNINSTALL_SCRIPT}" RESULT_VARIABLE result)
  if(NOT result EQUAL 0 OR EXISTS "${stage}${installed}" OR IS_SYMLINK "${stage}${link}")
    message(FATAL_ERROR "Staged uninstall failed on attempt ${attempt}.")
  endif()
  if(NOT EXISTS "${installed}" OR NOT EXISTS "${unrelated}")
    message(FATAL_ERROR "Staged uninstall removed a file outside its manifest.")
  endif()
endforeach()

file(REMOVE_RECURSE "${test_dir}")
