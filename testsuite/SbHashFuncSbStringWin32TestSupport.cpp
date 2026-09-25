// The SbHash_hashes_c_strings_without_an_SbString_temporary test (in
// InternalMacroTest.cpp) calls SbHashFunc(const SbString &), a private
// Coin symbol defined out-of-line in src/base/SbString.cpp. On Windows
// shared-library builds this symbol is not exported from the Coin DLL,
// so linking CoinTests.exe against it fails. Compile a copy of the
// (small, self-contained) implementation directly into the test
// executable instead of exposing it as public API, matching the
// existing dict.cpp precedent used elsewhere in this file for the same
// kind of problem.
//
// Included through misc/SbHash.h (with COIN_INTERNAL defined, see
// CMakeLists.txt) so this definition matches the exact declaration
// InternalMacroTest.cpp calls against. SbHash.h only forward-declares
// SbString, so the full class definition is pulled in separately.
#include "misc/SbHash.h"
#include <Inventor/SbString.h>

namespace {
  unsigned int sbstring_hash(const unsigned char * str, int n) {
    unsigned long hash = 0;
    while (n--) {
      hash = (*str++) + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
  }
}

unsigned int
SbHashFunc(const SbString & key) noexcept
{
  const unsigned char * cKey = reinterpret_cast<const unsigned char *>(key.getString());
  return sbstring_hash(cKey, key.getLength());
}
