#ifndef COIN_ENGINE_TEST_ALLOCATION_H
#define COIN_ENGINE_TEST_ALLOCATION_H
#include <cstdlib>
#include <new>
static long allocationOrdinal = -1;
static bool allocationFailed = false;
static void (*allocationFailureHook)() = NULL;
static void * allocate(std::size_t n) {
  if (allocationOrdinal >= 0 && allocationOrdinal-- == 0) {
    allocationOrdinal = -1; allocationFailed = true; if (allocationFailureHook) allocationFailureHook(); throw std::bad_alloc();
  }
  void * p = std::malloc(n ? n : 1); if (!p) throw std::bad_alloc(); return p;
}
void * operator new(std::size_t n) { return allocate(n); }
void * operator new[](std::size_t n) { return allocate(n); }
void operator delete(void * p) noexcept { std::free(p); }
void operator delete[](void * p) noexcept { std::free(p); }
#endif
