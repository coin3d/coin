add_executable(CoinNodeEngineMetadataTest regressions/nodeengine/Metadata.cpp)
target_link_libraries(CoinNodeEngineMetadataTest Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(CoinNodeEngineMetadataTest PRIVATE
  ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include ${COIN_TARGET_INCLUDE_DIRECTORIES})
foreach(case notify evaluate lookup empty engine-notify)
  add_test(NAME NodeEngineMetadata_${case} COMMAND CoinNodeEngineMetadataTest ${case})
endforeach()
