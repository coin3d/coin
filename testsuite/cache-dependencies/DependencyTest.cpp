// Dependencies must reach every open ancestor, across empty cache levels.
#include "../reproducers/CoinCleanup.h"
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/caches/SoCache.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoComplexityElement.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoCallback.h>
#include <cstdio>
#include <memory>

struct CacheUnref {
  void operator()(SoCache * cache) const { cache->unref(); }
};
typedef std::unique_ptr<SoCache, CacheUnref> CacheRef;

static CacheRef openCache(SoState * state)
{
  state->push();
  CacheRef cache(new SoCache(state));
  cache->ref();
  SoCacheElement::set(state, cache.get());
  return cache;
}

static void emptyLevels(SoState * state, int count)
{
  for (int i = 0; i < count; ++i) {
    state->push();
    SoCacheElement::set(state, NULL);
  }
}

struct Scenario {
  int top, middle;
  bool ok;
};

static void check(void * userdata, SoAction * action)
{
  Scenario & scenario = *static_cast<Scenario *>(userdata);
  SoState * state = action->getState();
  const SbBool invalid = SoCacheElement::setInvalid(FALSE);
  SoComplexityElement::set(state, 0.25f);

  CacheRef dependency = openCache(state);
  (void) SoComplexityElement::get(state);
  state->pop();

  // An empty stack is a no-op, not a reason to invalidate the dependency.
  SoCacheElement::addCacheDependency(state, dependency.get());
  const bool noopen = !SoCacheElement::anyOpen(state) && dependency->isValid(state);

  CacheRef outer = openCache(state);
  emptyLevels(state, scenario.middle);
  CacheRef inner = openCache(state);
  emptyLevels(state, scenario.top);
  const bool current = SoCacheElement::getCurrentCache(state) == inner.get();
  SoCacheElement::addCacheDependency(state, dependency.get());
  for (int i = 0; i < scenario.top + scenario.middle + 2; ++i) state->pop();

  const bool before = inner->isValid(state) && outer->isValid(state);
  SoComplexityElement::set(state, 0.25f);
  const bool unchanged = inner->isValid(state) && outer->isValid(state);
  SoComplexityElement::set(state, 0.75f);
  const bool innerinvalid = !inner->isValid(state);
  const bool outerinvalid = !outer->isValid(state);
  scenario.ok = noopen && current && before && unchanged && innerinvalid &&
    outerinvalid && !dependency->isValid(state);
  std::printf("top=%d middle=%d before=%d unchanged=%d inner_invalid=%d outer_invalid=%d valid=%d\n",
    scenario.top, scenario.middle, before, unchanged, innerinvalid, outerinvalid, scenario.ok);
  SoCacheElement::setInvalid(invalid);
}

int main()
{
  SoDB::init();
  CoinReproducerCleanup cleanup;
  Scenario scenarios[] = {{0,0,false}, {1,0,false}, {0,1,false}, {2,2,false}};
  bool ok = true;
  for (auto & scenario : scenarios) {
    SoCallback * callback = new SoCallback;
    callback->ref();
    callback->setCallback(check, &scenario);
    {
      SoCallbackAction action;
      action.apply(callback);
    }
    callback->unref();
    ok = ok && scenario.ok;
  }
  return ok ? 0 : 1;
}
