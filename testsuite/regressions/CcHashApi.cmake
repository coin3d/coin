add_executable(CcHashApiTest CcHashApiTest.c)
target_link_libraries(CcHashApiTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CcHashApiTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
add_test(NAME CcHashApi COMMAND CcHashApiTest)

# This C++ caller uses the normal /EHsc mode on MSVC. It must catch exceptions
# crossing the exported C-linkage API, including a call from a separate DLL.
add_executable(CcHashExceptionTest CcHashExceptionTest.cpp)
target_link_libraries(CcHashExceptionTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CcHashExceptionTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
add_test(NAME CcHashCppExceptions COMMAND CcHashExceptionTest)
set_tests_properties(CcHashCppExceptions PROPERTIES TIMEOUT 30)
