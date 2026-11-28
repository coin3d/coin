#include "Fixture.h"
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <cstring>

class ConnectionNode : public ProbeNode {
public:
  bool changeInput, catchNotification, caught;
  ConnectionNode() : changeInput(false), catchNotification(false), caught(false) {}
protected:
  void evaluate() override {
    ++state.evaluations;
    if (changeInput) {
      changeInput = false;
      try { input.setValue(input.getValue() + 1); }
      catch (const std::runtime_error &) {
        if (!catchNotification) throw;
        caught = true;
      }
      // Even if evaluate() catches a callback exception, all current
      // connections must be prepared again before it resumes writing.
      for (int i = 0; i < first.getNumConnections(); ++i) CHECK(!first[i]->isNotifyEnabled());
      for (int i = 0; i < second.getNumConnections(); ++i) CHECK(!second[i]->isNotifyEnabled());
    }
    const int value = input.getValue();
    SO_ENGINE_OUTPUT(first, SoSFInt32, setValue(value));
    SO_ENGINE_OUTPUT(second, SoSFInt32, setValue(value + 100));
  }
};

class ConnectionParent : public SoGroup {
public:
  ConnectionNode * engine;
  SoSFInt32 * reader;
  SoSFInt32 * removed;
  SoSFInt32 added;
  bool armed, changed, after, mixed, fail;
  const char * operation;
  int calls, seen;
  ConnectionParent() : engine(NULL), reader(NULL), removed(NULL),
    armed(false), changed(false), after(false), mixed(false), fail(false),
    operation(NULL), calls(0), seen(-1) { added.setValue(-1); }
  void changeConnections() {
    if (changed) return;
    changed = true;
    if (std::strcmp(operation, "add")) removed->disconnect(&engine->first);
    if (std::strcmp(operation, "remove")) {
      added.enableNotify(!mixed);
      CHECK(added.connectFrom(&engine->first));
    }
  }
  void notify(SoNotList * list) override {
    if (armed) {
      ++calls;
      if (!after) changeConnections();
      seen = reader->getValue();
      if (after) changeConnections();
      if (added.isConnected()) CHECK(added.getValue() == engine->input.getValue());
      if (fail) { fail = false; throw std::runtime_error("parent callback"); }
    }
    SoGroup::notify(list);
  }
protected:
  ~ConnectionParent() override {}
};

static void countNotification(void * data, SoSensor *) {
  ++*static_cast<int *>(data);
}

int main(int argc, char ** argv) {
  CHECK(argc == 4);
  initProbes();
  ConnectionNode * engine = new ConnectionNode; engine->ref();
  ConnectionParent * parent = new ConnectionParent; parent->ref();
  parent->engine = engine; parent->operation = argv[1];
  parent->after = !std::strcmp(argv[2], "after");
  parent->mixed = !std::strcmp(argv[3], "mixed");
  const bool throwing = !std::strcmp(argv[3], "throw");
  parent->fail = throwing; engine->catchNotification = throwing;
  parent->addChild(engine);
  SoSFInt32 first, second, removed;
  first.setValue(-1); second.setValue(-1); removed.setValue(-1);
  CHECK(first.connectFrom(&engine->first));
  CHECK(second.connectFrom(&engine->second));
  CHECK(removed.connectFrom(&engine->first));
  CHECK(first.getValue() == 0); CHECK(second.getValue() == 100);
  CHECK(removed.getValue() == 0);
  second.enableNotify(!parent->mixed); removed.enableNotify(!parent->mixed);
  engine->input.setValue(1);
  CHECK(first.getValue() == 1);
  int notifications = 0;
  SoFieldSensor sensor(countNotification, &notifications);
  sensor.setPriority(0); sensor.attach(&first);
  // Arm an immediate field observer without evaluating away the pending
  // read. It must not be triggered while the outer value is incomplete.
  engine->first.touchSlaves(NULL, FALSE);
  engine->second.touchSlaves(NULL, FALSE);
  parent->reader = &second; parent->removed = &removed; parent->armed = true;
  engine->changeInput = true;

  CHECK(first.getValue() == 2);
  CHECK(notifications == 0);
  CHECK(parent->changed); CHECK(parent->calls == 1); CHECK(parent->seen == 102);
  CHECK(first.isNotifyEnabled());
  CHECK(second.isNotifyEnabled() == !parent->mixed);
  CHECK(removed.isNotifyEnabled() == !parent->mixed);
  CHECK(removed.isConnected() == !std::strcmp(argv[1], "add"));
  if (parent->added.isConnected()) CHECK(parent->added.isNotifyEnabled() == !parent->mixed);

  if (throwing) {
    CHECK(engine->caught);
    // SoField/SoDB exception recovery is separate: isolate a wrapper retry
    // after the engine caught the callback failure, without another input event.
    engine->evaluateWrapper();
    CHECK(first.getValue() == 2); CHECK(second.getValue() == 102);
  }
  else {
    // Repeating the evaluation and delivering a later notification must use
    // the updated connection set, while disconnected slaves retain their value.
    engine->input.setValue(3);
    CHECK(parent->calls == 2); CHECK(parent->seen == 103);
    CHECK(first.getValue() == 3); CHECK(second.getValue() == 103);
    CHECK(removed.getValue() == (removed.isConnected() ? 3 : 2));
    CHECK(notifications == 1);
    CHECK(first.isNotifyEnabled());
    CHECK(second.isNotifyEnabled() == !parent->mixed);
    CHECK(removed.isNotifyEnabled() == !parent->mixed);
    if (parent->added.isConnected()) {
      CHECK(parent->added.getValue() == 3);
      CHECK(parent->added.isNotifyEnabled() == !parent->mixed);
    }
  }

  parent->armed = false; sensor.detach();
  parent->added.disconnect(); first.disconnect(); second.disconnect(); removed.disconnect();
  parent->unref(); engine->unref();
}
