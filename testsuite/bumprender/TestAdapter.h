#ifndef COIN_BUMP_TEST_ADAPTER_H
#define COIN_BUMP_TEST_ADAPTER_H

#include <Inventor/SoDB.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <memory>
#include <map>
#include <mutex>
#include <vector>
#include <cstdio>
#include <cstdlib>

#define CHECK(expr) do { if (!(expr)) { \
  std::fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #expr); \
  std::abort(); } } while (0)

// Compile the production implementation under a separate private class name.
// Only context/state bookkeeping is adapted; no cache logic is copied here.
#define soshape_bumprender CoinBumpTestRenderer
#define private public
#include "shapenodes/soshape_bumprender.h"
#undef private

struct BumpTestCacheElement {
  static void invalidate(SoState *) { ++invalidations(); }
  static int & invalidations() { static int count = 0; return count; }
};
struct BumpTestCacheContext {
  struct Callback { uint32_t id; void (*func)(void *, uint32_t); void * closure; };
  enum { DONT_AUTO_CACHE = 0 };
  static uint32_t & current() { static thread_local uint32_t id = 1; return id; }
  static int get(SoState *) { return (int) current(); }
  static void shouldAutoCache(SoState *, int) { }
  static std::vector<Callback> & queue() { static std::vector<Callback> q; return q; }
  static void scheduleDeleteCallback(uint32_t id, void (*func)(void *, uint32_t), void * closure) {
    queue().push_back(Callback{id, func, closure});
  }
  static void flush(uint32_t id) {
    std::vector<Callback> calls;
    for (size_t i = 0; i < queue().size();) {
      if (queue()[i].id == id) {
        calls.push_back(queue()[i]);
        queue().erase(queue().begin() + i);
      }
      else ++i;
    }
    for (size_t i = 0; i < calls.size(); ++i) calls[i].func(calls[i].closure, id);
  }
};

#define SoCacheElement BumpTestCacheElement
#define SoGLCacheContextElement BumpTestCacheContext

#endif
