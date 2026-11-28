#include "Allocation.h"
#include "Fixture.h"
#include <Inventor/nodes/SoGroup.h>

class AllocatingParent : public SoGroup {
public:
  ProbeNode * engine;
  SoSFInt32 * reader;
  SoSFInt32 added[128];
  bool armed;
  long ordinal;
  AllocatingParent() : engine(NULL), reader(NULL), armed(false), ordinal(0) {}
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
      allocationOrdinal = ordinal;
    }
    SoGroup::notify(list);
  }
protected:
  ~AllocatingParent() override {}
};

int main(int argc, char ** argv) {
  CHECK(argc == 2); initProbes();
  ProbeNode * engine = new ProbeNode; engine->ref();
  AllocatingParent * parent = new AllocatingParent; parent->ref();
  parent->engine = engine; parent->ordinal = std::strtol(argv[1], NULL, 10);
  parent->addChild(engine);
  SoSFInt32 first, second;
  CHECK(first.connectFrom(&engine->first)); CHECK(second.connectFrom(&engine->second));
  CHECK(first.getValue() == 17); CHECK(second.getValue() == 23);
  second.enableNotify(FALSE); parent->reader = &second; parent->armed = true;
  engine->state.dirtyDuringEvaluate = true;
  bool caught = false;
  try { engine->evaluateWrapper(); }
  catch (const std::bad_alloc &) { caught = true; }
  allocationOrdinal = -1;
  CHECK(caught); CHECK(allocationFailed);
  CHECK(first.isNotifyEnabled()); CHECK(!second.isNotifyEnabled());
  for (int i = 0; i < 128; ++i) CHECK(parent->added[i].isNotifyEnabled() == ((i % 3) != 0));
  engine->evaluateWrapper();
  CHECK(first.isNotifyEnabled()); CHECK(!second.isNotifyEnabled());
  for (int i = 0; i < 128; ++i) {
    CHECK(parent->added[i].getValue() == (i < 64 ? 17 : 23));
    CHECK(parent->added[i].isNotifyEnabled() == ((i % 3) != 0));
    parent->added[i].disconnect();
  }
  first.disconnect(); second.disconnect(); parent->unref(); engine->unref();
}
