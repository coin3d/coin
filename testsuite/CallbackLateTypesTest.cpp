#include <Inventor/SoDB.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSubNode.h>
#include <cstdio>
#include <string>
#include <vector>

class LateCube : public SoCube {
  SO_NODE_HEADER(LateCube);
public:
  static void initClass() { SO_NODE_INIT_CLASS(LateCube, SoCube, "Cube"); }
  LateCube() { SO_NODE_CONSTRUCTOR(LateCube); }
protected: ~LateCube() override {}
};
SO_NODE_SOURCE(LateCube);
class LaterCube : public LateCube {
  SO_NODE_HEADER(LaterCube);
public:
  static void initClass() { SO_NODE_INIT_CLASS(LaterCube, LateCube, "LateCube"); }
  LaterCube() { SO_NODE_CONSTRUCTOR(LaterCube); }
protected: ~LaterCube() override {}
};
SO_NODE_SOURCE(LaterCube);

static std::vector<int> seen;
static void record(void * p) { seen.push_back(*static_cast<int *>(p)); }
static SoCallbackAction::Response nodecb(void * p, SoCallbackAction *, const SoNode *) {
  record(p); return SoCallbackAction::CONTINUE;
}
static void tri(void * p, SoCallbackAction *, const SoPrimitiveVertex *, const SoPrimitiveVertex *, const SoPrimitiveVertex *) { record(p); }
static void line(void * p, SoCallbackAction *, const SoPrimitiveVertex *, const SoPrimitiveVertex *) { record(p); }
static void point(void * p, SoCallbackAction *, const SoPrimitiveVertex *) { record(p); }
static void add(SoCallbackAction & a, SoType t, int family, int * tag) {
  switch (family) {
  case 0: a.addPreCallback(t, nodecb, tag); break;
  case 1: a.addPostCallback(t, nodecb, tag); break;
  case 2: a.addTriangleCallback(t, tri, tag); break;
  case 3: a.addLineSegmentCallback(t, line, tag); break;
  case 4: a.addPointCallback(t, point, tag); break;
  }
}
static void invoke(SoCallbackAction & a, SoCube * n, int f) {
  SoPrimitiveVertex v;
  switch (f) {
  case 0: a.invokePreCallbacks(n); break;
  case 1: a.invokePostCallbacks(n); break;
  case 2: a.invokeTriangleCallbacks(n, &v, &v, &v); break;
  case 3: a.invokeLineSegmentCallbacks(n, &v, &v); break;
  case 4: a.invokePointCallbacks(n, &v); break;
  }
}
// invokePreCallbacks/invokePostCallbacks require traversal-initialized state.
// Prime the action before registering any observers so the setup neither
// consumes the callback being tested nor initializes either late subtype.
static void initializeAction(SoCallbackAction & action) {
  SoCube * root=new SoCube; root->ref();
  action.apply(root);
  root->unref();
}
static int failures = 0;
static void expect(const std::vector<int> & wanted, const char * label, int f) {
  if (seen != wanted) {
    std::fprintf(stderr, "%s family %d: expected", label, f);
    for (int x : wanted) std::fprintf(stderr, " %d", x);
    std::fprintf(stderr, "; observed");
    for (int x : seen) std::fprintf(stderr, " %d", x);
    std::fprintf(stderr, "\n"); ++failures;
  }
  seen.clear();
}
static int first = 1, second = 2, third = 3, fourth = 4;
static int activefamily;
static bool added;
static void liveadd(SoCallbackAction * a) {
  record(&first);
  if (!added) { added = true; add(*a, SoCube::getClassTypeId(), activefamily, &second); }
}
static SoCallbackAction::Response liveNode(void *, SoCallbackAction * a, const SoNode *) { liveadd(a); return SoCallbackAction::CONTINUE; }
static void liveTri(void *, SoCallbackAction * a, const SoPrimitiveVertex *, const SoPrimitiveVertex *, const SoPrimitiveVertex *) { liveadd(a); }
static void liveLine(void *, SoCallbackAction * a, const SoPrimitiveVertex *, const SoPrimitiveVertex *) { liveadd(a); }
static void livePoint(void *, SoCallbackAction * a, const SoPrimitiveVertex *) { liveadd(a); }
static SoCallbackAction::Response initialize(void * data, SoCallbackAction *, const SoNode *) {
  SoCallbackAction * a=static_cast<SoCallbackAction *>(data);
  LateCube::initClass(); LaterCube::initClass();
  LaterCube * n = new LaterCube; n->ref();
  for (int f = 0; f < 5; ++f) { invoke(*a, n, f); expect({1}, "created during callback", f); }
  n->unref(); return SoCallbackAction::CONTINUE;
}
int main(int argc, char ** argv) {
  SoDB::init();
  std::string mode = argc > 1 ? argv[1] : "late";
  if (mode == "internal") {
    LateCube::initClass();
    SoType hidden=LateCube::getClassTypeId(); hidden.makeInternal();
    LateCube * n=new LateCube; n->ref();
    SoCube * control=new SoCube; control->ref();
    for (int f=0; f<5; ++f) {
      SoCallbackAction a; initializeAction(a);
      add(a,SoNode::getClassTypeId(),f,&first);
      add(a,SoCube::getClassTypeId(),f,&second);
      invoke(a,n,f); expect({},"internal type excluded before registration",f);
      invoke(a,control,f); expect({1,2},"public type registration control",f);
      if (f>=2 && a.shouldGeneratePrimitives(n)) {
        std::fprintf(stderr,"internal type unexpectedly requests primitives family %d\n",f); ++failures;
      }
      if (f>=2 && !a.shouldGeneratePrimitives(control)) {
        std::fprintf(stderr,"public type missing primitive request family %d\n",f); ++failures;
      }
      // A refresh prompted by another public type must preserve the exclusion.
      if (f==0) LaterCube::initClass();
      invoke(a,n,f); expect({},"internal type excluded after public initialization",f);
    }
    control->unref(); n->unref();
  } else if (mode == "live") {
    SoCube * n = new SoCube; n->ref();
    for (int f = 0; f < 5; ++f) {
      SoCallbackAction a; initializeAction(a); activefamily = f; added = false;
      switch (f) {
      case 0: a.addPreCallback(SoCube::getClassTypeId(), liveNode, nullptr); break;
      case 1: a.addPostCallback(SoCube::getClassTypeId(), liveNode, nullptr); break;
      case 2: a.addTriangleCallback(SoCube::getClassTypeId(), liveTri, nullptr); break;
      case 3: a.addLineSegmentCallback(SoCube::getClassTypeId(), liveLine, nullptr); break;
      case 4: a.addPointCallback(SoCube::getClassTypeId(), livePoint, nullptr); break;
      }
      invoke(a,n,f); expect({1,2}, "live registration", f);
      invoke(a,n,f); expect({1,2}, "stable live registration", f);
    }
    n->unref();
  } else if (mode == "during") {
    SoCallbackAction a, observer;
    initializeAction(a); initializeAction(observer);
    for (int f=0; f<5; ++f) add(observer, SoCube::getClassTypeId(), f, &first);
    a.addPreCallback(SoCube::getClassTypeId(), initialize, &observer);
    SoCube * n = new SoCube; n->ref(); a.apply(n); n->unref();
  } else {
    SoCallbackAction actions[5];
    for (int f=0; f<5; ++f) {
      initializeAction(actions[f]);
      add(actions[f], SoNode::getClassTypeId(), f, &first);
      add(actions[f], SoCube::getClassTypeId(), f, &second);
      add(actions[f], SoNode::getClassTypeId(), f, &first);
    }
    LateCube::initClass();
    LateCube * n = new LateCube; n->ref();
    for (int f=0; f<5; ++f) {
      if (f>=2 && !actions[f].shouldGeneratePrimitives(n)) { std::fprintf(stderr,"missing primitive demand %d\n",f); ++failures; }
      invoke(actions[f],n,f); expect({1,2,1},"late first generation",f);
      add(actions[f], LateCube::getClassTypeId(),f,&third);
      invoke(actions[f],n,f); expect({1,2,1,3},"later subscription",f);
    }
    LaterCube::initClass();
    LaterCube * nn = new LaterCube; nn->ref();
    for (int f=0; f<5; ++f) {
      invoke(actions[f],nn,f); expect({1,2,1,3},"late second generation",f);
      add(actions[f],SoCube::getClassTypeId(),f,&fourth);
      for (int i=0;i<3;++i) { invoke(actions[f],nn,f); expect({1,2,1,3,4},"no duplicated observers",f); }
      invoke(actions[f],n,f); expect({1,2,1,3,4},"old type after refresh",f);
    }
    nn->unref(); n->unref();
  }
  return failures ? 1 : 0;
}
