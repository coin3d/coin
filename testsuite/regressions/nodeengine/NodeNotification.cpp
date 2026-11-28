#include "Fixture.h"
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/VRMLnodes/SoVRMLTimeSensor.h>
#include <cstring>
struct Seen {
  int calls; SoNode * node; SoField * field; ProbeNode * engine; bool inputWasCalled;
  Seen() : calls(0), node(NULL), field(NULL), engine(NULL), inputWasCalled(true) {}
};
static void observe(void * data, SoSensor * sensor) {
  Seen * s = static_cast<Seen *>(data); SoNodeSensor * n = static_cast<SoNodeSensor *>(sensor);
  ++s->calls; s->node = n->getTriggerNode(); s->field = n->getTriggerField();
  if (s->engine) s->inputWasCalled = s->engine->state.changes > 0;
}
static void attach(SoNodeSensor & s, SoNode * n) { s.setPriority(0); s.attach(n); }
class MissingNode : public ProbeNode {
public: const SoEngineOutputData * getOutputData() const override { return NULL; }
};
static void graph(const char * mode) {
  const bool missing = !std::strcmp(mode, "missing");
  ProbeNode * e = missing ? static_cast<ProbeNode *>(new MissingNode) : new ProbeNode; e->ref();
  SoGroup * root = new SoGroup; root->ref(); SoGroup * left = new SoGroup; SoGroup * right = new SoGroup;
  root->addChild(left); root->addChild(right); left->addChild(e); right->addChild(e);
  Seen direct, a, b, top; direct.engine = e; a.engine = e; b.engine = e; top.engine = e;
  SoNodeSensor ds(observe, &direct), as(observe, &a), bs(observe, &b), ts(observe, &top);
  attach(ds, e); attach(as, left); attach(bs, right); attach(ts, root);
  const SbUniqueId ei = e->getNodeId(), li = left->getNodeId(), ri = right->getNodeId(), ti = root->getNodeId();
  const bool touch = !std::strcmp(mode, "touch");
  if (touch) e->touch(); else e->input.setValue(11);
  CHECK(e->getNodeId() != ei); CHECK(left->getNodeId() != li); CHECK(right->getNodeId() != ri); CHECK(root->getNodeId() != ti);
  CHECK(direct.calls == 1); CHECK(a.calls == 1); CHECK(b.calls == 1); CHECK(top.calls == 1);
  CHECK(direct.node == e); CHECK(a.node == e); CHECK(b.node == e); CHECK(top.node == e);
  CHECK(direct.field == (touch ? NULL : &e->input)); CHECK(a.field == direct.field); CHECK(top.field == direct.field);
  CHECK(direct.inputWasCalled && a.inputWasCalled && b.inputWasCalled && top.inputWasCalled);
  CHECK(e->state.changes == 1); CHECK(e->state.lastInput == (touch ? NULL : &e->input));
  ds.detach(); as.detach(); bs.detach(); ts.detach(); root->unref(); e->unref();
}
static void disabled() {
  ProbeNode * e = new ProbeNode; e->ref(); SoGroup * root = new SoGroup; root->ref(); root->addChild(e);
  SoSFInt32 value; CHECK(value.connectFrom(&e->first, TRUE)); (void)value.getValue();
  const int evaluations = e->state.evaluations; Seen direct, ancestor;
  SoNodeSensor ds(observe, &direct), rs(observe, &ancestor); attach(ds, e); attach(rs, root);
  const SbUniqueId ei = e->getNodeId(), ri = root->getNodeId(); e->enableNotify(FALSE); e->input.setValue(42);
  CHECK(e->getNodeId() != ei); CHECK(root->getNodeId() == ri); CHECK(direct.calls == 0); CHECK(ancestor.calls == 0);
  CHECK(e->state.changes == 1); (void)value.getValue(); CHECK(e->state.evaluations == evaluations + 1);
  e->enableNotify(TRUE); e->input.setValue(43); CHECK(direct.calls == 1); CHECK(ancestor.calls == 1);
  value.disconnect(); ds.detach(); rs.detach(); root->unref(); e->unref();
}
static void feedback(bool self) {
  ProbeNode * e = new ProbeNode; e->ref(); SoSwitch * root = new SoSwitch; root->ref(); root->addChild(e);
  // Child notification is intentionally filtered by an inactive SoSwitch.
  // Select the engine before arming observers of this active scene graph.
  root->whichChild.setValue(0);
  if (self) CHECK(e->input.connectFrom(&e->first, TRUE));
  else CHECK(root->whichChild.connectFrom(&e->first, TRUE));
  Seen direct, ancestor; SoNodeSensor ds(observe, &direct), rs(observe, &ancestor); attach(ds, e); attach(rs, root);
  e->first.enable(FALSE); e->state.enableOutput = true; e->state.recurseInput = true;
  e->input.touch();
  CHECK(e->state.changes == 1); CHECK(!e->isNotifying()); CHECK(direct.calls == 1); CHECK(ancestor.calls == 1);
  e->state.recurseInput = false; e->state.enableOutput = false;
  if (self) e->input.disconnect(); else root->whichChild.disconnect();
  ds.detach(); rs.detach(); root->unref(); e->unref();
}
static void nodeType() {
  ProbeNode * e = new ProbeNode; e->ref(); const SoNode::NodeType before = e->getNodeType();
  e->input.setValue(5); CHECK(e->getNodeType() == before); e->unref();
  SoVRMLTimeSensor * vrml = new SoVRMLTimeSensor; vrml->ref();
  const SoNode::NodeType native = vrml->getNodeType(); vrml->enabled.setValue(FALSE);
  CHECK(vrml->getNodeType() == native); vrml->unref();
}
int main(int argc, char ** argv) {
  CHECK(argc == 2); initProbes();
  if (!std::strcmp(argv[1], "disabled")) disabled();
  else if (!std::strcmp(argv[1], "self")) feedback(true);
  else if (!std::strcmp(argv[1], "feedback")) feedback(false);
  else if (!std::strcmp(argv[1], "type")) nodeType();
  else graph(argv[1]);
}
