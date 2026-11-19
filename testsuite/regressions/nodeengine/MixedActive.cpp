#include "Fixture.h"
#include <Inventor/nodes/SoGroup.h>
#include <cstring>

class MixedNode : public ProbeNode {
public:
  ProbeEngine * peer;
  ProbeNode * target;
  bool run, touch;
  MixedNode() : peer(NULL), target(NULL), run(false), touch(false) {}
protected:
  void evaluate() override {
    ++state.evaluations;
    if (run) { run = false; peer->evaluateWrapper(); }
    if (touch) { touch = false; target->input.touch(); }
    SO_ENGINE_OUTPUT(first, SoSFInt32, setValue(17));
    SO_ENGINE_OUTPUT(second, SoSFInt32, setValue(23));
  }
};
class MixedEngine : public ProbeEngine {
public:
  ProbeNode * peer;
  ProbeNode * target;
  bool run, touch;
  MixedEngine() : peer(NULL), target(NULL), run(false), touch(false) {}
protected:
  void evaluate() override {
    ++state.evaluations;
    if (run) { run = false; peer->evaluateWrapper(); }
    if (touch) { touch = false; target->input.touch(); }
    SO_ENGINE_OUTPUT(first, SoSFInt32, setValue(117));
    SO_ENGINE_OUTPUT(second, SoSFInt32, setValue(123));
  }
};
class MixedParent : public SoGroup {
public:
  ProbeEngine * engine;
  SoSFInt32 * victim;
  SoSFInt32 * reader;
  SoSFInt32 added;
  const char * operation;
  bool armed;
  int calls, seen;
  MixedParent() : engine(NULL), victim(NULL), reader(NULL), operation(NULL),
    armed(false), calls(0), seen(-1) { added.setValue(-1); }
  void notify(SoNotList * list) override {
    if (armed) {
      armed = false; ++calls;
      if (std::strcmp(operation, "add")) victim->disconnect();
      if (std::strcmp(operation, "remove")) CHECK(added.connectFrom(&engine->first));
      seen = reader->getValue();
    }
    SoGroup::notify(list);
  }
protected: ~MixedParent() override {}
};
int main(int argc, char ** argv) {
  CHECK(argc == 4); initProbes();
  MixedNode * node = new MixedNode; node->ref();
  MixedEngine * engine = new MixedEngine; engine->ref();
  ProbeNode * outside = new ProbeNode; outside->ref();
  ProbeNode * target = !std::strcmp(argv[3], "external") ? outside : node;
  MixedParent * parent = new MixedParent; parent->ref(); parent->addChild(target);
  parent->operation = argv[1]; parent->engine = engine;
  SoSFInt32 nf, ns, ef, es, victim;
  CHECK(nf.connectFrom(&node->first)); CHECK(ns.connectFrom(&node->second));
  CHECK(ef.connectFrom(&engine->first)); CHECK(es.connectFrom(&engine->second));
  CHECK(victim.connectFrom(&engine->first));
  CHECK(nf.getValue() == 17); CHECK(ns.getValue() == 23);
  CHECK(ef.getValue() == 117); CHECK(es.getValue() == 123); CHECK(victim.getValue() == 117);
  victim.enableNotify(FALSE);
  node->peer = engine; node->target = target; engine->peer = node; engine->target = target;
  engine->input.touch();
  parent->reader = &es; parent->victim = &victim; parent->armed = true;
  if (!std::strcmp(argv[2], "node")) {
    node->run = true; engine->touch = true;
    node->first.touchSlaves(NULL, FALSE); CHECK(nf.getValue() == 17);
  }
  else {
    engine->run = true; node->touch = true;
    CHECK(ef.getValue() == 117);
  }
  CHECK(parent->calls == 1); CHECK(parent->seen == 123);
  CHECK(nf.isNotifyEnabled()); CHECK(ns.isNotifyEnabled());
  CHECK(ef.isNotifyEnabled()); CHECK(es.isNotifyEnabled()); CHECK(!victim.isNotifyEnabled());
  if (parent->added.isConnected()) { CHECK(parent->added.getValue() == 117); CHECK(parent->added.isNotifyEnabled()); }
  engine->input.touch(); CHECK(ef.getValue() == 117); CHECK(es.getValue() == 123);
  CHECK(ef.isNotifyEnabled()); CHECK(!victim.isNotifyEnabled());
  parent->added.disconnect(); nf.disconnect(); ns.disconnect(); ef.disconnect(); es.disconnect(); victim.disconnect();
  parent->unref(); node->unref(); engine->unref(); outside->unref();
}
