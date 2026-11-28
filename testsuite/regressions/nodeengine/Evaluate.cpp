#include "Fixture.h"
#include <cstring>
template<class T> void exercise(const char * mode, bool ordinary) {
  T * e = new T; e->ref();
  SoSFInt32 a, b, shared;
  CHECK(a.connectFrom(&e->first, TRUE)); CHECK(b.connectFrom(&e->second, TRUE));
  b.enableNotify(FALSE);
  bool sharedCase = !std::strcmp(mode, "shared") || !std::strcmp(mode, "shared-throw");
  if (sharedCase) { CHECK(shared.connectFrom(&e->first, TRUE)); CHECK(shared.appendConnection(&e->second, TRUE)); }
  directNotify(e);
  const int before = e->state.evaluations;
  bool throwing = !std::strcmp(mode, "throw") || !std::strcmp(mode, "shared-throw");
  e->state.throwEvaluate = throwing;
  bool threw = false;
  try { e->evaluateWrapper(); } catch (const std::runtime_error &) { threw = true; }
  CHECK(threw == throwing); CHECK(a.isNotifyEnabled()); CHECK(!b.isNotifyEnabled());
  if (sharedCase) CHECK(shared.isNotifyEnabled());
  CHECK(e->state.evaluations == before + 1);
  e->state.throwEvaluate = false;
  if (throwing) { e->evaluateWrapper(); CHECK(e->state.evaluations == before + 2); }
  if (!std::strcmp(mode, "dirty")) {
    directNotify(e); e->state.dirtyDuringEvaluate = true;
    e->evaluateWrapper(); const int evaluated = e->state.evaluations;
    e->evaluateWrapper(); CHECK(e->state.evaluations == evaluated + 1);
    if (ordinary) { e->evaluateWrapper(); CHECK(e->state.evaluations == evaluated + 1); }
  }
  CHECK(a.isNotifyEnabled()); CHECK(!b.isNotifyEnabled());
  if (sharedCase) { CHECK(shared.isNotifyEnabled()); shared.disconnect(); }
  a.disconnect(); b.disconnect(); e->unref();
}
int main(int argc, char ** argv) {
  CHECK(argc == 3); initProbes();
  if (!std::strcmp(argv[1], "node")) exercise<ProbeNode>(argv[2], false);
  else exercise<ProbeEngine>(argv[2], true);
}
