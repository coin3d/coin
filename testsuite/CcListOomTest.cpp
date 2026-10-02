#include <cstdio>
#include <cstdlib>
#include <cstddef>

static int fail_at = -1;
static int allocations = 0;

static void *
test_malloc(size_t size)
{
  if (allocations++ == fail_at) return NULL;
  return std::malloc(size);
}

#define malloc(size) test_malloc(size)
#include "../src/base/list.cpp"
#undef malloc

#define CHECK(condition) do { if (!(condition)) { \
  std::fprintf(stderr, "line %d: %s\n", __LINE__, #condition); \
  return 1; \
} } while (0)

int
main()
{
  fail_at = allocations;
  CHECK(cc_list_construct() == NULL);

  fail_at = allocations + 1;
  CHECK(cc_list_construct_sized(8) == NULL);

  fail_at = -1;
  cc_list * list = cc_list_construct();
  CHECK(list != NULL);
  for (uintptr_t value = 1; value <= 4; ++value)
    CHECK(cc_list_try_append(list, reinterpret_cast<void *>(value)));

  fail_at = allocations;
  CHECK(!cc_list_try_append(list, reinterpret_cast<void *>(5)));
  CHECK(cc_list_get_length(list) == 4);
  CHECK(cc_list_get(list, 3) == reinterpret_cast<void *>(4));

  fail_at = allocations;
  CHECK(!cc_list_try_reserve(list, 16));
  CHECK(cc_list_get_length(list) == 4);
  fail_at = -1;
  CHECK(cc_list_try_reserve(list, 16));
  CHECK(cc_list_try_append(list, reinterpret_cast<void *>(5)));
  CHECK(cc_list_get_length(list) == 5);
  cc_list_destruct(list);
  return 0;
}
