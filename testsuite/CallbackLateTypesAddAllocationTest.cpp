#include <Inventor/SoDB.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSubNode.h>
#include <cstdio>
#include <cstdlib>
#include <new>

static bool armed = false;
static unsigned allocationCount = 0;
static unsigned failureOrdinal = 0;

static void * allocate(std::size_t bytes) {
  if (armed && ++allocationCount == failureOrdinal) throw std::bad_alloc();
  void * p = std::malloc(bytes ? bytes : 1);
  if (!p) throw std::bad_alloc();
  return p;
}

void * operator new(std::size_t bytes) { return allocate(bytes); }
void * operator new[](std::size_t bytes) { return allocate(bytes); }
void operator delete(void * p) noexcept { std::free(p); }
void operator delete[](void * p) noexcept { std::free(p); }
#if __cplusplus >= 201402L
void operator delete(void * p, std::size_t) noexcept { std::free(p); }
void operator delete[](void * p, std::size_t) noexcept { std::free(p); }
#endif

class AddAllocationLateCube : public SoCube {
  SO_NODE_HEADER(AddAllocationLateCube);
public:
  static void initClass() {
    SO_NODE_INIT_CLASS(AddAllocationLateCube, SoCube, "Cube");
  }
  AddAllocationLateCube() { SO_NODE_CONSTRUCTOR(AddAllocationLateCube); }
protected:
  ~AddAllocationLateCube() override {}
};

SO_NODE_SOURCE(AddAllocationLateCube);

static int first = 1;
static int second = 2;
static int observed[8];
static int observedCount = 0;

static void record(void * data) {
  if (observedCount < 8) observed[observedCount] = *static_cast<int *>(data);
  ++observedCount;
}

static SoCallbackAction::Response node(void * data, SoCallbackAction *,
                                       const SoNode *) {
  record(data);
  return SoCallbackAction::CONTINUE;
}
static void triangle(void * data, SoCallbackAction *, const SoPrimitiveVertex *,
                     const SoPrimitiveVertex *, const SoPrimitiveVertex *) {
  record(data);
}
static void line(void * data, SoCallbackAction *, const SoPrimitiveVertex *,
                 const SoPrimitiveVertex *) {
  record(data);
}
static void point(void * data, SoCallbackAction *, const SoPrimitiveVertex *) {
  record(data);
}

static void add(SoCallbackAction & action, SoType type, int family, int * tag) {
  switch (family) {
  case 0: action.addPreCallback(type, node, tag); break;
  case 1: action.addPostCallback(type, node, tag); break;
  case 2: action.addTriangleCallback(type, triangle, tag); break;
  case 3: action.addLineSegmentCallback(type, line, tag); break;
  case 4: action.addPointCallback(type, point, tag); break;
  }
}

static void invoke(SoCallbackAction & action, SoCube * cube, int family) {
  SoPrimitiveVertex vertex;
  switch (family) {
  case 0: action.invokePreCallbacks(cube); break;
  case 1: action.invokePostCallbacks(cube); break;
  case 2: action.invokeTriangleCallbacks(cube, &vertex, &vertex, &vertex); break;
  case 3: action.invokeLineSegmentCallbacks(cube, &vertex, &vertex); break;
  case 4: action.invokePointCallbacks(cube, &vertex); break;
  }
}

static bool expectOne() {
  return observedCount == 1 && observed[0] == first;
}

static bool expectBoth() {
  return observedCount == 2 && observed[0] == first && observed[1] == second;
}

int main(int argc, char ** argv) {
  if (argc != 3) return 2;
  const int family = std::atoi(argv[1]);
  failureOrdinal = static_cast<unsigned>(std::atoi(argv[2]));
  if (family < 0 || family > 4) return 2;

  SoDB::init();
  AddAllocationLateCube::initClass();
  SoCube * cube = new SoCube;
  AddAllocationLateCube * late = new AddAllocationLateCube;
  cube->ref();
  late->ref();

  SoCallbackAction action;
  action.apply(cube);
  add(action, SoNode::getClassTypeId(), family, &first);
  invoke(action, cube, family);
  observedCount = 0;
  invoke(action, late, family);
  observedCount = 0;

  allocationCount = 0;
  armed = failureOrdinal != 0;
  bool threw = false;
  try {
    add(action, SoNode::getClassTypeId(), family, &second);
  }
  catch (const std::bad_alloc &) {
    threw = true;
  }
  catch (...) {
    armed = false;
    std::fputs("unexpected exception\n", stderr);
    late->unref();
    cube->unref();
    return 1;
  }
  armed = false;

  int failures = 0;
  invoke(action, cube, family);
  if (threw ? !expectOne() : !expectBoth()) ++failures;
  observedCount = 0;
  invoke(action, late, family);
  if (threw ? !expectOne() : !expectBoth()) ++failures;

  if (threw) {
    add(action, SoNode::getClassTypeId(), family, &second);
    observedCount = 0;
    invoke(action, cube, family);
    if (!expectBoth()) ++failures;
    observedCount = 0;
    invoke(action, late, family);
    if (!expectBoth()) ++failures;
  }

  std::fprintf(stderr,
               "family=%d ordinal=%u allocations=%u injected=%d failures=%d\n",
               family, failureOrdinal, allocationCount, int(threw), failures);
  late->unref();
  cube->unref();
  return failures ? 1 : 0;
}
