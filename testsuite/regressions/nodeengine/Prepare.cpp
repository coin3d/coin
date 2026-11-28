#include "Allocation.h"
#include "Fixture.h"
int main(int argc, char ** argv) {
  CHECK(argc == 2); initProbes();
  ProbeEngine * e = new ProbeEngine; e->ref();
  SoSFInt32 slaves[64];
  for (int i = 0; i < 64; ++i) {
    slaves[i].enableNotify((i % 3) != 0); e->first.addConnection(&slaves[i]);
  }
  allocationOrdinal = std::strtol(argv[1], NULL, 10);
  bool caught = false;
  try { e->first.prepareToWrite(); } catch (const std::bad_alloc &) { caught = true; }
  allocationOrdinal = -1;
  if (caught) {
    CHECK(allocationFailed);
    for (int i = 0; i < 64; ++i) CHECK(slaves[i].isNotifyEnabled() == ((i % 3) != 0));
    e->first.prepareToWrite();
  }
  for (int i = 0; i < 64; ++i) CHECK(!slaves[i].isNotifyEnabled());
  e->first.doneWriting();
  for (int i = 0; i < 64; ++i) {
    CHECK(slaves[i].isNotifyEnabled() == ((i % 3) != 0)); e->first.removeConnection(&slaves[i]);
  }
  e->unref();
  std::printf("injected=%d\n", caught ? 1 : 0);
  return 0;
}
