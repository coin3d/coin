#include "base/dynarray.h"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <new>

static bool track_object_allocation = false;
static bool fail_array_allocation = false;
static void * tracked_object = NULL;
static bool tracked_object_deleted = false;

void *
operator new(std::size_t bytes)
{
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
  void * memory = std::malloc(bytes);
  if (memory == NULL) throw std::bad_alloc();
  return memory;
}

void
operator delete[](void * memory) noexcept
{
  std::free(memory);
}

int
main(void)
{
  int values[5] = { 0, 1, 2, 3, 4 };
  cc_dynarray * source = cc_dynarray_new();
  for (int i = 0; i < 5; ++i) cc_dynarray_append(source, &values[i]);

  track_object_allocation = true;
  fail_array_allocation = true;
  bool caught = false;
  try { cc_dynarray_duplicate(source); }
  catch (const std::bad_alloc &) { caught = true; }
  track_object_allocation = false;
  fail_array_allocation = false;

  const bool source_ok = cc_dynarray_length(source) == 5;
  const bool failure_safe = caught && tracked_object != NULL &&
    tracked_object_deleted && source_ok;

  cc_dynarray * copy = cc_dynarray_duplicate(source);
  bool copy_ok = copy != NULL && cc_dynarray_length(copy) == 5;
  for (unsigned int i = 0; i < 5 && copy_ok; ++i) {
    copy_ok = cc_dynarray_get(copy, i) == &values[i];
  }
  cc_dynarray_destruct(copy);
  cc_dynarray_destruct(source);
  if (!failure_safe || !copy_ok) {
    std::fprintf(stderr,
                 "caught=%d tracked=%d released=%d source_ok=%d copy_ok=%d\n",
                 caught, tracked_object != NULL, tracked_object_deleted,
                 source_ok, copy_ok);
  }
  return failure_safe && copy_ok ? 0 : 1;
}
