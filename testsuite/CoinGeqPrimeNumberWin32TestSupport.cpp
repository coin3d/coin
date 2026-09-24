// SbHash's resize()/operator=() (exercised from InternalMacroTest.cpp via
// misc/SbHash.h) call coin_geq_prime_number(), a private Coin symbol. On
// Windows shared-library builds this symbol is not exported from the Coin
// DLL, so linking CoinTests.exe against it fails. Compile a copy of the
// (small, self-contained) implementation directly into the test executable
// instead of exposing it as public API, matching the existing dict.cpp
// precedent used elsewhere in this file for the same kind of problem.
//
// tidbitsp.h declares this function inside an `extern "C" { ... }` block,
// so the definition must be included through that same header (with
// COIN_INTERNAL defined, see CMakeLists.txt) to get matching C linkage --
// a plain C++ definition here would be name-mangled differently and the
// linker would report it as unresolved anyway despite existing.
#include "tidbitsp.h"

/* table containing the next prime number for all power of twos from
   2^1 to 2^32 (the 2^32 prime is obviously less than 2^32 though) */
static const unsigned long coin_prime_table[32] = {
  2,
  5,
  11,
  17,
  37,
  67,
  131,
  257,
  521,
  1031,
  2053,
  4099,
  8209,
  16411,
  32771,
  65537,
  131101,
  262147,
  524309,
  1048583,
  2097169,
  4194319,
  8388617,
  16777259,
  33554467,
  67108879,
  134217757,
  268435459,
  536870923,
  1073741827,
  2147483659U,
  4294967291U /* 2^32 = 4294967296 */
};

unsigned long
coin_geq_prime_number(unsigned long num)
{
  int i;
  for (i = 0; i < 32; i++) {
    if (coin_prime_table[i] >= num) {
      return coin_prime_table[i];
    }
  }
  /* just return num if we can't find a bigger prime number */
  return num;
}
