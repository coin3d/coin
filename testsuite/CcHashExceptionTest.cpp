// Keep this client separate from hash.cpp: /EHsc callers must not optimize away
// exception handlers around C-linkage API calls, including calls into a DLL.
#include <Inventor/C/base/hash.h>
#include <cstdio>

static_assert(!noexcept(cc_hash_put(NULL, 0, NULL)), "put must propagate hash exceptions");
static_assert(!noexcept(cc_hash_get(NULL, 0, NULL)), "get must propagate hash exceptions");
static_assert(!noexcept(cc_hash_remove(NULL, 0)), "remove must propagate hash exceptions");
static_assert(!noexcept(cc_hash_set_hash_func(NULL, NULL)), "rehash must propagate hash exceptions");
static_assert(!noexcept(cc_hash_apply(NULL, NULL, NULL)), "apply must propagate callback exceptions");

struct HashFailure {};
static unsigned int budget;
static cc_hash_key throwing_hash(cc_hash_key key)
{
  if (budget-- == 0) throw HashFailure();
  return key;
}
static cc_hash_key alternate_hash(cc_hash_key key)
{
  return throwing_hash(key) * 3;
}
static void throwing_apply(cc_hash_key, void *, void *) { throw HashFailure(); }

#define CHECK(condition) do { if (!(condition)) { \
  std::fprintf(stderr, "line %d: %s\n", __LINE__, #condition); return 1; \
} } while (false)
#define EXPECT_HASH_FAILURE(operation) do { \
  bool caught = false; \
  try { operation; } catch (const HashFailure &) { caught = true; } \
  CHECK(caught); \
} while (false)

int main()
{
  int values[4] = { 1, 2, 3, 4 };
  cc_hash * hash = cc_hash_construct(17, 1.0f);
  CHECK(hash != NULL);
  CHECK(cc_hash_put(hash, 0, &values[0]));
  CHECK(cc_hash_put(hash, 17, &values[1]));
  budget = 1;
  EXPECT_HASH_FAILURE(cc_hash_set_hash_func(hash, alternate_hash));
  void * found = NULL;
  CHECK(cc_hash_get(hash, 0, &found) && found == &values[0]);
  CHECK(cc_hash_get(hash, 17, &found) && found == &values[1]);

  budget = 16;
  cc_hash_set_hash_func(hash, throwing_hash);
  budget = 0;
  EXPECT_HASH_FAILURE(cc_hash_get(hash, 0, &found));
  budget = 0;
  EXPECT_HASH_FAILURE(cc_hash_remove(hash, 0));
  budget = 0;
  EXPECT_HASH_FAILURE(cc_hash_put(hash, 34, &values[2]));
  CHECK(cc_hash_get_num_elements(hash) == 2);
  EXPECT_HASH_FAILURE(cc_hash_apply(hash, throwing_apply, NULL));
  budget = 16;
  CHECK(cc_hash_get(hash, 0, &found) && found == &values[0]);
  CHECK(cc_hash_get(hash, 17, &found) && found == &values[1]);
  CHECK(!cc_hash_get(hash, 34, &found));
  cc_hash_destruct(hash);

  hash = cc_hash_construct(2, 1.0f);
  CHECK(hash != NULL);
  cc_hash_set_hash_func(hash, throwing_hash);
  budget = 16;
  CHECK(cc_hash_put(hash, 0, &values[0]));
  CHECK(cc_hash_put(hash, 2, &values[1]));
  budget = 2; // Initial lookup, one planned destination, then throw.
  EXPECT_HASH_FAILURE(cc_hash_put(hash, 4, &values[2]));
  CHECK(cc_hash_get_num_elements(hash) == 3);
  budget = 32;
  for (unsigned int i = 0; i < 3; ++i)
    CHECK(cc_hash_get(hash, i * 2, &found) && found == &values[i]);
  CHECK(cc_hash_put(hash, 6, &values[3]));
  CHECK(cc_hash_get_num_elements(hash) == 4);
  for (unsigned int i = 0; i < 4; ++i)
    CHECK(cc_hash_get(hash, i * 2, &found) && found == &values[i]);
  cc_hash_destruct(hash);
  return 0;
}
