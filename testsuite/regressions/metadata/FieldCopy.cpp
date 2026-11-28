#ifdef METADATA_ALLOCATION_TEST
#include "Allocation.h"
#endif
#include "Fixture.h"
#include <Inventor/fields/SoFieldData.h>
#include <new>
#include <cstring>

class FieldProbe : public ProbeEngine { public: SoSFInt32 extra; };

static void populate(SoFieldData & d, FieldProbe * e, int count) {
  for (int i = 0; i < count; ++i) {
    char name[32]; std::snprintf(name, sizeof name, "field%d", i);
    d.addField(e, name, i % 2 ? &e->extra : &e->input);
  }
  for (int i = 0; i < count; ++i) {
    char name[32]; std::snprintf(name, sizeof name, "value%d", i);
    d.addEnumValue("Mode", name, i * 13 - 9);
  }
  d.addEnumValue("Other", "negative", -19);
}
static void verify(SoFieldData & d, FieldProbe * e, int count) {
  CHECK(d.getNumFields() == count);
  for (int i = 0; i < count; ++i) {
    char name[32]; std::snprintf(name, sizeof name, "field%d", i);
    CHECK(d.getFieldName(i) == SbName(name)); CHECK(d.getField(e, i) == (i % 2 ? &e->extra : &e->input));
  }
  const int * values = NULL; const SbName * names = NULL; int n = -1;
  d.getEnumData("Mode", n, values, names); CHECK(n == count);
  for (int i = 0; i < count; ++i) {
    char name[32]; std::snprintf(name, sizeof name, "value%d", i);
    CHECK(names[i] == SbName(name)); CHECK(values[i] == i * 13 - 9);
  }
  d.getEnumData("Other", n, values, names);
  CHECK(n == 1); CHECK(values[0] == -19); CHECK(names[0] == SbName("negative"));
}
static void independent(SoFieldData & a, SoFieldData & b) {
  CHECK(&a.getFieldName(0) != &b.getFieldName(0));
  const int * av, * bv; const SbName * an, * bn; int ac, bc;
  a.getEnumData("Mode", ac, av, an); b.getEnumData("Mode", bc, bv, bn);
  CHECK(ac == bc); CHECK(av != bv); CHECK(an != bn);
}
static int normal() {
  FieldProbe * e = new FieldProbe; e->ref();
  SoFieldData survivor, chain;
  {
    SoFieldData source; populate(source, e, 40);
    SoFieldData refcopy(source), pointercopy(&source);
    verify(refcopy, e, 40); verify(pointercopy, e, 40);
    independent(source, refcopy); independent(source, pointercopy);
    populate(survivor, e, 2); CHECK(&(chain = survivor = source) == &chain);
    independent(source, survivor); independent(source, chain);
    source.copy(&source); verify(source, e, 40);
    CHECK(&(source = source) == &source); verify(source, e, 40);
    source.addEnumValue("Mode", "later", 99);
    verify(survivor, e, 40); verify(chain, e, 40);
    // Replacement must remove old fields and old enum sets, including on empty input.
    SoFieldData empty, nullcopy(static_cast<const SoFieldData *>(NULL));
    CHECK(nullcopy.getNumFields() == 0);
    refcopy.copy(&empty); CHECK(refcopy.getNumFields() == 0);
    pointercopy = empty; CHECK(pointercopy.getNumFields() == 0);
    const int * values = NULL; const SbName * names = NULL; int n = -1;
    pointercopy.getEnumData("Mode", n, values, names); CHECK(n == 0);
    empty = survivor; verify(empty, e, 40);
  }
  verify(survivor, e, 40); verify(chain, e, 40);
  CHECK(survivor.getIndex(e, &e->input) == 0);
  e->unref(); return 0;
}
#ifdef METADATA_ALLOCATION_TEST
static int failures(int operation) {
  FieldProbe * e = new FieldProbe; e->ref();
  SoFieldData source; populate(source, e, 40);
  { SoFieldData warm(&source); }
  unsigned hits = 0;
  for (long ordinal = 0; ordinal < 512; ++ordinal) {
    {
      SoFieldData destination; populate(destination, e, 2);
      alignas(SoFieldData) unsigned char storage[sizeof(SoFieldData)];
      SoFieldData * constructed = NULL;
      arm(ordinal); bool caught = false;
      try {
        if (operation == 0) constructed = new(storage) SoFieldData(source);
        else if (operation == 1) constructed = new(storage) SoFieldData(&source);
        else if (operation == 2) destination = source;
        else destination.copy(&source);
      } catch (const std::bad_alloc &) { caught = true; }
      disarm(); CHECK(caught == hit);
      if (caught) { ++hits; verify(destination, e, 2); }
      else if (constructed) verify(*constructed, e, 40);
      else verify(destination, e, 40);
      verify(source, e, 40);
      if (constructed) constructed->~SoFieldData();
    }
    CHECK(live == 0);
    if (!hit) break;
  }
  CHECK(hits >= 6); std::printf("field-copy operation=%d injected=%u live=%u\n", operation, hits, live);
  e->unref(); return 0;
}
#endif
int main(int argc, char ** argv) {
  initProbes();
#ifdef METADATA_ALLOCATION_TEST
  CHECK(argc == 2); return failures(std::atoi(argv[1]));
#else
  (void)argc; (void)argv; return normal();
#endif
}
