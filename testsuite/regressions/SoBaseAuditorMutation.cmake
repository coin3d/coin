add_executable(SoBaseAuditorMutationTest SoBaseAuditorMutationTest.cpp)
target_link_libraries(SoBaseAuditorMutationTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(SoBaseAuditorMutationTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
add_test(NAME SoBaseAuditorMutation COMMAND SoBaseAuditorMutationTest)

add_executable(CcRbptreeTraverseTest CcRbptreeTraverseTest.cpp)
target_link_libraries(CcRbptreeTraverseTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CcRbptreeTraverseTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
# This C++ fixture deliberately lets a callback exception cross the C API.
# MSVC /EHsc assumes extern "C" calls cannot throw; disable /EHc only here.
if(MSVC)
  target_compile_options(CcRbptreeTraverseTest PRIVATE /EHsc-)
endif()
add_test(NAME CcRbptreeTraverse COMMAND CcRbptreeTraverseTest)
