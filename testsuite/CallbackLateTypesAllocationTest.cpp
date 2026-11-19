#include <Inventor/SoDB.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSubNode.h>
#include <new>
#include <cstdlib>
#include <cstdio>

// Executable symbol interposition is enabled only on supported ELF systems.
// No Coin implementation allocation sites or container sizes are assumed.
static bool armed=false;
static unsigned allocationCount=0, failureOrdinal=0;
static void * allocate(std::size_t bytes) {
  if (armed && ++allocationCount==failureOrdinal) throw std::bad_alloc();
  void * p=std::malloc(bytes ? bytes : 1);
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
class AllocationLateCube : public SoCube {
  SO_NODE_HEADER(AllocationLateCube);
public:
  static void initClass() { SO_NODE_INIT_CLASS(AllocationLateCube,SoCube,"Cube"); }
  AllocationLateCube() { SO_NODE_CONSTRUCTOR(AllocationLateCube); }
protected: ~AllocationLateCube() override {}
};
SO_NODE_SOURCE(AllocationLateCube);
static int tags[3]={1,2,3};
static int observed[64], observedCount=0;
static void record(void * data) {
  if (observedCount<64) observed[observedCount]=*static_cast<int *>(data);
  ++observedCount;
}
static SoCallbackAction::Response nodecb(void * d, SoCallbackAction *, const SoNode *) { record(d); return SoCallbackAction::CONTINUE; }
static void tri(void * d, SoCallbackAction *, const SoPrimitiveVertex *, const SoPrimitiveVertex *, const SoPrimitiveVertex *) { record(d); }
static void line(void * d, SoCallbackAction *, const SoPrimitiveVertex *, const SoPrimitiveVertex *) { record(d); }
static void point(void * d, SoCallbackAction *, const SoPrimitiveVertex *) { record(d); }
static void add(SoCallbackAction & a, SoType t, int family, int * tag) {
  switch (family) {
  case 0: a.addPreCallback(t,nodecb,tag); break;
  case 1: a.addPostCallback(t,nodecb,tag); break;
  case 2: a.addTriangleCallback(t,tri,tag); break;
  case 3: a.addLineSegmentCallback(t,line,tag); break;
  case 4: a.addPointCallback(t,point,tag); break;
  }
}
static bool invoke(SoCallbackAction & a, SoCube * node, int family, SoPrimitiveVertex & v) {
  if (family>=2 && !a.shouldGeneratePrimitives(node)) return false;
  switch (family) {
  case 0: a.invokePreCallbacks(node); break;
  case 1: a.invokePostCallbacks(node); break;
  case 2: a.invokeTriangleCallbacks(node,&v,&v,&v); break;
  case 3: a.invokeLineSegmentCallbacks(node,&v,&v); break;
  case 4: a.invokePointCallbacks(node,&v); break;
  }
  return true;
}
static bool expected() { return observedCount==3 && observed[0]==1 && observed[1]==2 && observed[2]==3; }
int main(int argc, char ** argv) {
  if (argc!=3) return 2;
  int family=std::atoi(argv[1]); failureOrdinal=static_cast<unsigned>(std::atoi(argv[2]));
  if (family<0 || family>4) return 2;
  SoDB::init();
  SoCallbackAction a;
  SoCube * warm=new SoCube; warm->ref(); a.apply(warm); warm->unref();
  add(a,SoNode::getClassTypeId(),family,&tags[0]);
  add(a,SoCube::getClassTypeId(),family,&tags[1]);
  add(a,SoNode::getClassTypeId(),family,&tags[2]);
  AllocationLateCube::initClass();
  AllocationLateCube * late=new AllocationLateCube; late->ref();
  SoPrimitiveVertex vertex;
  bool threw=false, demanded=true;
  allocationCount=0; armed=failureOrdinal!=0;
  try { demanded=invoke(a,late,family,vertex); }
  catch (const std::bad_alloc &) { threw=true; }
  catch (...) { armed=false; std::fprintf(stderr,"unexpected exception\n"); late->unref(); return 1; }
  armed=false;
  int failures=0;
  if (!threw && (!demanded || !expected())) {
    std::fprintf(stderr,"first successful dispatch missing or duplicated callbacks: family=%d ordinal=%u count=%d\n",family,failureOrdinal,observedCount);
    ++failures;
  }
  // A throwing invocation may have delivered a prefix. Retry is a fresh
  // invocation; its complete sequence must contain each registration once.
  for (int attempt=0; attempt<3; ++attempt) {
    observedCount=0;
    if (!invoke(a,late,family,vertex) || !expected()) {
      std::fprintf(stderr,"retry %d missing/duplicated callbacks: family=%d ordinal=%u count=%d\n",attempt,family,failureOrdinal,observedCount);
      ++failures;
    }
  }
  std::fprintf(stderr,"family=%d ordinal=%u allocations=%u injected=%d\n",family,failureOrdinal,allocationCount,int(threw));
  late->unref(); return failures ? 1 : 0;
}
