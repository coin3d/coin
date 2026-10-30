if(NOT WIN32)
  add_executable(CcRbptreeOomTest CcRbptreeOomTest.cpp)
  target_compile_definitions(CcRbptreeOomTest PRIVATE
    COIN_INTERNAL HAVE_CONFIG_H COIN_BUILDING_COIN)
  target_link_libraries(CcRbptreeOomTest Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(CcRbptreeOomTest PRIVATE
    ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/src ${PROJECT_BINARY_DIR}/include
    ${COIN_TARGET_INCLUDE_DIRECTORIES})
  add_test(NAME CcRbptreeOom COMMAND CcRbptreeOomTest)
endif()
