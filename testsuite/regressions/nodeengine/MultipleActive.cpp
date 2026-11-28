#include "Fixture.h"
#include <Inventor/nodes/SoGroup.h>
#include <cstring>

class ChainedNode : public ProbeNode {
public:
  ChainedNode * next;
  ProbeNode * target;
  bool run, touch;
  int value;
  ChainedNode() : next(NULL), target(NULL), run(false), touch(false), value(17) {}
protected:
  void evaluate() override {
    ++state.evaluations;
    if (run) { run = false; next->evaluateWrapper(); }
    if (touch) { touch = false; target->input.touch(); }
    SO_ENGINE_OUTPUT(first, SoSFInt32, setValue(value));
    SO_ENGINE_OUTPUT(second, SoSFInt32, setValue(value + 6));
  }
};

class ChangingParent : public SoGroup {
public:
  ChainedNode * engines[2];
  SoSFInt32 * victims[2];
  SoSFInt32 added[2];
  SoSFInt32 * reader;
  ChainedNode * helper;
  const char * operation;
  bool armed;
  int calls, seen;
  ChangingParent() : reader(NULL), helper(NULL), operation(NULL), armed(false), calls(0), seen(-1) {}
  void notify(SoNotList * list) override {
    if (armed) {
      armed = false; ++calls;
      if (helper) helper->evaluateWrapper();
      // Change two scopes, including one other than the notified engine.
      for (int i = 0; i < 2; ++i) {
        if (std::strcmp(operation, "add")) victims[i]->disconnect(&engines[i]->first);
        if (std::strcmp(operation, "remove")) {
          added[i].setValue(-1); added[i].enableNotify(i != 0);
          CHECK(added[i].connectFrom(&engines[i]->first, TRUE));
        }
      }
      seen = reader->getValue();
    }
    SoGroup::notify(list);
  }
protected:
  ~ChangingParent() override {}
};

int main(int argc, char ** argv) {
  CHECK(argc == 5); initProbes();
  const int depth = std::atoi(argv[2]); CHECK(depth == 2 || depth == 3);
  const bool external = !std::strcmp(argv[3], "external");
  const bool nested = !std::strcmp(argv[3], "nested");
  const bool share = !std::strcmp(argv[4], "shared");
  ChainedNode * engines[3]; SoSFInt32 first[3], second[3];
  for (int i = 0; i < depth; ++i) {
    engines[i] = new ChainedNode; engines[i]->ref(); engines[i]->value += i * 100;
    CHECK(first[i].connectFrom(&engines[i]->first));
    CHECK(second[i].connectFrom(&engines[i]->second));
    CHECK(first[i].getValue() == engines[i]->value);
    CHECK(second[i].getValue() == engines[i]->value + 6);
  }
  ChainedNode * outside = new ChainedNode; outside->ref(); outside->value = 417;
  SoSFInt32 outsideValue; CHECK(outsideValue.connectFrom(&outside->second));
  CHECK(outsideValue.getValue() == 423);
  ProbeNode * target = external ? static_cast<ProbeNode *>(outside) : engines[0];
  ChangingParent * parent = new ChangingParent; parent->ref(); parent->addChild(target);
  parent->operation = argv[1]; parent->reader = external ? &outsideValue : &second[0];
  SoSFInt32 victims[2], shared;
  for (int i = 0; i < 2; ++i) {
    parent->engines[i] = engines[depth - 2 + i]; parent->victims[i] = &victims[i];
    CHECK(victims[i].connectFrom(&parent->engines[i]->first));
    (void)victims[i].getValue(); victims[i].enableNotify(i != 0);
  }
  if (share) {
    CHECK(shared.connectFrom(&parent->engines[0]->first));
    CHECK(shared.appendConnection(&parent->engines[1]->first));
    (void)shared.getValue();
  }
  ChangingParent * child = NULL;
  if (nested) {
    child = new ChangingParent; child->ref(); child->addChild(outside);
    child->engines[0] = outside; child->engines[1] = engines[depth-1];
    child->operation = "add"; child->reader = &outsideValue; child->armed = true;
    outside->target = outside; outside->touch = true; parent->helper = outside;
  }
  for (int i = 0; i + 1 < depth; ++i) { engines[i]->next = engines[i+1]; engines[i]->run = true; }
  engines[depth-1]->target = target; engines[depth-1]->touch = true;
  parent->armed = true;
  engines[0]->first.touchSlaves(NULL, FALSE);
  CHECK(first[0].getValue() == 17);
  CHECK(parent->calls == 1); CHECK(parent->seen == (external ? 423 : 23));
  if (child) {
    CHECK(child->calls == 1); CHECK(child->seen == 423);
    CHECK(!child->added[0].isNotifyEnabled()); CHECK(child->added[1].isNotifyEnabled());
    CHECK(child->added[0].getValue() == 417);
    CHECK(child->added[1].getValue() == engines[depth-1]->value);
  }
  for (int i = 0; i < depth; ++i) {
    CHECK(first[i].isNotifyEnabled()); CHECK(second[i].isNotifyEnabled());
    CHECK(first[i].getValue() == engines[i]->value);
  }
  for (int i = 0; i < 2; ++i) {
    CHECK(victims[i].isNotifyEnabled() == (i != 0));
    CHECK(victims[i].isConnected() == !std::strcmp(argv[1], "add"));
    if (parent->added[i].isConnected()) {
      CHECK(parent->added[i].getValue() == parent->engines[i]->value);
      CHECK(parent->added[i].isNotifyEnabled() == (i != 0));
    }
  }
  if (share) { CHECK(shared.isNotifyEnabled()); shared.disconnect(); }
  // Repeat ordinary evaluations after every suspended scope has unwound.
  for (int i = 0; i < depth; ++i) engines[i]->evaluateWrapper();
  for (int i = 0; i < 2; ++i) {
    CHECK(victims[i].isNotifyEnabled() == (i != 0));
    parent->added[i].disconnect(); victims[i].disconnect();
  }
  if (child) { child->added[0].disconnect(); child->added[1].disconnect(); child->unref(); }
  outsideValue.disconnect(); parent->unref(); outside->unref();
  for (int i = 0; i < depth; ++i) { first[i].disconnect(); second[i].disconnect(); engines[i]->unref(); }
}
