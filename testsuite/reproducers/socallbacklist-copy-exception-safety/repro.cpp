// Deterministic, process-isolated oracle for SoCallbackList copy/assignment.
// Exit 0: observed contract preserved; 10: uninjected control succeeded;
// 77: allocator interposition is opaque; any other exit: violation.

#include "../CoinCleanup.h"

#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/lists/SoCallbackList.h>
#include <Inventor/nodes/SoSelection.h>

#include <atomic>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include <type_traits>

namespace FailureInjection {
std::atomic<long> countdown(-1);
std::atomic<bool> armed(false);
std::atomic<bool> triggered(false);
void arm(long n) { triggered.store(false); countdown.store(n); armed.store(true); }
void disarm() { armed.store(false); }
bool wasTriggered() { return triggered.load(); }
bool failNow()
{
  if (!armed.load() || countdown.fetch_sub(1) != 0) return false;
  triggered.store(true);
  return true;
}
}

void * operator new(std::size_t n)
{
  if (FailureInjection::failNow()) throw std::bad_alloc();
  if (void * p = std::malloc(n ? n : 1)) return p;
  throw std::bad_alloc();
}
void * operator new[](std::size_t n)
{
  if (FailureInjection::failNow()) throw std::bad_alloc();
  if (void * p = std::malloc(n ? n : 1)) return p;
  throw std::bad_alloc();
}
void operator delete(void * p) noexcept { std::free(p); }
void operator delete[](void * p) noexcept { std::free(p); }
#if __cplusplus >= 201402L
void operator delete(void * p, std::size_t) noexcept { std::free(p); }
void operator delete[](void * p, std::size_t) noexcept { std::free(p); }
#endif

struct Event { int id; char dispatch; const void * callbackdata; };
struct Recorder {
  Event events[96];
  int count;
  Recorder() : count(0) {}
  void clear() { count = 0; }
  void add(int id, char dispatch, const void * callbackdata)
  {
    if (count < 96) events[count] = Event{id, dispatch, callbackdata};
    ++count;
  }
};
struct Registration { Recorder * recorder; int id; };

static void rawCallback(void * userdata, void * callbackdata)
{
  Registration * r = static_cast<Registration *>(userdata);
  r->recorder->add(r->id, 'R', callbackdata);
}
static void rawAfterCallback(void * userdata, void * callbackdata)
{
  Registration * r = static_cast<Registration *>(userdata);
  r->recorder->add(r->id, 'R', callbackdata);
}
static void typedCallback(void * userdata, SoSelection * callbackdata)
{
  Registration * r = static_cast<Registration *>(userdata);
  r->recorder->add(r->id, 'T', callbackdata);
}
static void typedAfterCallback(void * userdata, SoSelection * callbackdata)
{
  Registration * r = static_cast<Registration *>(userdata);
  r->recorder->add(r->id, 'T', callbackdata);
}

class Trigger : public SoSelection {
public:
  SoCallbackList & callbacks() { return *this->changeCBList; }
};

enum FixtureKind { RAW_ONLY, TYPED_ONLY, MIXED };

static void populate(Trigger * t, FixtureKind kind, Registration * regs, int base,
                     int count)
{
  for (int i = 0; i < count; ++i) {
    regs[i].id = base + i;
    if (kind == TYPED_ONLY || (kind == MIXED && !(i & 1)))
      t->addChangeCallback(typedCallback, &regs[i]);
    else
      t->callbacks().addCallback(rawCallback, &regs[i]);
  }
}

static char expectedKind(FixtureKind kind, int i)
{
  return kind == TYPED_ONLY || (kind == MIXED && !(i & 1)) ? 'T' : 'R';
}

static bool verify(SoCallbackList & list, Recorder & recorder, FixtureKind kind,
                   int base, int count, const void * token, int extraId = -1,
                   char extraKind = 'R')
{
  recorder.clear();
  list.invokeCallbacks(const_cast<void *>(token));
  const int expected = count + (extraId >= 0 ? 1 : 0);
  if (list.getNumCallbacks() != expected || recorder.count != expected) return false;
  for (int i = 0; i < count; ++i) {
    if (recorder.events[i].id != base + i ||
        recorder.events[i].dispatch != expectedKind(kind, i) ||
        recorder.events[i].callbackdata != token) return false;
  }
  if (extraId >= 0) {
    const Event & e = recorder.events[count];
    if (e.id != extraId || e.dispatch != extraKind || e.callbackdata != token) return false;
  }
  return true;
}

static Trigger * makeTrigger(void)
{
  Trigger * t = new Trigger;
  t->ref();
  return t;
}

static int copyCase(long failafter, FixtureKind kind)
{
  Recorder recorder;
  Registration regs[7];
  for (int i = 0; i < 7; ++i) regs[i].recorder = &recorder;
  Trigger * source = makeTrigger();
  populate(source, kind, regs, 100, 6);
  int sourceToken = 11;
  int copyToken = 12;
  typedef typename std::aligned_storage<sizeof(SoCallbackList), alignof(SoCallbackList)>::type Storage;
  Storage storage;
  SoCallbackList * copy = NULL;
  bool threw = false;
  FailureInjection::arm(failafter);
  try { copy = new (&storage) SoCallbackList(source->callbacks()); }
  catch (const std::bad_alloc &) { threw = true; }
  catch (...) { FailureInjection::disarm(); source->unref(); return 1; }
  FailureInjection::disarm();
  const bool injected = FailureInjection::wasTriggered();

  if (!threw) {
    if (injected) { copy->~SoCallbackList(); source->unref(); return 1; }
    bool ok = verify(source->callbacks(), recorder, kind, 100, 6, &sourceToken);
    source->callbacks().clearCallbacks();
    source->unref();
    ok = ok && verify(*copy, recorder, kind, 100, 6, &copyToken);
    if (kind != TYPED_ONLY) {
      // MIXED is T,R,T,R,T,R, so regs[1] is the first raw registration.
      copy->removeCallback(rawCallback, &regs[kind == MIXED ? 1 : 0]);
      ok = ok && copy->getNumCallbacks() == 5;
    }
    copy->~SoCallbackList();
    return ok ? 10 : 1;
  }

  bool ok = verify(source->callbacks(), recorder, kind, 100, 6, &sourceToken);

  // Reconstruct at exactly the failed object's address. Since the side registry
  // is keyed by the datalist address, only same-address reuse reliably exposes a
  // constructor-partial ghost (B-M8).
  SoCallbackList * reused = new (&storage) SoCallbackList;
  regs[6].id = 190;
  recorder.clear();
  reused->addCallback(rawAfterCallback, &regs[6]);
  reused->invokeCallbacks(&copyToken);
  ok = ok && reused->getNumCallbacks() == 1 && recorder.count == 1 &&
       recorder.events[0].id == 190 && recorder.events[0].dispatch == 'R' &&
       recorder.events[0].callbackdata == &copyToken;
  reused->removeCallback(rawAfterCallback, &regs[6]);
  reused->~SoCallbackList();

  regs[6].id = 190;
  source->callbacks().addCallback(rawAfterCallback, &regs[6]);
  ok = ok && verify(source->callbacks(), recorder, kind, 100, 6, &sourceToken, 190, 'R');
  source->callbacks().removeCallback(rawAfterCallback, &regs[6]);
  source->callbacks().clearCallbacks();
  source->unref();
  return ok ? 0 : 1;
}

static int assignmentCase(long failafter, FixtureKind sourceKind,
                          FixtureKind destinationKind)
{
  Recorder recorder;
  Registration sourceRegs[7];
  Registration destinationRegs[8];
  for (int i = 0; i < 7; ++i) sourceRegs[i].recorder = &recorder;
  for (int i = 0; i < 8; ++i) destinationRegs[i].recorder = &recorder;
  Trigger * source = makeTrigger();
  Trigger * destination = makeTrigger();
  populate(source, sourceKind, sourceRegs, 100, 6);
  // Three registrations retain inline capacity four. Assigning the six-entry
  // source therefore forces independent reallocations of both public lists in
  // a sequential memberwise implementation, making B-M1 observable at k=1.
  populate(destination, destinationKind, destinationRegs, 200, 3);
  int sourceToken = 21;
  int destinationToken = 22;
  bool threw = false;
  FailureInjection::arm(failafter);
  try { destination->callbacks() = source->callbacks(); }
  catch (const std::bad_alloc &) { threw = true; }
  catch (...) {
    FailureInjection::disarm(); source->unref(); destination->unref(); return 1;
  }
  FailureInjection::disarm();
  const bool injected = FailureInjection::wasTriggered();

  if (!threw) {
    if (injected) { source->unref(); destination->unref(); return 1; }
    bool ok = verify(source->callbacks(), recorder, sourceKind, 100, 6, &sourceToken) &&
              verify(destination->callbacks(), recorder, sourceKind, 100, 6, &destinationToken);
    source->callbacks().clearCallbacks();
    source->unref();
    ok = ok && verify(destination->callbacks(), recorder, sourceKind, 100, 6, &destinationToken);
    destination->callbacks().clearCallbacks();
    destination->unref();
    return ok ? 10 : 1;
  }

  bool ok = verify(source->callbacks(), recorder, sourceKind, 100, 6, &sourceToken) &&
            verify(destination->callbacks(), recorder, destinationKind, 200, 3, &destinationToken);

  // Recovery with both dispatch families exposes hybrid lists and ghost owners.
  destinationRegs[6].id = 290;
  destination->callbacks().addCallback(rawAfterCallback, &destinationRegs[6]);
  ok = ok && verify(destination->callbacks(), recorder, destinationKind, 200, 3,
                    &destinationToken, 290, 'R');
  destination->callbacks().removeCallback(rawAfterCallback, &destinationRegs[6]);
  destinationRegs[7].id = 291;
  destination->addChangeCallback(typedAfterCallback, &destinationRegs[7]);
  ok = ok && verify(destination->callbacks(), recorder, destinationKind, 200, 3,
                    &destinationToken, 291, 'T');
  destination->removeChangeCallback(typedAfterCallback, &destinationRegs[7]);
  source->callbacks().clearCallbacks();
  destination->callbacks().clearCallbacks();
  source->unref();
  destination->unref();
  return ok ? 0 : 1;
}

static int selfAssignment(void)
{
  Recorder recorder;
  Registration regs[6];
  for (int i = 0; i < 6; ++i) regs[i].recorder = &recorder;
  Trigger * t = makeTrigger();
  populate(t, MIXED, regs, 300, 6);
  int token = 31;
  bool threw = false;
  FailureInjection::arm(0);
  try { t->callbacks() = t->callbacks(); }
  catch (const std::bad_alloc &) { threw = true; }
  catch (...) { FailureInjection::disarm(); t->unref(); return 1; }
  FailureInjection::disarm();
  const bool injected = FailureInjection::wasTriggered();
  if (injected && !threw) { t->unref(); return 1; }
  bool ok = verify(t->callbacks(), recorder, MIXED, 300, 6, &token);
  t->removeChangeCallback(typedCallback, &regs[4]);
  ok = ok && t->callbacks().getNumCallbacks() == 5;
  t->callbacks().clearCallbacks();
  t->unref();
  (void)threw; // Either no allocation or propagated bad_alloc is permitted.
  return ok ? 0 : 1;
}

static int positiveChain(void)
{
  Recorder recorder;
  Registration regs[6];
  for (int i = 0; i < 6; ++i) regs[i].recorder = &recorder;
  Trigger * source = makeTrigger();
  populate(source, MIXED, regs, 400, 6);
  SoCallbackList copy(source->callbacks());
  SoCallbackList assigned;
  assigned = copy;
  assigned = assigned;
  source->callbacks().clearCallbacks();
  source->unref();
  int token = 41;
  bool ok = verify(copy, recorder, MIXED, 400, 6, &token) &&
            verify(assigned, recorder, MIXED, 400, 6, &token);
  copy.clearCallbacks();
  assigned.clearCallbacks();
  return ok ? 0 : 1;
}

static int probe(void)
{
  Recorder recorder;
  Registration regs[5];
  for (int i = 0; i < 5; ++i) { regs[i].recorder = &recorder; regs[i].id = i; }
  SoCallbackList list;
  for (int i = 0; i < 4; ++i) list.addCallback(rawCallback, &regs[i]);
  bool threw = false;
  FailureInjection::arm(0);
  try { list.addCallback(rawAfterCallback, &regs[4]); }
  catch (const std::bad_alloc &) { threw = true; }
  catch (...) { FailureInjection::disarm(); return 1; }
  FailureInjection::disarm();
  if (!threw) {
    if (FailureInjection::wasTriggered()) {
      std::fprintf(stderr, "VIOLATED allocation point was reached but bad_alloc was swallowed\n");
      return 1;
    }
    std::fprintf(stderr, "UNKNOWN allocator replacement did not reach shared-library allocation\n");
    return 77;
  }
  return 0;
}

static bool parseLong(const char * text, long & value)
{
  char * end = NULL;
  errno = 0;
  value = std::strtol(text, &end, 10);
  return errno == 0 && end != text && *end == '\0' && value >= 0;
}

int main(int argc, char ** argv)
{
  SoDB::init();
  CoinReproducerCleanup cleanup;
  SoInteraction::init();
  if (argc == 2 && std::strcmp(argv[1], "probe") == 0) return probe();
  if (argc == 2 && std::strcmp(argv[1], "self") == 0) return selfAssignment();
  if (argc == 2 && std::strcmp(argv[1], "positive-chain") == 0) return positiveChain();
  if (argc != 3) return 2;
  long failafter = -1;
  if (!parseLong(argv[2], failafter)) return 2;
  if (std::strcmp(argv[1], "copy-raw") == 0) return copyCase(failafter, RAW_ONLY);
  if (std::strcmp(argv[1], "copy-typed") == 0) return copyCase(failafter, TYPED_ONLY);
  if (std::strcmp(argv[1], "copy-mixed") == 0) return copyCase(failafter, MIXED);
  if (std::strcmp(argv[1], "assign-raw-mixed") == 0)
    return assignmentCase(failafter, RAW_ONLY, MIXED);
  if (std::strcmp(argv[1], "assign-typed-raw") == 0)
    return assignmentCase(failafter, TYPED_ONLY, RAW_ONLY);
  if (std::strcmp(argv[1], "assign-mixed-typed") == 0)
    return assignmentCase(failafter, MIXED, TYPED_ONLY);
  if (std::strcmp(argv[1], "assign-mixed-mixed") == 0)
    return assignmentCase(failafter, MIXED, MIXED);
  return 2;
}
