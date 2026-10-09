add_executable(SoBaseAuditorTypeTest SoBaseAuditorTypeTest.cpp)
target_link_libraries(SoBaseAuditorTypeTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(SoBaseAuditorTypeTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
add_test(NAME SoBaseAuditorType COMMAND SoBaseAuditorTypeTest)

# A C client must be able to link the public pointer/data removal function.
add_executable(CcRbptreePairRemovalTest CcRbptreePairRemovalTest.c)
target_link_libraries(CcRbptreePairRemovalTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CcRbptreePairRemovalTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
add_test(NAME CcRbptreePairRemoval COMMAND CcRbptreePairRemovalTest)
