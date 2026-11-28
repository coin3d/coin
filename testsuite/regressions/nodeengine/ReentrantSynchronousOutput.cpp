#include "Fixture.h"
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <cstring>

class ChangingNode : public ProbeNode {
public:
  bool changeInput, recurseEvaluation, failEvaluation;
  SoSFInt32 * preparedFirst;
  SoSFInt32 * preparedSecond;
  ChangingNode() : changeInput(false), recurseEvaluation(false),
    failEvaluation(false), preparedFirst(NULL), preparedSecond(NULL) {}
protected:
  void evaluate() override {
    ++state.evaluations;
    if (preparedFirst) {
      CHECK(!preparedFirst->isNotifyEnabled());
      CHECK(!preparedSecond->isNotifyEnabled());
    }
    if (recurseEvaluation) {
      recurseEvaluation = false;
      evaluateWrapper();
    }
    if (failEvaluation) {
      failEvaluation = false;
      throw std::runtime_error("nested evaluation");
    }
    if (changeInput) {
      changeInput = false;
      input.setValue(input.getValue() + 1);
    }
    const int value = input.getValue();
    SO_ENGINE_OUTPUT(first, SoSFInt32, setValue(value));
    SO_ENGINE_OUTPUT(second, SoSFInt32, setValue(value + 100));
  }
};

class SynchronousParent : public SoGroup {
public:
  SoSFInt32 * field;
  bool armed;
  int calls, seen;
  SynchronousParent() : field(NULL), armed(false), calls(0), seen(-1) {}
  void notify(SoNotList * list) override {
    if (armed) {
      ++calls;
      seen = field->getValue();
    }
    SoGroup::notify(list);
  }
protected:
  ~SynchronousParent() override {}
};

static void countNotification(void * data, SoSensor *) {
  ++*static_cast<int *>(data);
}

static void nestedException() {
  ChangingNode * engine = new ChangingNode; engine->ref();
  SoSFInt32 first, second;
  CHECK(first.connectFrom(&engine->first, TRUE));
  CHECK(second.connectFrom(&engine->second, TRUE));
  second.enableNotify(FALSE);
  engine->preparedFirst = &first; engine->preparedSecond = &second;
  engine->recurseEvaluation = true; engine->failEvaluation = true;
  bool threw = false;
  // Isolate wrapper recovery from SoField's separate exception protocol.
  try { engine->evaluateWrapper(); }
  catch (const std::runtime_error &) { threw = true; }
  CHECK(threw); CHECK(engine->state.evaluations == 2);
  CHECK(first.isNotifyEnabled()); CHECK(!second.isNotifyEnabled());
  engine->evaluateWrapper();
  CHECK(engine->state.evaluations == 3);
  CHECK(first.isNotifyEnabled()); CHECK(!second.isNotifyEnabled());
  CHECK(first.getValue() == 0); CHECK(second.getValue() == 100);
  engine->preparedFirst = NULL; engine->preparedSecond = NULL;
  first.disconnect(); second.disconnect(); engine->unref();
}

int main(int argc, char ** argv) {
  CHECK(argc == 2);
  const bool mixed = !std::strcmp(argv[1], "mixed");
  initProbes();
  if (!std::strcmp(argv[1], "throw")) { nestedException(); return 0; }
  ChangingNode * engine = new ChangingNode; engine->ref();
  SynchronousParent * parent = new SynchronousParent; parent->ref();
  parent->addChild(engine);
  SoSFInt32 first, second;
  first.setValue(-1); second.setValue(-1);
  CHECK(first.connectFrom(&engine->first));
  CHECK(second.connectFrom(&engine->second));
  CHECK(first.getValue() == 0); CHECK(second.getValue() == 100);
  second.enableNotify(!mixed);

  int notifications = 0;
  SoFieldSensor sensor(countNotification, &notifications);
  sensor.setPriority(0);
  engine->input.setValue(1);
  parent->field = &second; parent->armed = true;
  engine->changeInput = true;
  const int before = engine->state.evaluations;

  // Reading the first output starts evaluate(). Its input change notifies
  // the parent, which reads the second output before evaluate() returns.
  CHECK(first.getValue() == 2);
  CHECK(engine->state.evaluations == before + 2);
  CHECK(parent->calls == 1); CHECK(parent->seen == 102);
  CHECK(second.getValue() == 102);
  CHECK(first.isNotifyEnabled()); CHECK(second.isNotifyEnabled() == !mixed);

  // Check that both values and future notifications still propagate.
  sensor.attach(&first);
  const int previousNotifications = notifications;
  engine->input.setValue(3);
  CHECK(parent->calls == 2); CHECK(parent->seen == 103);
  CHECK(first.getValue() == 3); CHECK(second.getValue() == 103);
  CHECK(notifications == previousNotifications + 1);
  CHECK(first.isNotifyEnabled()); CHECK(second.isNotifyEnabled() == !mixed);

  parent->armed = false; sensor.detach();
  first.disconnect(); second.disconnect();
  parent->unref(); engine->unref();
}
