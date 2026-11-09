add_executable(CoinFontPathTest fonts/PathTest.cpp)
target_link_libraries(CoinFontPathTest Coin)

if(APPLE)
  add_executable(CoinCoretextTest fonts/CoretextTest.cpp)
  add_test(NAME FontCoretext COMMAND CoinCoretextTest)
  set_tests_properties(FontCoretext PROPERTIES SKIP_RETURN_CODE 77)
  target_compile_definitions(CoinCoretextTest PRIVATE COIN_INTERNAL HAVE_CONFIG_H)
  target_include_directories(CoinCoretextTest PRIVATE
    ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/src ${PROJECT_BINARY_DIR}/include
    ${COIN_TARGET_INCLUDE_DIRECTORIES})
  target_link_libraries(CoinCoretextTest Coin ${COIN_TARGET_LINK_LIBRARIES})
endif()

add_test(NAME FontAbsolutePath COMMAND CoinFontPathTest
  "${CMAKE_CURRENT_SOURCE_DIR}/fonts/path-font.bdf")
set_tests_properties(FontAbsolutePath PROPERTIES SKIP_RETURN_CODE 77)
