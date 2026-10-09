add_executable(CcListOomTest CcListOomTest.cpp)
target_compile_definitions(CcListOomTest PRIVATE
  COIN_INTERNAL HAVE_CONFIG_H COIN_BUILDING_COIN)
target_include_directories(CcListOomTest PRIVATE
  ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}/src ${PROJECT_BINARY_DIR}/include)
add_test(NAME CcListOom COMMAND CcListOomTest)

if(NOT WIN32)
  add_executable(CcWpoolOomTest CcWpoolOomTest.cpp)
  target_compile_definitions(CcWpoolOomTest PRIVATE COIN_INTERNAL HAVE_CONFIG_H)
  target_link_libraries(CcWpoolOomTest Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(CcWpoolOomTest PRIVATE
    ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/src ${PROJECT_BINARY_DIR}/include
    ${COIN_TARGET_INCLUDE_DIRECTORIES})
  add_test(NAME CcWpoolOom COMMAND CcWpoolOomTest)

  add_executable(CcSyncOomTest CcSyncOomTest.cpp)
  target_compile_definitions(CcSyncOomTest PRIVATE COIN_INTERNAL HAVE_CONFIG_H)
  target_link_libraries(CcSyncOomTest Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(CcSyncOomTest PRIVATE
    ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/src ${PROJECT_BINARY_DIR}/include
    ${COIN_TARGET_INCLUDE_DIRECTORIES})
  add_test(NAME CcSyncOom COMMAND CcSyncOomTest)

  add_executable(CcHashOomTest CcHashOomTest.cpp)
  target_compile_definitions(CcHashOomTest PRIVATE COIN_INTERNAL HAVE_CONFIG_H)
  target_link_libraries(CcHashOomTest Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(CcHashOomTest PRIVATE
    ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/src ${PROJECT_BINARY_DIR}/include
    ${COIN_TARGET_INCLUDE_DIRECTORIES})
  add_test(NAME CcHashOom COMMAND CcHashOomTest)

  add_executable(CcWorkerOomTest CcWorkerOomTest.cpp)
  target_compile_definitions(CcWorkerOomTest PRIVATE COIN_INTERNAL HAVE_CONFIG_H)
  target_link_libraries(CcWorkerOomTest Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(CcWorkerOomTest PRIVATE
    ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/src ${PROJECT_BINARY_DIR}/include
    ${COIN_TARGET_INCLUDE_DIRECTORIES})
  add_test(NAME CcWorkerOom COMMAND CcWorkerOomTest)

  add_executable(CcSbHashOomTest CcSbHashOomTest.cpp)
  target_compile_definitions(CcSbHashOomTest PRIVATE COIN_INTERNAL HAVE_CONFIG_H)
  target_link_libraries(CcSbHashOomTest Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(CcSbHashOomTest PRIVATE
    ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/src ${PROJECT_BINARY_DIR}/include
    ${COIN_TARGET_INCLUDE_DIRECTORIES})
  add_test(NAME CcSbHashOom COMMAND CcSbHashOomTest)
endif()
