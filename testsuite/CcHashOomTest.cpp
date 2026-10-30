#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>

#include "base/hashp.h"
#include "tidbitsp.h"

static bool fail_calloc = false;
static bool fail_malloc = false;
static bool fail_entry = false;
static bool fail_allocator = false;

static void * injected_calloc(size_t count, size_t size)
{
  if (fail_calloc) { fail_calloc = false; return NULL; }
  return std::calloc(count, size);
}

static void * injected_malloc(size_t size)
{
  if (fail_malloc) { fail_malloc = false; return NULL; }
  return std::malloc(size);
}

static void * injected_allocate(cc_memalloc * allocator)
{
  if (fail_entry) { fail_entry = false; return NULL; }
  return cc_memalloc_allocate(allocator);
}

static cc_memalloc * injected_allocator_construct(unsigned int size,
                                                  unsigned int align)
{
  if (fail_allocator) { fail_allocator = false; return NULL; }
  return cc_memalloc_construct_aligned(size, align);
}

#define calloc(count, size) injected_calloc(count, size)
#define malloc(size) injected_malloc(size)
#define cc_memalloc_allocate(allocator) injected_allocate(allocator)
#define cc_memalloc_construct_aligned(size, align) injected_allocator_construct(size, align)
#include "../src/base/hash.cpp"
#undef calloc
#undef malloc
#undef cc_memalloc_allocate
#undef cc_memalloc_construct_aligned

#define CHECK(condition) do { if (!(condition)) { \
  std::fprintf(stderr, "line %d: %s\n", __LINE__, #condition); \
  return 1; \
} } while (0)

int main()
{
  struct rlimit no_core = { 0, 0 };
  setrlimit(RLIMIT_CORE, &no_core);
  for (int stage = 0; stage < 4; ++stage) {
    const pid_t child = fork();
    if (child == 0) {
      if (stage == 0) fail_malloc = true;
      if (stage == 1) fail_calloc = true;
      if (stage == 2) fail_allocator = true;
      cc_hash * hash = cc_hash_construct(2, 0.25f);
      if (stage == 3) fail_entry = true;
      (void) cc_hash_put(hash, 7, hash);
      _exit(1);
    }
    if (child < 0) return 2;
    int status = 0;
    CHECK(waitpid(child, &status, 0) == child);
    CHECK(WIFSIGNALED(status) && WTERMSIG(status) == SIGABRT);
  }

  cc_hash * hash = cc_hash_construct(2, 0.25f);
  CHECK(hash != NULL);
  const unsigned int oldsize = hash->size;
  fail_calloc = true;
  CHECK(cc_hash_put(hash, 7, hash));
  CHECK(!fail_calloc);
  CHECK(hash->size == oldsize);
  void * found = NULL;
  CHECK(cc_hash_get(hash, 7, &found) && found == hash);
  CHECK(cc_hash_put(hash, 8, hash));
  CHECK(hash->size > oldsize);
  CHECK(cc_hash_get(hash, 7, &found) && found == hash);
  cc_hash_destruct(hash);
  return 0;
}
