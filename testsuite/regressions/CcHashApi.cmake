add_executable(CcHashApiTest CcHashApiTest.c)
target_link_libraries(CcHashApiTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CcHashApiTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
add_test(NAME CcHashApi COMMAND CcHashApiTest)
