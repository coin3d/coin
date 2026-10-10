# Compile the private wrapper in the test so no internal DLL export is needed.
if(WIN32)
  foreach(font_api_suffix "" Unicode)
    set(font_api_target CoinWin32FontApiTest${font_api_suffix})
    set(font_api_test Win32FontApi${font_api_suffix})
    add_executable(${font_api_target} Win32FontApiTest.cpp
      ${PROJECT_SOURCE_DIR}/src/glue/win32api.cpp)
    target_compile_definitions(${font_api_target} PRIVATE COIN_INTERNAL HAVE_CONFIG_H)
    if(font_api_suffix STREQUAL "Unicode")
      target_compile_definitions(${font_api_target} PRIVATE UNICODE _UNICODE)
    endif()
    if(MSVC)
      # Reintroducing GetVersionEx must fail rather than emit C4996.
      target_compile_options(${font_api_target} PRIVATE /we4996)
    endif()
    target_include_directories(${font_api_target} PRIVATE
      ${PROJECT_SOURCE_DIR}/src ${PROJECT_BINARY_DIR}/src
      ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include)
    target_link_libraries(${font_api_target} Coin gdi32)
    add_test(NAME ${font_api_test} COMMAND ${font_api_target})
    set_tests_properties(${font_api_test} PROPERTIES LABELS "unit;windows" TIMEOUT 30)
  endforeach()
endif()
