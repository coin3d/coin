function(coin_add_xml_entity_abi_test target source)
  add_executable(${target} ${source})
  target_link_libraries(${target} Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(${target} PRIVATE
    ${PROJECT_BINARY_DIR}/include
    ${PROJECT_SOURCE_DIR}/include
    ${COIN_TARGET_INCLUDE_DIRECTORIES})
  set_target_properties(${target} PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
  add_test(NAME ${target} COMMAND ${target})
endfunction()

coin_add_xml_entity_abi_test(XmlEntityCApiTest entity-c-api.c)
# Coin is implemented in C++; force its linker for this C translation unit.
set_target_properties(XmlEntityCApiTest PROPERTIES
  C_STANDARD 99
  C_STANDARD_REQUIRED YES
  C_EXTENSIONS NO
  LINKER_LANGUAGE CXX)

coin_add_xml_entity_abi_test(XmlEntityLegacyCxxAbiTest entity-legacy-cxx-abi.cpp)
