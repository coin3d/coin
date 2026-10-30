#ifndef COIN_BUMP_TEST_ADAPTER_H
#define COIN_BUMP_TEST_ADAPTER_H

// Keep windows.h from replacing std::numeric_limits<T>::max().
#ifndef NOMINMAX
#define NOMINMAX
#endif

// This executable consumes Coin's public DLL classes even though it compiles
// the private bump renderer below. Do not define imported Pimpl methods here:
// MSVC rejects their out-of-line definitions with C2491. The public declarations
// are enough; the wrapper implementations remain inside Coin.
#ifdef _WIN32
#pragma push_macro("COIN_INTERNAL")
#undef COIN_INTERNAL
#include <Inventor/tools/SbPimplPtr.h>
#pragma pop_macro("COIN_INTERNAL")
#endif

#include <Inventor/SoDB.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoContextHandler.h>
#include <memory>
#include <map>
#include <mutex>
#include <new>
#include <vector>
#include <cstdio>
#include <cstdlib>

#include "glue/glp.h"
#include "rendering/SoGL.h"
#include "tidbitsp.h"

#define CHECK(expr) do { if (!(expr)) { \
  std::fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #expr); \
  std::abort(); } } while (0)

// The private implementation is compiled into this executable, not into Coin.
// Use the public client cleanup API so shared-library builds on Windows do not
// need coin_atexit_func exported. Client cleanup runs before Coin's own context
// dispatcher cleanup, preserving the ordering needed by this test registry.
inline void bumpTestAtExit(coin_atexit_f * func, coin_atexit_priorities priority) {
  CHECK(priority == CC_ATEXIT_NORMAL);
  cc_coin_atexit(func);
}
#undef coin_atexit
#define coin_atexit(func, priority) bumpTestAtExit(func, priority)

// Only the private cache/upload entry points are exercised here. Keep the two
// unexported drawing helpers out of the executable's DLL imports, and fail if a
// test accidentally starts using those unadapted drawing paths. GLXTest's real
// SoShape integration still exercises the original helpers inside Coin.
inline const cc_glglue * bumpTestStateGlue(const SoState *) {
  CHECK(false);
  return NULL;
}
inline void bumpTestNormalizationCubeMap(const cc_glglue *) { CHECK(false); }
#define sogl_glue_instance bumpTestStateGlue
#define coin_apply_normalization_cube_map bumpTestNormalizationCubeMap

// Compile the production implementation under a separate private class name.
// Adapt external context/platform dependencies, but keep the production cache
// and upload logic unchanged; no cache logic is copied into this adapter.
#define soshape_bumprender CoinBumpTestRenderer
// The included implementation also defines this global. Keep the fixture's
// copy separate from Coin's copy when linking against the static library.
#define bumphack coinBumpTestHack
#define private public
#include "shapenodes/soshape_bumprender.h"
#undef private

struct BumpTestCacheElement {
  static void invalidate(SoState *) { ++invalidations(); }
  static int & invalidations() { static int count = 0; return count; }
};
struct BumpTestForeignFailure { };
// Faults occur before the real provider is called: its own partial-failure
// guarantees are deliberately not simulated by this private-cache fixture.
struct BumpTestContextHandler {
  typedef SoContextHandler::ContextDestructionCB ContextDestructionCB;
  enum Failure { NONE, ALLOCATION, FOREIGN };
  static Failure & failNextRegistration() { static Failure failure = NONE; return failure; }
  static uintptr_t & lastToken() { static uintptr_t token = 0; return token; }
  static void addContextDestructionCallback(ContextDestructionCB * func, void * closure) {
    lastToken() = (uintptr_t) closure;
    const Failure failure = failNextRegistration();
    failNextRegistration() = NONE;
    if (failure == ALLOCATION) throw std::bad_alloc();
    if (failure == FOREIGN) throw BumpTestForeignFailure();
    SoContextHandler::addContextDestructionCallback(func, closure);
  }
  static void removeContextDestructionCallback(ContextDestructionCB * func, void * closure) {
    SoContextHandler::removeContextDestructionCallback(func, closure);
  }
  static void destructingContext(uint32_t id) { SoContextHandler::destructingContext(id); }
};
struct BumpTestCacheContext {
  struct Callback { uint32_t id; void (*func)(void *, uint32_t); void * closure; };
  enum { DONT_AUTO_CACHE = 0 };
  static uint32_t & current() { static thread_local uint32_t id = 1; return id; }
  static int get(SoState *) { return (int) current(); }
  static void shouldAutoCache(SoState *, int) { }
  static std::vector<Callback> & queue() { static std::vector<Callback> q; return q; }
  static bool & failNextScheduling() { static bool fail = false; return fail; }
  static void scheduleDeleteCallback(uint32_t id, void (*func)(void *, uint32_t), void * closure) {
    if (failNextScheduling()) {
      failNextScheduling() = false;
      throw std::bad_alloc();
    }
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
#define SoContextHandler BumpTestContextHandler

#endif
