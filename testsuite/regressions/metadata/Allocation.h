#ifndef COIN_METADATA_ALLOCATION_H
#define COIN_METADATA_ALLOCATION_H
#include <cstdlib>
#include <new>
#include <cstdio>
// Fixed bookkeeping keeps the test harness out of the armed allocation path.
static void * tracked[32768];
static unsigned live = 0;
static long failAt = -1;
static bool tracking = false, hit = false;
static void * allocateMetadataTest(std::size_t n) {
  if (tracking && failAt >= 0 && failAt-- == 0) {
    failAt = -1; hit = true; throw std::bad_alloc();
  }
  void * p = std::malloc(n ? n : 1);
  if (!p) throw std::bad_alloc();
  if (tracking) { if (live == 32768) std::abort(); tracked[live++] = p; }
  return p;
}
static void releaseMetadataTest(void * p) noexcept {
  for (unsigned i = 0; i < live; ++i) if (tracked[i] == p) {
    tracked[i] = tracked[--live]; break;
  }
  std::free(p);
}
void * operator new(std::size_t n) { return allocateMetadataTest(n); }
void * operator new[](std::size_t n) { return allocateMetadataTest(n); }
void operator delete(void * p) noexcept { releaseMetadataTest(p); }
void operator delete[](void * p) noexcept { releaseMetadataTest(p); }
#if __cplusplus >= 201402L
void operator delete(void * p, std::size_t) noexcept { releaseMetadataTest(p); }
void operator delete[](void * p, std::size_t) noexcept { releaseMetadataTest(p); }
#endif
static void arm(long ordinal) { if (live) std::abort(); hit = false; failAt = ordinal; tracking = true; }
static void disarm() { tracking = false; failAt = -1; }
#endif
