#ifndef COIN_AUDITOR_LIST_TEST_SUPPORT_H
#define COIN_AUDITOR_LIST_TEST_SUPPORT_H
#include <Inventor/SoDB.h>
#include <Inventor/lists/SoAuditorList.h>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include <thread>

#define CHECK(c) do { if (!(c)) { std::fprintf(stderr,"line %d: %s\n",__LINE__,#c); std::exit(1); } } while (0)
namespace allocation {
static long remaining = -1;
static bool armed = false;
static void * allocate(std::size_t n) {
  if (armed && remaining-- == 0) throw std::bad_alloc();
  void * p = std::malloc(n ? n : 1);
  if (!p) throw std::bad_alloc();
  return p;
}
static void arm(long n) { remaining=n; armed=true; }
static void disarm() { armed=false; }
}
void * operator new(std::size_t n) { return allocation::allocate(n); }
void * operator new[](std::size_t n) { return allocation::allocate(n); }
void operator delete(void * p) noexcept { std::free(p); }
void operator delete[](void * p) noexcept { std::free(p); }
#if __cplusplus >= 201402L
void operator delete(void * p, std::size_t) noexcept { std::free(p); }
void operator delete[](void * p, std::size_t) noexcept { std::free(p); }
#endif

static void unlocked(bool = false) {
  std::atomic<bool> done(false);
  std::thread t([&done]() {
    SoAuditorList l;
    int p;
    l.append(&p,SoNotRec::PARENT);
    l.remove(0);
    done.store(true);
  });
  const auto deadline=std::chrono::steady_clock::now()+std::chrono::seconds(2);
  while (!done.load()) {
    if (std::chrono::steady_clock::now()>deadline) {
      std::fputs("notification lock stranded\n",stderr);
      std::_Exit(93);
    }
    std::this_thread::yield();
  }
  t.join();
}
static int occurrences(const SoAuditorList & l, void * p, SoNotRec::Type type) {
  int n=0; for(int i=0;i<l.getLength();++i)
    if(l.getObject(i)==p && l.getType(i)==type) ++n;
  return n;
}
#endif
