#ifndef COIN_BUMP_TEST_ALLOCATION_FAILURE_H
#define COIN_BUMP_TEST_ALLOCATION_FAILURE_H

#include <cstdlib>
#include <new>

// Only FailureTest.cpp includes this header. Other threads are unaffected.
namespace BumpTestAllocation {
thread_local bool failNext = false;
struct Scope {
  Scope() { failNext = true; }
  ~Scope() { failNext = false; }
  bool untouched() const { return failNext; }
private:
  Scope(const Scope &) = delete;
  Scope & operator=(const Scope &) = delete;
};
}

void * operator new(std::size_t size) {
  if (BumpTestAllocation::failNext) {
    BumpTestAllocation::failNext = false;
    throw std::bad_alloc();
  }
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
