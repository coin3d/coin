#include "Allocation.h"
#include "Fixture.h"
#include <Inventor/nodes/SoGroup.h>
#include <cstring>

static void failAgain() { allocationOrdinal = 0; }

class CatchingNode : public ProbeNode {
public:
  bool caught;
  int unprepared, failures;
  bool recurse;
  ProbeNode * peer;
  CatchingNode() : caught(false), unprepared(0), failures(0), recurse(false), peer(NULL) {}
protected:
  void evaluate() override {
    ++state.evaluations;
    if (state.dirtyDuringEvaluate) {
      state.dirtyDuringEvaluate = false;
      try { input.touch(); }
      catch (const std::bad_alloc &) { caught = true; ++failures; }
    }
    if (caught && recurse) {
      recurse = false;
      try { directNotify(this); }
      catch (const std::bad_alloc &) { ++failures; }
      evaluateWrapper();
      peer->evaluateWrapper();
    }
    if (caught) {
      for (int i = 0; i < first.getNumConnections(); ++i)
        if (first[i]->isNotifyEnabled()) ++unprepared;
      for (int i = 0; i < second.getNumConnections(); ++i)
        if (second[i]->isNotifyEnabled()) ++unprepared;
    }
    SO_ENGINE_OUTPUT(first, SoSFInt32, setValue(17));
    SO_ENGINE_OUTPUT(second, SoSFInt32, setValue(23));
  }
};
class ObservingField : public SoSFInt32 {
public:
  SoSFInt32 * observed;
  int calls, seen;
  ObservingField() : observed(NULL), calls(0), seen(-999) { setValue(-1); }
  void notify(SoNotList * list) override {
    if (observed && isNotifyEnabled()) { ++calls; seen = observed->getValue(); }
    SoSFInt32::notify(list);
  }
};
class SurroundingNode : public ProbeNode {
public:
  ProbeNode * inner;
  bool run;
  SurroundingNode() : inner(NULL), run(false) {}
protected:
  void evaluate() override {
    ++state.evaluations;
    if (run) { run = false; inner->evaluateWrapper(); }
    SO_ENGINE_OUTPUT(first, SoSFInt32, setValue(17));
    SO_ENGINE_OUTPUT(second, SoSFInt32, setValue(23));
  }
};
class AllocatingParent : public SoGroup {
public:
  ProbeNode * engine;
  SoSFInt32 * reader;
  ObservingField added[128];
  bool armed;
  long ordinal;
  bool persistent;
  AllocatingParent() : engine(NULL), reader(NULL), armed(false), ordinal(0), persistent(false) {}
  void notify(SoNotList * list) override {
    if (armed) {
      armed = false;
      CHECK(reader->getValue() == 23);
      for (int i = 0; i < 128; ++i) {
        added[i].setValue(-1);
        added[i].enableNotify((i % 3) != 0);
        CHECK(added[i].connectFrom(i < 64 ? &engine->first : &engine->second, TRUE));
      }
      // Fail only when the suspended outer scope prepares the enlarged
      // connection lists, after this callback and its nested read finish.
      added[65].observed = &added[127];
      if (persistent) allocationFailureHook = failAgain;
      allocationOrdinal = ordinal;
    }
    SoGroup::notify(list);
  }
protected:
  ~AllocatingParent() override {}
};

int main(int argc, char ** argv) {
  CHECK(argc == 4); initProbes();
  CatchingNode * engine = new CatchingNode; engine->ref();
  AllocatingParent * parent = new AllocatingParent; parent->ref();
  parent->engine = engine; parent->ordinal = std::strtol(argv[1], NULL, 10);
  parent->persistent = !std::strcmp(argv[2], "persistent");
  const bool chained = !std::strcmp(argv[3], "chained");
  engine->recurse = std::strcmp(argv[3], "normal") != 0;
  ProbeNode * peer = new ProbeNode; peer->ref(); engine->peer = peer;
  SurroundingNode * outer = new SurroundingNode; outer->ref(); outer->inner = engine;
  parent->addChild(engine);
  SoSFInt32 first, second;
  CHECK(first.connectFrom(&engine->first)); CHECK(second.connectFrom(&engine->second));
  CHECK(first.getValue() == 17); CHECK(second.getValue() == 23);
  CHECK(first.appendConnection(&peer->first, TRUE));
  if (chained) CHECK(first.appendConnection(&outer->first, TRUE));
  second.enableNotify(FALSE); parent->reader = &second; parent->armed = true;
  engine->state.dirtyDuringEvaluate = true;
  if (chained) { outer->run = true; outer->evaluateWrapper(); }
  else engine->evaluateWrapper();
  allocationOrdinal = -1; allocationFailureHook = NULL;
  CHECK(allocationFailed); CHECK(engine->caught); CHECK(engine->unprepared == 0);
  CHECK(engine->failures == (parent->persistent && std::strcmp(argv[3], "normal") ? 2 : 1));
  CHECK(parent->added[65].calls == 0);
  CHECK(first.isNotifyEnabled()); CHECK(!second.isNotifyEnabled());
  for (int i = 0; i < 128; ++i) {
    CHECK(parent->added[i].isNotifyEnabled() == ((i % 3) != 0));
    CHECK(parent->added[i].getValue() == (i < 64 ? 17 : 23));
  }
  // The emergency mask must be gone, and subsequent normal preparation
  // must preserve flags even when another engine shares a slave.
  engine->evaluateWrapper(); peer->evaluateWrapper();
  CHECK(first.isNotifyEnabled()); CHECK(!second.isNotifyEnabled());
  CHECK(parent->added[65].calls == 0);
  parent->armed = false;
  for (int i = 0; i < 128; ++i) parent->added[i].disconnect();
  first.disconnect(); second.disconnect(); parent->unref(); engine->unref(); peer->unref(); outer->unref();
  return 0;
}
