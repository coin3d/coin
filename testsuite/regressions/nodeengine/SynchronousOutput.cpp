#include "Fixture.h"
#include <Inventor/nodes/SoGroup.h>

class InputEchoNode : public ProbeNode {
protected:
  void evaluate() override {
    ++state.evaluations;
    const int value = input.getValue();
    SO_ENGINE_OUTPUT(first, SoSFInt32, setValue(value));
  }
};

class ReadingParent : public SoGroup {
public:
  bool armed;
  int observations;
  int seen;
  SoSFInt32 * slave;
  ReadingParent() : armed(false), observations(0), seen(-1), slave(NULL) {}
  void notify(SoNotList * list) override {
    if (armed) {
      ++observations;
      // Observe while node notification is still on the stack. A delayed
      // SoNodeSensor callback would hide ordering between node and outputs.
      seen = slave->getValue();
    }
    SoGroup::notify(list);
  }
protected:
  ~ReadingParent() override {}
};

int main() {
  initProbes();
  InputEchoNode * engine = new InputEchoNode; engine->ref();
  ReadingParent * parent = new ReadingParent; parent->ref();
  parent->addChild(engine);
  SoSFInt32 slave;
  // A normal connection marks the new slave dirty; suppressing its
  // initial notification would leave it unevaluated.
  CHECK(slave.connectFrom(&engine->first));
  CHECK(slave.getValue() == 0);
  CHECK(engine->state.evaluations == 1);
  CHECK(engine->state.changes == 0);

  parent->slave = &slave;
  parent->armed = true;
  engine->input.setValue(41);
  parent->armed = false;
  CHECK(parent->observations == 1);
  CHECK(parent->seen == 41);
  CHECK(engine->state.evaluations == 2);
  CHECK(engine->state.changes == 1);
  CHECK(slave.getValue() == 41);
  CHECK(engine->state.evaluations == 2);

  slave.disconnect();
  parent->unref();
  engine->unref();
}
