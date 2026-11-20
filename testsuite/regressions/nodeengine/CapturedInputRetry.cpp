#include "Fixture.h"
#include <Inventor/nodes/SoGroup.h>
#include <cstring>

class DrivingNode : public ProbeNode {
public:
  ProbeEngine * peer;
  bool run;
  DrivingNode() : peer(NULL), run(false) {}
protected:
  void evaluate() override {
    ++state.evaluations;
    if (run) { run = false; peer->evaluateWrapper(); }
    SO_ENGINE_OUTPUT(first, SoSFInt32, setValue(17));
    SO_ENGINE_OUTPUT(second, SoSFInt32, setValue(23));
  }
};
class DrivingEngine : public ProbeEngine {
public:
  ProbeEngine * peer;
  bool run;
  DrivingEngine() : peer(NULL), run(false) {}
protected:
  void evaluate() override {
    ++state.evaluations;
    if (run) { run = false; peer->evaluateWrapper(); }
    SO_ENGINE_OUTPUT(first, SoSFInt32, setValue(17));
    SO_ENGINE_OUTPUT(second, SoSFInt32, setValue(23));
  }
};
class CapturingEngine : public ProbeEngine {
public:
  ProbeNode * target;
  bool change, catchCallback, caught;
  CapturingEngine() : target(NULL), change(false), catchCallback(false), caught(false) {}
protected:
  void evaluate() override {
    ++state.evaluations;
    const int captured = input.getValue();
    if (change) {
      change = false;
      input.setValue(captured + 1);
      try { target->input.touch(); }
      catch (const std::runtime_error &) {
        if (!catchCallback) throw;
        caught = true;
      }
    }
    // Keep the pre-change input deliberately: a later callback evaluation
    // computes a newer result before this older invocation resumes writing.
    SO_ENGINE_OUTPUT(first, SoSFInt32, setValue(captured));
    SO_ENGINE_OUTPUT(second, SoSFInt32, setValue(captured));
  }
};
class ReadingParent : public SoGroup {
public:
  CapturingEngine * engine;
  SoSFInt32 * reader;
  bool armed, twice, fail;
  int calls, seen;
  ReadingParent() : engine(NULL), reader(NULL), armed(false), twice(false),
    fail(false), calls(0), seen(-1) {}
  void notify(SoNotList * list) override {
    if (armed) {
      armed = false; ++calls;
      seen = reader->getValue();
      CHECK(seen == 2);
      if (twice) {
        engine->input.setValue(3);
        seen = reader->getValue();
        CHECK(seen == 3);
      }
      if (fail) throw std::runtime_error("synchronous reader");
    }
    SoGroup::notify(list);
  }
protected: ~ReadingParent() override {}
};
int main(int argc, char ** argv) {
  CHECK(argc == 6); initProbes();
  const bool ordinaryDriver = !std::strcmp(argv[1], "engine");
  const bool mixed = !std::strcmp(argv[4], "mixed");
  DrivingNode * node = new DrivingNode; node->ref();
  DrivingEngine * driver = new DrivingEngine; driver->ref();
  CapturingEngine * engine = new CapturingEngine; engine->ref();
  ReadingParent * parent = new ReadingParent; parent->ref(); parent->addChild(node);
  SoSFInt32 driving, first, second, shared;
  shared.setValue(-1);
  CHECK(driving.connectFrom(ordinaryDriver ? &driver->first : &node->first));
  CHECK(first.connectFrom(&engine->first)); CHECK(second.connectFrom(&engine->second));
  engine->input.setValue(1);
  CHECK(first.getValue() == 1); CHECK(second.getValue() == 1);
  CHECK(shared.connectFrom(&engine->first));
  CHECK(shared.appendConnection(&engine->second));
  engine->input.touch();
  CHECK(shared.getValue() == 1);
  second.enableNotify(!mixed);
  CHECK(driving.getValue() == 17);
  node->peer = engine; driver->peer = engine; engine->target = node;
  engine->input.touch();
  parent->engine = engine;
  parent->reader = !std::strcmp(argv[2], "first") ? &first : &second;
  parent->twice = !std::strcmp(argv[3], "twice");
  parent->fail = !std::strcmp(argv[5], "throw"); engine->catchCallback = parent->fail;
  engine->change = true;
  if (ordinaryDriver) { directNotify(driver); driver->run = true; }
  else { node->first.touchSlaves(NULL, FALSE); node->run = true; }
  parent->armed = true;
  CHECK(driving.getValue() == 17);
  CHECK(parent->calls == 1); CHECK(parent->seen == (parent->twice ? 3 : 2));
  CHECK(engine->caught == parent->fail);
  CHECK(first.isNotifyEnabled()); CHECK(second.isNotifyEnabled() == !mixed);
  CHECK(shared.isNotifyEnabled());

  // The nested reader must not consume the outer invocation's retry. This
  // call must recompute from the current input, then leave the engine clean.
  const int before = engine->state.evaluations;
  engine->evaluateWrapper();
  CHECK(engine->state.evaluations == before + 1);
  CHECK(first.getValue() == engine->input.getValue());
  CHECK(second.getValue() == engine->input.getValue());
  CHECK(shared.getValue() == engine->input.getValue());
  engine->evaluateWrapper();
  CHECK(engine->state.evaluations == before + 1);
  CHECK(first.isNotifyEnabled()); CHECK(second.isNotifyEnabled() == !mixed);
  CHECK(shared.isNotifyEnabled());

  // Exception cases isolate wrapper retry from SoField/SoDB recovery.
  parent->armed = false;
  shared.disconnect(); driving.disconnect(); first.disconnect(); second.disconnect();
  parent->unref(); node->unref(); driver->unref(); engine->unref();
}
