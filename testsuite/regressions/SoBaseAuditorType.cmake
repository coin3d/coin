add_executable(SoBaseAuditorTypeTest SoBaseAuditorTypeTest.cpp)
target_link_libraries(SoBaseAuditorTypeTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(SoBaseAuditorTypeTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
add_test(NAME SoBaseAuditorType COMMAND SoBaseAuditorTypeTest)
