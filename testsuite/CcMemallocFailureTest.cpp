// Compile the production allocator here so malloc failures can be injected
// without affecting other parts of Coin.
#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <Inventor/C/base/memalloc.h>

static unsigned int malloc_calls = 0;
static unsigned int fail_on_call = 0;
static size_t last_malloc_size = 0;
static unsigned int live_allocations = 0;

static void *
test_malloc(size_t size)
{
  ++malloc_calls;
  last_malloc_size = size;
  if (malloc_calls == fail_on_call) return NULL;
  void * ptr = std::malloc(size);
  if (ptr != NULL) ++live_allocations;
  return ptr;
}

static void
test_free(void * ptr)
{
  if (ptr != NULL) --live_allocations;
  std::free(ptr);
}

#define malloc(size) test_malloc(size)
#define free(ptr) test_free(ptr)
#include "../src/base/memalloc.cpp"
#undef malloc
#undef free

#define CHECK(condition) do { if (!(condition)) { \
  std::fprintf(stderr, "line %d: %s\n", __LINE__, #condition); \
  return false; } } while (false)

static int
one_unit_strategy(int)
{
  return 1;
}

static bool
fundamental_alignment()
{
  for (unsigned int size = 0; size <= 64; ++size) {
    cc_memalloc * allocator = cc_memalloc_construct(size);
    CHECK(allocator != NULL);
    void * units[130];
    for (unsigned int i = 0; i < 130; ++i) {
      units[i] = cc_memalloc_allocate(allocator);
      CHECK(units[i] != NULL);
      CHECK(reinterpret_cast<uintptr_t>(units[i]) %
            alignof(std::max_align_t) == 0);
      std::memset(units[i], static_cast<int>(i), size);
    }
    for (unsigned int i = 0; i < 130; ++i) {
      const unsigned char * bytes = static_cast<unsigned char *>(units[i]);
      for (unsigned int j = 0; j < size; ++j) {
        CHECK(bytes[j] == static_cast<unsigned char>(i));
      }
    }
    for (unsigned int i = 0; i < 130; ++i) {
      cc_memalloc_deallocate(allocator, units[i]);
    }
    for (unsigned int i = 130; i > 0; --i) {
      CHECK(cc_memalloc_allocate(allocator) == units[i - 1]);
    }
    cc_memalloc_clear(allocator);
    CHECK(cc_memalloc_allocate(allocator) != NULL);
    cc_memalloc_destruct(allocator);
  }
  return true;
}

static bool
explicit_alignment()
{
  const unsigned int size = 3 * sizeof(void *);
  cc_memalloc * allocator =
    cc_memalloc_construct_aligned(size, alignof(void *));
  CHECK(allocator != NULL);
  CHECK(allocator->chunksize == size);
  for (unsigned int i = 0; i < 4; ++i) {
    void * unit = cc_memalloc_allocate(allocator);
    CHECK(unit != NULL);
    CHECK(reinterpret_cast<uintptr_t>(unit) % alignof(void *) == 0);
  }
  cc_memalloc_destruct(allocator);

  CHECK(cc_memalloc_construct_aligned(size, 0) == NULL);
  CHECK(cc_memalloc_construct_aligned(size, 3) == NULL);
  CHECK(cc_memalloc_construct_aligned(
    size, 2 * alignof(std::max_align_t)) == NULL);
  return true;
}

static bool
mixed_lifecycle()
{
  cc_memalloc * allocator = cc_memalloc_construct(17);
  CHECK(allocator != NULL);
  std::vector<unsigned char *> live;
  std::vector<unsigned char> values;
  unsigned int state = 0x9e3779b9U;
  for (unsigned int step = 0; step < 10000; ++step) {
    state = state * 1664525U + 1013904223U;
    if (step % 997 == 0) {
      cc_memalloc_clear(allocator);
      live.clear();
      values.clear();
    }
    else if (!live.empty() && (live.size() == 128 || state % 3 == 0)) {
      const size_t index = state % live.size();
      for (unsigned int i = 0; i < 17; ++i) {
        CHECK(live[index][i] == values[index]);
      }
      cc_memalloc_deallocate(allocator, live[index]);
      live[index] = live.back();
      live.pop_back();
      values[index] = values.back();
      values.pop_back();
    }
    else {
      unsigned char * unit =
        static_cast<unsigned char *>(cc_memalloc_allocate(allocator));
      CHECK(unit != NULL);
      CHECK(reinterpret_cast<uintptr_t>(unit) %
            alignof(std::max_align_t) == 0);
      for (size_t i = 0; i < live.size(); ++i) CHECK(unit != live[i]);
      std::memset(unit, static_cast<int>(step % 256), 17);
      live.push_back(unit);
      values.push_back(static_cast<unsigned char>(step % 256));
    }
  }
  cc_memalloc_destruct(allocator);
  return true;
}

static bool
construction_failure()
{
  fail_on_call = malloc_calls + 1;
  CHECK(cc_memalloc_construct(9) == NULL);
  fail_on_call = 0;
  CHECK(cc_memalloc_construct(UINT_MAX) == NULL);
  return true;
}

static bool
large_unit_default_strategy()
{
  const unsigned int unitsize = UINT_MAX / 64 + 1;
  cc_memalloc * allocator = cc_memalloc_construct(unitsize);
  CHECK(allocator != NULL);
  const unsigned int calls_before = malloc_calls;
  fail_on_call = malloc_calls + 2;
  CHECK(cc_memalloc_allocate(allocator) == NULL);
  CHECK(malloc_calls == calls_before + 2);
  CHECK(last_malloc_size == unitsize);
  CHECK(allocator->num_allocated_units == 0);
  CHECK(allocator->memnode == NULL);
  fail_on_call = 0;
  cc_memalloc_destruct(allocator);
  return true;
}

static bool
first_block_failure(unsigned int allocation_offset)
{
  cc_memalloc * allocator = cc_memalloc_construct(9);
  CHECK(allocator != NULL);
  cc_memalloc_set_strategy(allocator, one_unit_strategy);

  const unsigned int live_before = live_allocations;
  fail_on_call = malloc_calls + allocation_offset;
  CHECK(cc_memalloc_allocate(allocator) == NULL);
  CHECK(allocator->num_allocated_units == 0);
  CHECK(allocator->memnode == NULL);
  CHECK(allocator->free == NULL);
  CHECK(live_allocations == live_before);

  fail_on_call = 0;
  void * recovered = cc_memalloc_allocate(allocator);
  CHECK(recovered != NULL);
  CHECK(allocator->num_allocated_units == 1);
  cc_memalloc_deallocate(allocator, recovered);
  cc_memalloc_destruct(allocator);
  return true;
}

static bool
growth_failure(unsigned int allocation_offset)
{
  cc_memalloc * allocator = cc_memalloc_construct(9);
  CHECK(allocator != NULL);
  cc_memalloc_set_strategy(allocator, one_unit_strategy);
  void * first = cc_memalloc_allocate(allocator);
  CHECK(first != NULL);
  std::memset(first, 0x5a, 9);
  cc_memalloc_memnode * original_node = allocator->memnode;
  const unsigned int original_position = original_node->currpos;
  const unsigned int live_before = live_allocations;

  fail_on_call = malloc_calls + allocation_offset;
  CHECK(cc_memalloc_allocate(allocator) == NULL);
  CHECK(allocator->num_allocated_units == 1);
  CHECK(allocator->memnode == original_node);
  CHECK(allocator->memnode->next == NULL);
  CHECK(allocator->memnode->currpos == original_position);
  CHECK(live_allocations == live_before);
  for (unsigned int i = 0; i < 9; ++i)
    CHECK(static_cast<unsigned char *>(first)[i] == 0x5a);

  // Reusing an existing unit needs no new block, even after failed growth.
  const unsigned int calls_before_reuse = malloc_calls;
  cc_memalloc_deallocate(allocator, first);
  CHECK(cc_memalloc_allocate(allocator) == first);
  CHECK(malloc_calls == calls_before_reuse);
  CHECK(allocator->num_allocated_units == 1);

  fail_on_call = 0;
  void * second = cc_memalloc_allocate(allocator);
  CHECK(second != NULL && second != first);
  CHECK(allocator->num_allocated_units == 2);
  cc_memalloc_deallocate(allocator, second);
  cc_memalloc_deallocate(allocator, first);
  cc_memalloc_destruct(allocator);
  return true;
}

static int strategy_result = 0;
static int strategy_input = 0;

static int
controlled_strategy(int count)
{
  strategy_input = count;
  return strategy_result;
}

static bool
invalid_strategy_recovery()
{
  cc_memalloc * allocator = cc_memalloc_construct(64);
  CHECK(allocator != NULL);
  cc_memalloc_set_strategy(allocator, controlled_strategy);
  const unsigned int calls_before = malloc_calls;
  const int invalid[] = { 0, -1, INT_MAX };
  for (unsigned int i = 0; i < sizeof(invalid) / sizeof(invalid[0]); ++i) {
    strategy_result = invalid[i];
    CHECK(cc_memalloc_allocate(allocator) == NULL);
    CHECK(strategy_input == 1);
    CHECK(allocator->num_allocated_units == 0);
    CHECK(allocator->memnode == NULL);
    CHECK(malloc_calls == calls_before);
  }
  cc_memalloc_set_strategy(allocator, NULL);
  CHECK(cc_memalloc_allocate(allocator) != NULL);
  CHECK(allocator->num_allocated_units == 1);
  cc_memalloc_destruct(allocator);
  return true;
}

static bool
clear_accounting()
{
  cc_memalloc * allocator = cc_memalloc_construct(9);
  CHECK(allocator != NULL);
  cc_memalloc_set_strategy(allocator, one_unit_strategy);
  CHECK(cc_memalloc_allocate(allocator) != NULL);
  CHECK(cc_memalloc_allocate(allocator) != NULL);
  cc_memalloc_clear(allocator);
  CHECK(allocator->num_allocated_units == 0);
  CHECK(allocator->memnode == NULL);
  CHECK(allocator->free == NULL);
  CHECK(cc_memalloc_allocate(allocator) != NULL);
  CHECK(allocator->num_allocated_units == 1);
  cc_memalloc_destruct(allocator);
  return true;
}

int
main(int argc, char ** argv)
{
  // Modes let regressions be reproduced independently of earlier failures.
  if (argc == 2) {
    bool result = false;
    if (std::strcmp(argv[1], "alignment") == 0) result = fundamental_alignment();
    else if (std::strcmp(argv[1], "construction") == 0) result = construction_failure();
    else if (std::strcmp(argv[1], "first-node") == 0) result = first_block_failure(1);
    else if (std::strcmp(argv[1], "first-block") == 0) result = first_block_failure(2);
    else if (std::strcmp(argv[1], "growth-node") == 0) result = growth_failure(1);
    else if (std::strcmp(argv[1], "growth-block") == 0) result = growth_failure(2);
    else if (std::strcmp(argv[1], "strategy") == 0) result = invalid_strategy_recovery();
    else if (std::strcmp(argv[1], "clear") == 0) result = clear_accounting();
    else return 2;
    return result && live_allocations == 0 ? 0 : 1;
  }
  if (argc != 1) return 2;
  return fundamental_alignment() && explicit_alignment() &&
         mixed_lifecycle() &&
         construction_failure() && large_unit_default_strategy() &&
         first_block_failure(1) && first_block_failure(2) &&
         growth_failure(1) && growth_failure(2) &&
         invalid_strategy_recovery() && clear_accounting() &&
         live_allocations == 0 ? 0 : 1;
}
