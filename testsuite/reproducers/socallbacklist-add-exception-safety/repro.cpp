// Deterministic, process-isolated oracle for SoCallbackList add exception safety.
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

namespace FailureInjection {
std::atomic<long> countdown(-1);
std::atomic<bool> armed(false);
std::atomic<bool> triggered(false);

void arm(long n) { triggered.store(false); countdown.store(n); armed.store(true); }
void disarm() { armed.store(false); }
bool wasTriggered() { return triggered.load(); }

bool failNow()
{
  if (!armed.load()) return false;
  if (countdown.fetch_sub(1) != 0) return false;
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

struct Event {
  int id;
  char dispatch;
  const void * callbackdata;
};

struct Recorder {
  Event events[64];
  int count;
  Recorder() : count(0) {}
  void clear() { count = 0; }
  void add(int id, char dispatch, const void * callbackdata)
  {
    if (count < 64) events[count] = Event{id, dispatch, callbackdata};
    ++count;
  }
};

struct Registration {
  Recorder * recorder;
  int id;
};

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

static void typedFailedCallback(void * userdata, SoSelection * callbackdata)
{
  Registration * r = static_cast<Registration *>(userdata);
  r->recorder->add(r->id, 'F', callbackdata);
}

static void typedAfterCallback(void * userdata, SoSelection * callbackdata)
{
  Registration * r = static_cast<Registration *>(userdata);
  r->recorder->add(r->id, 'T', callbackdata);
}

class Trigger : public SoSelection {
public:
  SoCallbackList & callbacks() { return *this->changeCBList; }
  void fire(void * token) { this->changeCBList->invokeCallbacks(token); }
};

static bool exact(const Recorder & got, const int * ids, const char * kinds,
                  int n, const void * token)
{
  if (got.count != n) return false;
  for (int i = 0; i < n; ++i) {
    if (got.events[i].id != ids[i] || got.events[i].dispatch != kinds[i] ||
        got.events[i].callbackdata != token) return false;
  }
  return true;
}

static void addMixedPrefix(Trigger * t, Registration * r)
{
  t->callbacks().addCallback(rawCallback, &r[0]);
  t->addChangeCallback(typedCallback, &r[1]);
  t->callbacks().addCallback(rawCallback, &r[2]);
  t->addChangeCallback(typedCallback, &r[3]);
}

static int rawEmptyControl(void)
{
  Recorder recorder;
  Registration r = { &recorder, 11 };
  SoCallbackList list;
  list.addCallback(rawCallback, &r);
  int token = 1;
  list.invokeCallbacks(&token);
  const int ids[] = { 11 };
  if (list.getNumCallbacks() != 1 || !exact(recorder, ids, "R", 1, &token)) return 1;
  list.removeCallback(rawCallback, &r);
  return list.getNumCallbacks() == 0 ? 0 : 1;
}

static int positiveOperations(void)
{
  Recorder recorder;
  Registration r[5] = {{&recorder, 1}, {&recorder, 2}, {&recorder, 3},
                       {&recorder, 4}, {&recorder, 5}};
  Trigger * t = new Trigger;
  t->ref();
  addMixedPrefix(t, r);
  t->addChangeCallback(typedCallback, &r[1]);
  t->removeChangeCallback(typedCallback, &r[1]); // last duplicate
  int token = 2;
  t->fire(&token);
  const int ids[] = {1, 2, 3, 4};
  bool ok = exact(recorder, ids, "RTRT", 4, &token);
  t->callbacks().clearCallbacks();
  t->callbacks().addCallback(rawAfterCallback, &r[4]);
  recorder.clear();
  t->fire(&token);
  const int finalids[] = {5};
  ok = ok && exact(recorder, finalids, "R", 1, &token);
  t->unref();
  return ok ? 0 : 1;
}

static int rawGrowth(long failafter, bool recoverTyped)
{
  Recorder recorder;
  Registration r[7] = {{&recorder, 1}, {&recorder, 2}, {&recorder, 3},
                       {&recorder, 4}, {&recorder, 90}, {&recorder, 5},
                       {&recorder, 6}};
  Trigger * t = new Trigger;
  t->ref();
  addMixedPrefix(t, r);
  bool threw = false;
  FailureInjection::arm(failafter);
  try { t->callbacks().addCallback(rawCallback, &r[4]); }
  catch (const std::bad_alloc &) { threw = true; }
  catch (...) { FailureInjection::disarm(); t->unref(); return 1; }
  FailureInjection::disarm();
  const bool injected = FailureInjection::wasTriggered();

  int token = 3;
  if (!threw) {
    if (injected) { t->unref(); return 1; } // bad_alloc was swallowed
    t->fire(&token);
    const int ids[] = {1, 2, 3, 4, 90};
    bool ok = t->callbacks().getNumCallbacks() == 5 &&
              exact(recorder, ids, "RTRTR", 5, &token);
    t->unref();
    return ok ? 10 : 1;
  }

  t->fire(&token);
  const int beforeids[] = {1, 2, 3, 4};
  bool ok = t->callbacks().getNumCallbacks() == 4 &&
            exact(recorder, beforeids, "RTRT", 4, &token);
  recorder.clear();
  if (recoverTyped) t->addChangeCallback(typedAfterCallback, &r[6]);
  else t->callbacks().addCallback(rawAfterCallback, &r[5]);
  t->fire(&token);
  const int typedids[] = {1, 2, 3, 4, 6};
  const int rawids[] = {1, 2, 3, 4, 5};
  ok = ok && exact(recorder, recoverTyped ? typedids : rawids,
                   recoverTyped ? "RTRTT" : "RTRTR", 5, &token);
  t->callbacks().clearCallbacks();
  t->unref();
  return ok ? 0 : 1;
}

static int typedAdd(long failafter, bool mixed)
{
  Recorder recorder;
  Registration r[8] = {{&recorder, 1}, {&recorder, 2}, {&recorder, 3},
                       {&recorder, 4}, {&recorder, 90}, {&recorder, 5},
                       {&recorder, 6}, {&recorder, 7}};
  Trigger * t = new Trigger;
  t->ref();
  if (mixed) addMixedPrefix(t, r);
  bool threw = false;
  FailureInjection::arm(failafter);
  try { t->addChangeCallback(typedFailedCallback, &r[4]); }
  catch (const std::bad_alloc &) { threw = true; }
  catch (...) { FailureInjection::disarm(); t->unref(); return 1; }
  FailureInjection::disarm();
  const bool injected = FailureInjection::wasTriggered();

  int token = 4;
  const int prefix = mixed ? 4 : 0;
  if (!threw) {
    if (injected) { t->unref(); return 1; } // bad_alloc was swallowed
    t->fire(&token);
    const int mixedids[] = {1, 2, 3, 4, 90};
    const int emptyids[] = {90};
    bool ok = t->callbacks().getNumCallbacks() == prefix + 1 &&
              exact(recorder, mixed ? mixedids : emptyids,
                    mixed ? "RTRTF" : "F", prefix + 1, &token);
    t->unref();
    return ok ? 10 : 1;
  }

  t->fire(&token);
  const int beforeids[] = {1, 2, 3, 4};
  bool ok = t->callbacks().getNumCallbacks() == prefix &&
            exact(recorder, beforeids, "RTRT", prefix, &token);

  // Mandatory ghost-ownership discriminator: failed typed T_fail, then a
  // different raw callback/function/userdata at precisely the vacated index.
  recorder.clear();
  t->callbacks().addCallback(rawAfterCallback, &r[5]);
  t->fire(&token);
  const int mixedrawids[] = {1, 2, 3, 4, 5};
  const int emptyrawids[] = {5};
  ok = ok && exact(recorder, mixed ? mixedrawids : emptyrawids,
                   mixed ? "RTRTR" : "R", prefix + 1, &token);
  t->callbacks().removeCallback(rawAfterCallback, &r[5]);

  recorder.clear();
  t->addChangeCallback(typedAfterCallback, &r[6]);
  t->fire(&token);
  const int mixedtypedids[] = {1, 2, 3, 4, 6};
  const int emptytypedids[] = {6};
  ok = ok && exact(recorder, mixed ? mixedtypedids : emptytypedids,
                   mixed ? "RTRTT" : "T", prefix + 1, &token);
  t->removeChangeCallback(typedAfterCallback, &r[6]);
  t->callbacks().clearCallbacks();
  t->callbacks().addCallback(rawAfterCallback, &r[7]);
  recorder.clear();
  t->fire(&token);
  const int reuseids[] = {7};
  ok = ok && exact(recorder, reuseids, "R", 1, &token);
  t->unref();
  return ok ? 0 : 1;
}

static int probe(void)
{
  Recorder recorder;
  Registration r[5] = {{&recorder, 1}, {&recorder, 2}, {&recorder, 3},
                       {&recorder, 4}, {&recorder, 5}};
  SoCallbackList list;
  for (int i = 0; i != 4; ++i) list.addCallback(rawCallback, &r[i]);
  bool threw = false;
  FailureInjection::arm(0);
  try { list.addCallback(rawAfterCallback, &r[4]); }
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
  if (argc == 2 && std::strcmp(argv[1], "raw-empty-control") == 0) return rawEmptyControl();
  if (argc == 2 && std::strcmp(argv[1], "positive-ops") == 0) return positiveOperations();
  if (argc != 3) return 2;
  long failafter = -1;
  if (!parseLong(argv[2], failafter)) return 2;
  if (std::strcmp(argv[1], "raw-growth-raw") == 0) return rawGrowth(failafter, false);
  if (std::strcmp(argv[1], "raw-growth-typed") == 0) return rawGrowth(failafter, true);
  if (std::strcmp(argv[1], "typed-empty") == 0) return typedAdd(failafter, false);
  if (std::strcmp(argv[1], "typed-mixed") == 0) return typedAdd(failafter, true);
  return 2;
}
