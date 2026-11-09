add_executable(CoinFontPathTest fonts/PathTest.cpp)
target_link_libraries(CoinFontPathTest Coin)
add_test(NAME FontAbsolutePath COMMAND CoinFontPathTest
  "${CMAKE_CURRENT_SOURCE_DIR}/fonts/path-font.bdf")
set_tests_properties(FontAbsolutePath PROPERTIES SKIP_RETURN_CODE 77)
