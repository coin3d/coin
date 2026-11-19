#include "Allocation.h"
#include "Fixture.h"
#include <cstring>
static SoSFInt32 * firstSlave = NULL;
static bool earlierPrepared = false;
static void observeFailure() { earlierPrepared = firstSlave && !firstSlave->isNotifyEnabled(); }
template<class T> int exercise(long ordinal) {
  T * e = new T; e->ref();
  SoSFInt32 slaves[128];
  for (int i = 0; i < 128; ++i) {
    slaves[i].enableNotify((i % 3) != 0);
    (i < 64 ? e->first : e->second).addConnection(&slaves[i]);
  }
  directNotify(e); const int before = e->state.evaluations;
  firstSlave = &slaves[1]; allocationFailureHook = observeFailure;
  allocationOrdinal = ordinal; bool caught = false;
  try { e->evaluateWrapper(); } catch (const std::bad_alloc &) { caught = true; }
  allocationOrdinal = -1; allocationFailureHook = NULL;
  if (caught) {
    CHECK(allocationFailed); CHECK(e->state.evaluations == before);
    for (int i = 0; i < 128; ++i) CHECK(slaves[i].isNotifyEnabled() == ((i % 3) != 0));
    e->evaluateWrapper(); CHECK(e->state.evaluations == before + 1);
    std::printf("preparation failure; earlier output prepared: %s\n", earlierPrepared ? "yes" : "no");
  }
  for (int i = 0; i < 128; ++i) {
    CHECK(slaves[i].isNotifyEnabled() == ((i % 3) != 0));
    (i < 64 ? e->first : e->second).removeConnection(&slaves[i]);
  }
  std::printf("injected=%d\n", caught ? 1 : 0);
  e->unref(); return 0;
}
int main(int argc, char ** argv) {
  CHECK(argc == 3); initProbes(); long ordinal = std::strtol(argv[2], NULL, 10);
  return !std::strcmp(argv[1], "node") ? exercise<ProbeNode>(ordinal) : exercise<ProbeEngine>(ordinal);
}
