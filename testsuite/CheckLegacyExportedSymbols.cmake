# These Itanium ABI entry points were exported by libCoin 4.0.10.
# Check the dynamic symbol table so inlining cannot silently remove them.
execute_process(
  COMMAND "${NM}" -D --defined-only "${LIBRARY}"
  RESULT_VARIABLE status
  OUTPUT_VARIABLE symbols
  ERROR_VARIABLE error)
if(NOT status EQUAL 0)
  message(FATAL_ERROR "Cannot inspect ${LIBRARY}: ${error}")
endif()

set(required_symbols
  _ZN6SbListI13SoOutputROUTEE4growEi
  _ZN15SoShaderObjectP20invalidateParametersEv)
foreach(symbol IN LISTS required_symbols)
  if(NOT symbols MATCHES "[ \t]${symbol}(\n|$)")
    message(FATAL_ERROR "Missing legacy exported symbol: ${symbol}")
  endif()
endforeach()
