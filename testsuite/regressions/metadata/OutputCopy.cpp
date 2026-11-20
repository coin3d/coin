#ifdef METADATA_ALLOCATION_TEST
#include "Allocation.h"
#endif
#include "Fixture.h"
#include <Inventor/engines/SoOutputData.h>
#include <Inventor/fields/SoSFFloat.h>
#include <cstring>
#include <new>

template<class T> void populate(SoEngineOutputData & d, T * e, int count) {
  for (int i = 0; i < count; ++i) {
    char name[32]; std::snprintf(name, sizeof name, "output%d", i);
    d.addOutput(e, name, i % 2 ? &e->second : &e->first,
                i % 2 ? SoSFInt32::getClassTypeId() : SoSFFloat::getClassTypeId());
  }
}
template<class T> void verify(const SoEngineOutputData & d, T * e, int count) {
  CHECK(d.getNumOutputs() == count);
  for (int i = 0; i < count; ++i) {
    char name[32]; std::snprintf(name, sizeof name, "output%d", i);
    CHECK(d.getOutputName(i) == SbName(name));
    CHECK(d.getOutput(e, i) == (i % 2 ? &e->second : &e->first));
    CHECK(d.getType(i) == (i % 2 ? SoSFInt32::getClassTypeId() : SoSFFloat::getClassTypeId()));
  }
}
template<class T> int normal() {
  T * e = new T; e->ref();
  SoEngineOutputData survivor, chain;
  {
    SoEngineOutputData source; populate(source, e, 40);
    SoEngineOutputData refcopy(source), pointercopy(&source);
    verify(refcopy, e, 40); verify(pointercopy, e, 40);
    CHECK(&source.getOutputName(0) != &refcopy.getOutputName(0));
    CHECK(&source.getOutputName(0) != &pointercopy.getOutputName(0));
    populate(survivor, e, 2);
    CHECK(&(chain = survivor = source) == &chain);
    CHECK(&source.getOutputName(0) != &survivor.getOutputName(0));
    CHECK(&(source = source) == &source); verify(source, e, 40);
    SoEngineOutputData empty, nullcopy(static_cast<const SoEngineOutputData *>(NULL));
    CHECK(nullcopy.getNumOutputs() == 0);
    refcopy = empty; CHECK(refcopy.getNumOutputs() == 0);
    empty = source; verify(empty, e, 40);
  }
  verify(survivor, e, 40); verify(chain, e, 40);
  CHECK(survivor.getIndex(e, &e->second) == 1);
  e->unref(); return 0;
}
#ifdef METADATA_ALLOCATION_TEST
template<class T> int failures(int operation) {
  T * e = new T; e->ref();
  SoEngineOutputData source; populate(source, e, 40);
  // Warm names and types before tracking; copying cannot legitimately intern names.
  { SoEngineOutputData warm(&source); }
  unsigned hits = 0;
  for (long ordinal = 0; ordinal < 256; ++ordinal) {
    {
      SoEngineOutputData destination; populate(destination, e, 2);
      alignas(SoEngineOutputData) unsigned char storage[sizeof(SoEngineOutputData)];
      SoEngineOutputData * constructed = NULL;
      arm(ordinal);
      bool caught = false;
      try {
        if (operation == 0) constructed = new(storage) SoEngineOutputData(source);
        else if (operation == 1) constructed = new(storage) SoEngineOutputData(&source);
        else destination = source;
      } catch (const std::bad_alloc &) { caught = true; }
      disarm();
      CHECK(caught == hit);
      if (caught) { ++hits; verify(destination, e, 2); }
      else if (constructed) verify(*constructed, e, 40);
      else verify(destination, e, 40);
      verify(source, e, 40);
      if (constructed) constructed->~SoEngineOutputData();
    }
    CHECK(live == 0);
    if (!hit) break;
  }
  CHECK(hits >= 3); std::printf("output-copy operation=%d injected=%u live=%u\n", operation, hits, live);
  e->unref(); return 0;
}
#endif
int main(int argc, char ** argv) {
  initProbes();
#ifdef METADATA_ALLOCATION_TEST
  CHECK(argc == 3); int operation = std::atoi(argv[2]);
  return !std::strcmp(argv[1], "node") ? failures<ProbeNode>(operation) : failures<ProbeEngine>(operation);
#else
  CHECK(argc == 2);
  return !std::strcmp(argv[1], "node") ? normal<ProbeNode>() : normal<ProbeEngine>();
#endif
}
