add_executable(SoBaseAuditorDedupTest SoBaseAuditorDedupTest.cpp)
target_link_libraries(SoBaseAuditorDedupTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(SoBaseAuditorDedupTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
add_test(NAME SoBaseAuditorDedup COMMAND SoBaseAuditorDedupTest)
