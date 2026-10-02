// Compile the production allocator here so malloc failures can be injected
// without affecting other parts of Coin.
#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <Inventor/C/base/memalloc.h>

static unsigned int malloc_calls = 0;
static unsigned int fail_on_call = 0;

static void *
test_malloc(size_t size)
{
  ++malloc_calls;
  if (malloc_calls == fail_on_call) return NULL;
  return std::malloc(size);
}

#define malloc(size) test_malloc(size)
#include "../src/base/memalloc.cpp"
#undef malloc

#define CHECK(condition) do { if (!(condition)) { \
  std::fprintf(stderr, "line %d: %s\n", __LINE__, #condition); \
  return false; } } while (false)

static int
one_unit_strategy(int)
{
  return 1;
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
first_block_failure(unsigned int allocation_offset)
{
  cc_memalloc * allocator = cc_memalloc_construct(9);
  CHECK(allocator != NULL);
  cc_memalloc_set_strategy(allocator, one_unit_strategy);

  fail_on_call = malloc_calls + allocation_offset;
  CHECK(cc_memalloc_allocate(allocator) == NULL);
  CHECK(allocator->num_allocated_units == 0);
  CHECK(allocator->memnode == NULL);
  CHECK(allocator->free == NULL);

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
  cc_memalloc_memnode * original_node = allocator->memnode;

  fail_on_call = malloc_calls + allocation_offset;
  CHECK(cc_memalloc_allocate(allocator) == NULL);
  CHECK(allocator->num_allocated_units == 1);
  CHECK(allocator->memnode == original_node);
  CHECK(allocator->memnode->next == NULL);

  fail_on_call = 0;
  void * second = cc_memalloc_allocate(allocator);
  CHECK(second != NULL && second != first);
  CHECK(allocator->num_allocated_units == 2);
  cc_memalloc_deallocate(allocator, second);
  cc_memalloc_deallocate(allocator, first);
  cc_memalloc_destruct(allocator);
  return true;
}

int
main()
{
  return construction_failure() &&
         first_block_failure(1) && first_block_failure(2) &&
         growth_failure(1) && growth_failure(2) ? 0 : 1;
}
