#define COIN_ALLOW_SBDICT
#define COIN_ALLOW_CC_HASH
#include <Inventor/SbDict.h>
#include <Inventor/C/base/hash.h>
#include "base/dict.h"
#include <cstdio>
#include <map>
#define CHECK(condition) do { if (!(condition)) { \
  std::fprintf(stderr, "line %d: %s\n", __LINE__, #condition); return 1; \
} } while (false)
static unsigned int next(unsigned int & state) { return state = state * 1664525u + 1013904223u; }
int main() {
  int values[256];
  SbDict publicdict(1);
  cc_dict * privatedict = cc_dict_construct(1, 0.75f);
  cc_hash * legacy = cc_hash_construct(1, 0.75f);
  CHECK(privatedict && legacy);
  std::map<uintptr_t, void *> expected;
  unsigned int random = 0x44494646u;
  for (unsigned int operation = 0; operation < 20000; ++operation) {
    const uintptr_t key = (next(random) >> 16) % 256;
    const unsigned int choice = (next(random) >> 16) % 100;
    if (operation % 1021 == 0) {
      publicdict.clear(); cc_dict_clear(privatedict); cc_hash_clear(legacy); expected.clear();
    }
    if (choice < 45) {
      void * value = (choice & 1) ? &values[key] : NULL;
      const SbBool inserted = expected.count(key) ? FALSE : TRUE;
      CHECK(publicdict.enter(key, value) == inserted);
      CHECK(cc_dict_put(privatedict, key, value) == inserted);
      CHECK(cc_hash_put(legacy, key, value) == inserted);
      expected[key] = value;
    }
    else if (choice < 70) {
      const SbBool removed = expected.erase(key) ? TRUE : FALSE;
      CHECK(publicdict.remove(key) == removed);
      CHECK(cc_dict_remove(privatedict, key) == removed);
      CHECK(cc_hash_remove(legacy, key) == removed);
    }
    else {
      void * a = &values[0], * b = a, * c = a;
      const auto entry = expected.find(key);
      const SbBool found = entry == expected.end() ? FALSE : TRUE;
      CHECK(publicdict.find(key, a) == found);
      CHECK(cc_dict_get(privatedict, key, &b) == found);
      CHECK(cc_hash_get(legacy, key, &c) == found);
      CHECK(a == b && b == c && a == (found ? entry->second : &values[0]));
    }
    CHECK(cc_dict_get_num_elements(privatedict) == expected.size());
    CHECK(cc_hash_get_num_elements(legacy) == expected.size());
  }
  cc_dict_destruct(privatedict); cc_hash_destruct(legacy);
  return 0;
}
