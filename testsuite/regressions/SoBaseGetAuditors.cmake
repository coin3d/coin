add_executable(SoBaseGetAuditorsTest SoBaseGetAuditorsTest.cpp)
target_link_libraries(SoBaseGetAuditorsTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(SoBaseGetAuditorsTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
add_test(NAME SoBaseGetAuditors COMMAND SoBaseGetAuditorsTest refresh)
add_test(NAME SoBaseGetAuditorsEmpty COMMAND SoBaseGetAuditorsTest empty)
add_test(NAME SoBaseGetAuditorsLifetime COMMAND SoBaseGetAuditorsTest lifetime)
