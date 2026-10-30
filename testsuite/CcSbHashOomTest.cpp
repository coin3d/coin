#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <cstddef>
#include <new>
#include <limits>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>

#include <Inventor/C/base/memalloc.h>
#include <Inventor/lists/SbList.h>

static bool fail_constructor = false;
static bool fail_entry = false;
static bool fail_buckets = false;
static unsigned int allocator_constructions = 0;

// Intercept only the nothrow bucket path. Successful allocations still use
// the ordinary array allocator and its matching delete[], including in ASan.
void * operator new[](std::size_t size, const std::nothrow_t &) noexcept
{
  if (fail_buckets) return NULL;
  try { return ::operator new[](size); }
  catch (...) { return NULL; }
}

void operator delete[](void * pointer, const std::nothrow_t &) noexcept
{
  ::operator delete[](pointer);
}

static cc_memalloc * injected_construct(unsigned int size, unsigned int align)
{
  ++allocator_constructions;
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

class HashProbe : public SbHash<unsigned int, int> {
public:
  HashProbe(unsigned int size = 2, float factor = 0.5f)
    : SbHash<unsigned int, int>(size, factor) {}
  bool allocated() const { return this->hasAllocatedStorage() != FALSE; }
  unsigned int buckets() const { return this->getNumBuckets(); }
  unsigned int threshold() const { return this->getResizeThreshold(); }
};

static bool check_lazy_storage_and_resize()
{
  allocator_constructions = 0;
  fail_constructor = true;
  fail_buckets = true;
  {
    HashProbe empty;
    SbHash<unsigned int, int> copy(empty), assigned;
    assigned = empty;
    SbList<unsigned int> keys;
    empty.makeKeyList(keys);
    int value = 0;
    if (empty.allocated() || allocator_constructions != 0 ||
        empty.get(0, value) || empty.erase(0) ||
        empty.begin() != empty.end() ||
        empty.find(0) != empty.const_end() || keys.getLength() != 0) return false;
    empty.clear();
    empty.releaseStorage();
  }
  if (allocator_constructions != 0) return false;
  fail_constructor = false;
  fail_buckets = false;

  HashProbe hash;
  const unsigned int buckets = hash.buckets();
  const unsigned int threshold = hash.threshold();
  for (unsigned int i = 0; i < threshold; ++i) hash.put(i, int(i + 7));
  if (!hash.allocated() || allocator_constructions != 1) return false;
  fail_buckets = true;
  hash.put(threshold, int(threshold + 7)); // Insert succeeds even if resize fails.
  fail_buckets = false;
  if (hash.buckets() != buckets || hash.getNumElements() != threshold + 1)
    return false;
  for (unsigned int i = 0; i <= threshold; ++i) {
    int value = 0;
    if (!hash.get(i, value) || value != int(i + 7)) return false;
  }
  hash.put(threshold + 1, int(threshold + 8));
  if (hash.buckets() <= buckets || hash.getNumElements() != threshold + 2)
    return false;
  for (unsigned int i = 0; i <= threshold + 1; ++i) {
    int value = 0;
    if (!hash.get(i, value) || value != int(i + 7)) return false;
  }
  hash.clear();
  if (!hash.allocated()) return false;
  hash.releaseStorage();
  if (hash.allocated()) return false;
  hash.put(77, 99);
  return allocator_constructions == 2 && hash.getNumElements() == 1;
}

int main()
{
  struct rlimit no_core = { 0, 0 };
  setrlimit(RLIMIT_CORE, &no_core);
  for (int stage = 0; stage < 3; ++stage) {
    const pid_t child = fork();
    if (child == 0) {
      fail_constructor = stage == 0;
      SbHash<unsigned int, int> hash(2);
      fail_entry = stage == 1;
      fail_buckets = stage == 2;
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

  if (!check_lazy_storage_and_resize()) {
    std::fprintf(stderr, "SbHash lazy storage or optional resize contract failed\n");
    return 1;
  }
  HashProbe nan_hash(2, std::numeric_limits<float>::quiet_NaN());
  HashProbe infinity_hash(2, std::numeric_limits<float>::infinity());
  HashProbe default_hash(2, 0.75f);
  if (nan_hash.threshold() != default_hash.threshold() ||
      infinity_hash.threshold() != default_hash.threshold()) return 1;

  SbHash<unsigned int, int> hash(2, 1e10f);
  hash.put(1, 7);
  int value = 0;
  if (!hash.get(1, value) || value != 7) return 1;
  return 0;
}
