#include "Fixture.h"
#include <Inventor/lists/SoEngineOutputList.h>
#include <cstring>
class DynamicNode : public ProbeNode {
public:
  bool missing; SoEngineOutputData empty;
  DynamicNode() : missing(true) {}
  const SoEngineOutputData * getOutputData() const override { return missing ? NULL : &empty; }
};
class DynamicEngine : public ProbeEngine {
public:
  const SoEngineOutputData * getOutputData() const override { return NULL; }
};
int main(int argc, char ** argv) {
  CHECK(argc == 2); initProbes();
  if (!std::strcmp(argv[1], "engine-notify")) {
    DynamicEngine * e = new DynamicEngine; e->ref(); directNotify(e);
    CHECK(!e->isNotifying()); CHECK(e->state.changes == 1);
    e->evaluateWrapper(); CHECK(e->state.evaluations == 0); e->unref(); return 0;
  }
  DynamicNode * n = new DynamicNode; n->ref();
  if (!std::strcmp(argv[1], "notify")) { directNotify(n); CHECK(!n->isNotifying()); CHECK(n->state.changes == 1); }
  else if (!std::strcmp(argv[1], "evaluate")) { n->evaluateWrapper(); CHECK(n->state.evaluations == 0); }
  else if (!std::strcmp(argv[1], "lookup")) { SoEngineOutputList l; CHECK(n->getOutputs(l) == 0); CHECK(n->getOutput("first") == NULL); }
  else if (!std::strcmp(argv[1], "empty")) { n->missing = false; n->evaluateWrapper(); CHECK(n->state.evaluations == 1); directNotify(n); CHECK(n->state.changes == 1); }
  else return 2;
  // Keep metadata available for baseline teardown, which is tested separately.
  n->missing = false; n->unref();
}
