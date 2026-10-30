/* Exercise the exported API from C, without private headers or implementation
   inclusion. This client can also be built against the previous public header. */
#include <Inventor/C/base/hash.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>

#define CHECK(condition) do { if (!(condition)) { \
  fprintf(stderr, "line %d: %s\n", __LINE__, #condition); return 1; \
} } while (0)

struct ApplyState {
  cc_hash * hash;
  unsigned int visits;
  int failed;
};

static cc_hash_key collide(cc_hash_key key)
{
  (void) key;
  return 1;
}

static cc_hash_key mixed(cc_hash_key key)
{
  return (key ^ (key >> 17)) * (cc_hash_key) 2654435761U;
}

static void remove_current(cc_hash_key key, void * value, void * closure)
{
  struct ApplyState * state = (struct ApplyState *) closure;
  void * found = NULL;
  ++state->visits;
  if (!cc_hash_get(state->hash, key, &found) || found != value ||
      !cc_hash_remove(state->hash, key)) state->failed = 1;
}

static int api_boundaries(void)
{
  const cc_hash_key maximum = (cc_hash_key) -1;
  const cc_hash_key keys[] = { 0, 17, 34, maximum, maximum - 1,
    (cc_hash_key) 1 << (sizeof(cc_hash_key) * CHAR_BIT / 2), 0x1000 };
  cc_hash_func * functions[] = { collide, mixed, NULL, collide };
  int values[sizeof(keys) / sizeof(keys[0])];
  const unsigned int count = sizeof(keys) / sizeof(keys[0]);
  unsigned int i, pass;
  void * found = &values[0];
  cc_hash * hash = cc_hash_construct(0, 0.0f);
  struct ApplyState state;
  CHECK(hash != NULL && cc_hash_get_num_elements(hash) == 0);
  CHECK(!cc_hash_get(hash, 99, &found) && found == &values[0]);
  CHECK(!cc_hash_remove(hash, 99));
  for (i = 0; i < count; ++i) {
    values[i] = (int) i;
    CHECK(cc_hash_put(hash, keys[i], i == 0 ? NULL : &values[i]));
  }
  CHECK(cc_hash_get(hash, 0, &found) && found == NULL);
  CHECK(!cc_hash_put(hash, 0, &values[0]));
  CHECK(cc_hash_get_num_elements(hash) == count);
  for (pass = 0; pass < sizeof(functions) / sizeof(functions[0]); ++pass) {
    cc_hash_set_hash_func(hash, functions[pass]);
    for (i = 0; i < count; ++i) {
      found = NULL;
      CHECK(cc_hash_get(hash, keys[i], &found) && found == &values[i]);
      CHECK(!cc_hash_put(hash, keys[i], &values[i]));
    }
    CHECK(cc_hash_get_num_elements(hash) == count);
  }
  state.hash = hash; state.visits = 0; state.failed = 0;
  cc_hash_apply(hash, remove_current, &state);
  CHECK(!state.failed && state.visits == count);
  CHECK(cc_hash_get_num_elements(hash) == 0);
  cc_hash_set_hash_func(hash, NULL);
  CHECK(cc_hash_put(hash, maximum, NULL));
  cc_hash_clear(hash);
  cc_hash_clear(hash);
  CHECK(cc_hash_get_num_elements(hash) == 0);
  cc_hash_destruct(hash);
  return 0;
}

static int collision_positions_and_reuse(void)
{
  int values[512];
  unsigned int cycle, i;
  cc_hash * hash = cc_hash_construct(1, 0.25f);
  cc_hash_set_hash_func(hash, collide);
  for (cycle = 0; cycle < 3; ++cycle) {
    for (i = 0; i < 512; ++i) {
      values[i] = (int) i;
      CHECK(cc_hash_put(hash, i, &values[i]));
    }
    CHECK(cc_hash_get_num_elements(hash) == 512);
    for (i = 0; i < 512; ++i) {
      void * found = NULL;
      CHECK(cc_hash_get(hash, i, &found) && found == &values[i]);
    }
    /* A permutation removes the head, tail and middle of the collision chain. */
    for (i = 0; i < 512; ++i) {
      cc_hash_key key = (i * 131U) % 512;
      CHECK(cc_hash_remove(hash, key));
      CHECK(!cc_hash_remove(hash, key));
    }
    CHECK(cc_hash_get_num_elements(hash) == 0);
    cc_hash_clear(hash);
  }
  cc_hash_destruct(hash);
  return 0;
}

static unsigned int random_next(unsigned int * state)
{
  *state = *state * 1664525U + 1013904223U;
  return *state;
}

static int operations_match_model(void)
{
  int present[256] = { 0 };
  int values[256];
  void * expected[256] = { NULL };
  unsigned int random = 0x48415348U, count = 0, operation, i;
  cc_hash * hash = cc_hash_construct(1, 0.5f);
  for (operation = 0; operation < 20000; ++operation) {
    unsigned int key = (random_next(&random) >> 16) % 256;
    unsigned int choice = (random_next(&random) >> 16) % 100;
    if (operation % 1021 == 0) {
      cc_hash_clear(hash);
      for (i = 0; i < 256; ++i) present[i] = 0;
      count = 0;
      cc_hash_set_hash_func(hash, (operation & 1) ? collide : mixed);
    }
    if (choice < 45) {
      void * value = (choice & 1) ? &values[key] : NULL;
      CHECK(cc_hash_put(hash, key, value) == (present[key] ? FALSE : TRUE));
      if (!present[key]) ++count;
      present[key] = 1; expected[key] = value;
    }
    else if (choice < 70) {
      CHECK(cc_hash_remove(hash, key) == (present[key] ? TRUE : FALSE));
      if (present[key]) { --count; present[key] = 0; }
    }
    else {
      void * found = &values[0];
      CHECK(cc_hash_get(hash, key, &found) == (present[key] ? TRUE : FALSE));
      CHECK(found == (present[key] ? expected[key] : &values[0]));
    }
    CHECK(cc_hash_get_num_elements(hash) == count);
    if (operation % 137 == 0) {
      cc_hash_set_hash_func(hash, (operation & 1) ? collide : NULL);
      for (i = 0; i < 256; ++i) {
        void * found = &values[0];
        CHECK(cc_hash_get(hash, i, &found) == (present[i] ? TRUE : FALSE));
        CHECK(found == (present[i] ? expected[i] : &values[0]));
      }
    }
  }
  cc_hash_destruct(hash);
  return 0;
}

static int loadfactor_boundaries(void)
{
  const float factors[] = { 0.0f, -1.0f, 0.01f, 1.0f, 2.0f,
    FLT_MAX, FLT_MIN, NAN, INFINITY, -INFINITY };
  unsigned int i;
  for (i = 0; i < sizeof(factors) / sizeof(factors[0]); ++i) {
    void * found = (void *) factors;
    cc_hash * hash = cc_hash_construct(2, factors[i]);
    CHECK(cc_hash_put(hash, 0, NULL));
    CHECK(cc_hash_put(hash, (cc_hash_key) -1, NULL));
    CHECK(cc_hash_get(hash, 0, &found) && found == NULL);
    CHECK(cc_hash_get_num_elements(hash) == 2);
    cc_hash_destruct(hash);
  }
  return 0;
}

int main(void)
{
  if (api_boundaries() || collision_positions_and_reuse() ||
      operations_match_model() || loadfactor_boundaries()) return 1;
  puts("cc_hash C API, rehash, traversal, numeric and model tests passed.");
  return 0;
}
