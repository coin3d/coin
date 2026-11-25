#ifndef COIN_NODEENGINE_REGRESSION_FIXTURE_H
#define COIN_NODEENGINE_REGRESSION_FIXTURE_H
#include <Inventor/SoDB.h>
#include <Inventor/engines/SoNodeEngine.h>
#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/misc/SoNotification.h>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#define CHECK(x) do { if (!(x)) { std::fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #x); std::abort(); } } while (0)
struct Behavior {
  int evaluations, changes; int * externalEvaluations;
  bool throwEvaluate, throwInput, recurseInput, enableOutput, dirtyDuringEvaluate;
  bool sawNotifying; SoField * lastInput;
  Behavior() : evaluations(0), changes(0), externalEvaluations(NULL),
    throwEvaluate(false), throwInput(false), recurseInput(false), enableOutput(false),
    dirtyDuringEvaluate(false), sawNotifying(false), lastInput(NULL) {}
};
class ProbeNode : public SoNodeEngine {
  SO_NODE_HEADER(ProbeNode);
public:
  SoSFInt32 input; SoEngineOutput first, second; Behavior state;
  SoEngineOutputData outputs;
  ProbeNode() {
    SO_NODE_CONSTRUCTOR(ProbeNode); SO_NODE_ADD_FIELD(input, (0));
    outputs.addOutput(this, "first", &first, SoSFInt32::getClassTypeId());
    outputs.addOutput(this, "second", &second, SoSFInt32::getClassTypeId());
    first.setNodeContainer(this); second.setNodeContainer(this);
  }
  static void initClass() { SO_NODE_INIT_CLASS(ProbeNode, SoNodeEngine, "NodeEngine"); }
  const SoEngineOutputData * getOutputData() const override { return &outputs; }
protected:
  ~ProbeNode() override {}
  void inputChanged(SoField * f) override {
    ++state.changes; state.lastInput = f; state.sawNotifying = isNotifying();
    if (state.enableOutput) first.enable(TRUE);
    if (state.recurseInput) { SoNotList l; SoNotRec r(this); r.setType(SoNotRec::CONTAINER); l.append(&r, &input); notify(&l); }
    if (state.throwInput) throw std::runtime_error("input");
  }
  void evaluate() override {
    ++state.evaluations; if (state.externalEvaluations) ++*state.externalEvaluations;
    if (state.throwEvaluate) throw std::runtime_error("evaluate");
    if (state.dirtyDuringEvaluate) { state.dirtyDuringEvaluate = false; input.touch(); }
    SO_ENGINE_OUTPUT(first, SoSFInt32, setValue(17));
    SO_ENGINE_OUTPUT(second, SoSFInt32, setValue(23));
  }
};
SO_NODE_SOURCE(ProbeNode);
class ProbeEngine : public SoEngine {
  SO_ENGINE_HEADER(ProbeEngine);
public:
  SoSFInt32 input; SoEngineOutput first, second; Behavior state;
  ProbeEngine() {
    SO_ENGINE_CONSTRUCTOR(ProbeEngine); SO_ENGINE_ADD_INPUT(input, (0));
    SO_ENGINE_ADD_OUTPUT(first, SoSFInt32); SO_ENGINE_ADD_OUTPUT(second, SoSFInt32);
  }
  static void initClass() { SO_ENGINE_INIT_CLASS(ProbeEngine, SoEngine, "Engine"); }
protected:
  ~ProbeEngine() override {}
  void inputChanged(SoField * f) override {
    ++state.changes; state.lastInput = f; state.sawNotifying = isNotifying();
    if (state.enableOutput) first.enable(TRUE);
    if (state.recurseInput) { SoNotList l; SoNotRec r(this); r.setType(SoNotRec::CONTAINER); l.append(&r, &input); notify(&l); }
    if (state.throwInput) throw std::runtime_error("input");
  }
  void evaluate() override {
    ++state.evaluations; if (state.externalEvaluations) ++*state.externalEvaluations;
    if (state.throwEvaluate) throw std::runtime_error("evaluate");
    if (state.dirtyDuringEvaluate) { state.dirtyDuringEvaluate = false; input.touch(); }
    SO_ENGINE_OUTPUT(first, SoSFInt32, setValue(17));
    SO_ENGINE_OUTPUT(second, SoSFInt32, setValue(23));
  }
};
SO_ENGINE_SOURCE(ProbeEngine);
template<class T> void directNotify(T * engine) {
  SoNotList l; SoNotRec r(engine); r.setType(SoNotRec::CONTAINER);
  l.append(&r, &engine->input); engine->notify(&l);
}
static void initProbes() { SoDB::init(); ProbeNode::initClass(); ProbeEngine::initClass(); }
#endif
