#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include <new>
#include <climits>
static bool fail_buckets;
void * operator new[](std::size_t size) {
  if (fail_buckets) throw std::bad_alloc();
  void * p = std::malloc(size ? size : 1);
  if (!p) throw std::bad_alloc();
  return p;
}
void operator delete[](void * p) noexcept { std::free(p); }
void operator delete[](void * p, std::size_t) noexcept { std::free(p); }
void * operator new[](std::size_t n, const std::nothrow_t &) noexcept {
  try { return operator new[](n); } catch (...) { return NULL; }
}
void operator delete[](void * p, const std::nothrow_t &) noexcept { operator delete[](p); }

#include <Inventor/C/base/memalloc.h>
#include <Inventor/lists/SbList.h>

static bool fail_constructor = false;
static bool fail_entry = false;

static cc_memalloc * injected_construct(unsigned int size, unsigned int align)
{
  return fail_constructor ? NULL : cc_memalloc_construct_aligned(size, align);
}

static void * injected_allocate(cc_memalloc * allocator)
{
  return fail_entry ? NULL : cc_memalloc_allocate(allocator);
}

#define cc_memalloc_construct_aligned(size, align) injected_construct(size, align)
#define cc_memalloc_allocate(allocator) injected_allocate(allocator)
#include "misc/SbHash.h"
#undef cc_memalloc_construct_aligned
#undef cc_memalloc_allocate

int main()
{
  struct rlimit no_core = { 0, 0 };
  setrlimit(RLIMIT_CORE, &no_core);
  for (int stage = 0; stage < 4; ++stage) {
    const pid_t child = fork();
    if (child == 0) {
      fail_constructor = stage == 0;
      fail_buckets = stage == 2;
      SbHash<unsigned int, int> hash(stage == 3 ? UINT_MAX : 2);
      fail_entry = stage == 1;
      hash.put(1, 7);
      _exit(1);
    }
    if (child < 0) return 2;
    int status = 0;
    if (waitpid(child, &status, 0) != child ||
        !WIFSIGNALED(status) || WTERMSIG(status) != SIGABRT) {
      std::fprintf(stderr, "SbHash OOM stage %d did not abort safely\n", stage);
      return 1;
    }
  }

  SbHash<unsigned int, int> hash(2, 1e10f);
  hash.put(1, 7);
  int value = 0;
  if (!hash.get(1, value) || value != 7) return 1;
  return 0;
}
