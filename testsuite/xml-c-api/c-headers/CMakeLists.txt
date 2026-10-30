function(coin_add_xml_c_test target source)
  add_executable(${target} ${source})
  target_link_libraries(${target} Coin ${COIN_TARGET_LINK_LIBRARIES})
  target_include_directories(${target} PRIVATE
    ${PROJECT_BINARY_DIR}/include
    ${PROJECT_SOURCE_DIR}/include
    ${COIN_TARGET_INCLUDE_DIRECTORIES})
  set_target_properties(${target} PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
    DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
    LINKER_LANGUAGE CXX)
  add_test(NAME ${target} COMMAND ${target})
endfunction()

set(xml_c_header_tests
  attribute
  document
  element
  entity
  parser
  path
  types
  world)

foreach(header IN LISTS xml_c_header_tests)
  string(SUBSTRING ${header} 0 1 first_letter)
  string(TOUPPER ${first_letter} first_letter)
  string(SUBSTRING ${header} 1 -1 remaining_letters)
  coin_add_xml_c_test(
    XmlCHeader${first_letter}${remaining_letters}Test
    header-${header}.c)
endforeach()

coin_add_xml_c_test(XmlCHeadersAggregateTest headers-aggregate.c)
coin_add_xml_c_test(XmlCApiTest c-api.c)

add_executable(XmlLegacyPathDocumentCxxAbiTest legacy-path-document-cxx-abi.cpp)
target_link_libraries(XmlLegacyPathDocumentCxxAbiTest
  Coin ${COIN_TARGET_LINK_LIBRARIES})
target_include_directories(XmlLegacyPathDocumentCxxAbiTest PRIVATE
  ${PROJECT_BINARY_DIR}/include
  ${PROJECT_SOURCE_DIR}/include
  ${COIN_TARGET_INCLUDE_DIRECTORIES})
set_target_properties(XmlLegacyPathDocumentCxxAbiTest PROPERTIES
  DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
add_test(NAME XmlLegacyPathDocumentCxxAbiTest
  COMMAND XmlLegacyPathDocumentCxxAbiTest)
