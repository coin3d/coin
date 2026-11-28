#include "Fixture.h"
#include <cstring>
class ThrowingSlave : public SoSFInt32 {
public:
  bool armed; int calls; bool engineRecord;
  ThrowingSlave() : armed(false), calls(0), engineRecord(false) {}
  void notify(SoNotList * l) override {
    ++calls;
    engineRecord = l->getLastRec() && l->getLastRec()->getType() == SoNotRec::ENGINE;
    if (armed) throw std::runtime_error("slave");
    // Isolate the engine's unwind protocol from generic field notification.
  }
};
template<class T> void exercise(const char * mode) {
  T * e = new T; e->ref();
  ThrowingSlave slave; CHECK(slave.connectFrom(&e->first, TRUE)); slave.calls = 0;
  bool input = !std::strcmp(mode, "input"); bool output = !std::strcmp(mode, "slave");
  e->state.throwInput = input; slave.armed = output;
  e->state.recurseInput = true; e->first.enable(FALSE); e->state.enableOutput = true;
  bool threw = false;
  try { directNotify(e); } catch (const std::runtime_error &) { threw = true; }
  CHECK(threw == (input || output)); CHECK(!e->isNotifying());
  CHECK(e->state.sawNotifying); CHECK(e->state.changes == 1);
  e->state.throwInput = false; slave.armed = false;
  directNotify(e);
  CHECK(!e->isNotifying()); CHECK(e->state.changes == 2);
  CHECK(e->state.lastInput == &e->input); CHECK(slave.engineRecord);
  CHECK(slave.calls == (input ? 1 : 2));
  e->state.recurseInput = false; e->state.enableOutput = false;
  slave.disconnect(); e->unref();
}
int main(int argc, char ** argv) {
  CHECK(argc == 3); initProbes();
  if (!std::strcmp(argv[1], "node")) exercise<ProbeNode>(argv[2]);
  else exercise<ProbeEngine>(argv[2]);
}
