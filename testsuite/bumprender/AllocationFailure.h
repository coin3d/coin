#ifndef COIN_BUMP_TEST_ALLOCATION_FAILURE_H
#define COIN_BUMP_TEST_ALLOCATION_FAILURE_H

#include <cstdlib>
#include <new>

// Only FailureTest.cpp includes this header. Other threads are unaffected.
namespace BumpTestAllocation {
thread_local int remaining = -1;
struct Scope {
  explicit Scope(int successfulAllocations = 0) { remaining = successfulAllocations; }
  ~Scope() { remaining = -1; }
  bool untouched() const { return remaining >= 0; }
private:
  Scope(const Scope &) = delete;
  Scope & operator=(const Scope &) = delete;
};
}

void * operator new(std::size_t size) {
  if (BumpTestAllocation::remaining == 0) {
    BumpTestAllocation::remaining = -1;
    throw std::bad_alloc();
  }
  if (BumpTestAllocation::remaining > 0) --BumpTestAllocation::remaining;
  void * memory = std::malloc(size ? size : 1);
  if (!memory) throw std::bad_alloc();
  return memory;
}
void operator delete(void * memory) noexcept { std::free(memory); }
void * operator new[](std::size_t size) { return ::operator new(size); }
void operator delete[](void * memory) noexcept { std::free(memory); }
#if defined(__cpp_sized_deallocation)
void operator delete(void * memory, std::size_t) noexcept { std::free(memory); }
void operator delete[](void * memory, std::size_t) noexcept { std::free(memory); }
#endif

#endif
