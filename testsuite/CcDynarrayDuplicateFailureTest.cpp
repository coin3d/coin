#include "base/dynarray.h"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <new>

static bool track_object_allocation = false;
static bool fail_object_allocation = false;
static bool fail_array_allocation = false;
static void * tracked_object = NULL;
static bool tracked_object_deleted = false;

void *
operator new(std::size_t bytes)
{
  if (fail_object_allocation) {
    fail_object_allocation = false;
    throw std::bad_alloc();
  }
  if (bytes == 0) bytes = 1;
  void * memory = std::malloc(bytes);
  if (memory == NULL) throw std::bad_alloc();
  if (track_object_allocation) {
    tracked_object = memory;
    track_object_allocation = false;
  }
  return memory;
}

void
operator delete(void * memory) noexcept
{
  if (memory == tracked_object) tracked_object_deleted = true;
  std::free(memory);
}

void *
operator new[](std::size_t bytes)
{
  if (fail_array_allocation) {
    fail_array_allocation = false;
    throw std::bad_alloc();
  }
  if (bytes == 0) bytes = 1;
  void * memory = std::malloc(bytes);
  if (memory == NULL) throw std::bad_alloc();
  return memory;
}

void
operator delete[](void * memory) noexcept
{
  std::free(memory);
}

#if __cplusplus >= 201402L
void operator delete(void * memory, std::size_t) noexcept
{
  ::operator delete(memory);
}
void operator delete[](void * memory, std::size_t) noexcept
{
  ::operator delete[](memory);
}
#endif

static bool
has_values(const cc_dynarray * array, int * values)
{
  if (cc_dynarray_length(array) != 5) return false;
  for (unsigned int i = 0; i < 5; ++i) {
    if (cc_dynarray_get(array, i) != &values[i]) return false;
  }
  return true;
}

int
main(void)
{
  int values[5] = { 0, 1, 2, 3, 4 };
  cc_dynarray * source = cc_dynarray_new();
  for (int i = 0; i < 5; ++i) cc_dynarray_append(source, &values[i]);

  fail_object_allocation = true;
  bool object_caught = false;
  try { cc_dynarray_duplicate(source); }
  catch (const std::bad_alloc &) { object_caught = true; }
  fail_object_allocation = false;
  const bool object_failure_safe = object_caught && has_values(source, values);

  track_object_allocation = true;
  fail_array_allocation = true;
  bool caught = false;
  try { cc_dynarray_duplicate(source); }
  catch (const std::bad_alloc &) { caught = true; }
  track_object_allocation = false;
  fail_array_allocation = false;

  const bool source_ok = has_values(source, values);
  const bool failure_safe = caught && tracked_object != NULL &&
    tracked_object_deleted && source_ok;

  cc_dynarray * copy = cc_dynarray_duplicate(source);
  bool copy_ok = copy != NULL && has_values(copy, values);
  int replacement = 42;
  cc_dynarray_set(copy, 0, &replacement);
  copy_ok = copy_ok && cc_dynarray_get(copy, 0) == &replacement &&
    has_values(source, values);
  cc_dynarray_destruct(copy);
  copy_ok = copy_ok && has_values(source, values);
  cc_dynarray_destruct(source);
  if (!object_failure_safe || !failure_safe || !copy_ok) {
    std::fprintf(stderr,
                 "object_safe=%d caught=%d tracked=%d released=%d source_ok=%d copy_ok=%d\n",
                 object_failure_safe, caught, tracked_object != NULL, tracked_object_deleted,
                 source_ok, copy_ok);
  }
  return object_failure_safe && failure_safe && copy_ok ? 0 : 1;
}
