#include "Support.h"
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/sensors/SoOneShotSensor.h>
#include <Inventor/misc/SoNotification.h>
#include <cstring>

namespace {
void count(void * data, SoSensor *) { ++*static_cast<int *>(data); }
void fail(void * data, SoSensor *)
{
  count(data, NULL);
  throw &fielddb::failure;
}
struct State {
  bool armed, catchNested, caughtNested;
  SoOneShotSensor * pending;
  SoField * nestedField;
  SoBase * nestedBase;
  int notifications;
  int * dispatched;
  State() : armed(false), catchNested(false), caughtNested(false), pending(NULL),
    nestedField(NULL), nestedBase(NULL), notifications(0), dispatched(NULL) {}
  void propagate() {
    ++notifications;
    REQUIRE(SoDB::isNotifying());
    if (pending) pending->schedule();
    if (nestedField || nestedBase) {
      try {
        if (nestedField) nestedField->startNotify();
        else nestedBase->startNotify();
      }
      catch (fielddb::Failure * error) {
        REQUIRE(error == &fielddb::failure);
        if (!catchNested) throw;
        caughtNested = true;
      }
      // The nested session must leave the outer session active, whether its
      // failure was caught here or it completed normally.
      REQUIRE(SoDB::isNotifying());
      REQUIRE(dispatched && *dispatched == 0);
    }
    if (armed) throw &fielddb::failure;
  }
};
class SessionField : public SoSFInt32 {
public:
  State state;
  void notify(SoNotList *) override { state.propagate(); }
};
class SessionBase : public SoSeparator {
public:
  State state;
  void notify(SoNotList *) override { state.propagate(); }
};
void databaseOnOtherThread()
{
  fielddb::onOtherThread([]() {
    REQUIRE(!SoDB::isNotifying());
    SoDB::startNotify();
    // Detect a double decrement that would otherwise look "not notifying".
    REQUIRE(SoDB::isNotifying());
    SoDB::endNotify();
    REQUIRE(!SoDB::isNotifying());
  });
}
void futureSessionDrains()
{
  int calls = 0;
  SoOneShotSensor pending(count, &calls);
  pending.setPriority(0); pending.schedule();
  SoDB::startNotify();
  REQUIRE(SoDB::isNotifying() && calls == 0);
  SoDB::endNotify();
  REQUIRE(calls == 1 && !pending.isScheduled() && !SoDB::isNotifying());
}
void propagationFailure(bool base)
{
  int calls = 0;
  SoOneShotSensor queued(count, &calls), existing(count, &calls);
  queued.setPriority(0); existing.setPriority(0);
  existing.schedule();
  SessionField field;
  SessionBase * node = new SessionBase;
  node->ref();
  State & state = base ? node->state : field.state;
  state.armed = true; state.pending = &queued;
  fielddb::expectFailure([&]() {
    if (base) node->startNotify(); else field.startNotify();
  });
  REQUIRE(state.notifications == 1 && !SoDB::isNotifying());
  REQUIRE(calls == 0 && queued.isScheduled() && existing.isScheduled());
  // The first later ordinary session is allowed to dispatch retained work.
  databaseOnOtherThread();
  REQUIRE(calls == 2 && !queued.isScheduled() && !existing.isScheduled());
  state.armed = false;
  if (base) node->startNotify(); else field.startNotify();
  REQUIRE(state.notifications == 2 && calls == 3 && !SoDB::isNotifying());
  futureSessionDrains(); node->unref();
}
void endCallbackFailure(const char * mode)
{
  int calls = 0;
  SoOneShotSensor queued(fail, &calls);
  queued.setPriority(0);
  SessionField field;
  SessionBase * node = new SessionBase;
  node->ref();
  const bool base = !std::strcmp(mode, "end-base");
  const bool direct = !std::strcmp(mode, "end-direct");
  if (direct) {
    SoDB::startNotify();
    SoDB::startNotify();
    queued.schedule();
    SoDB::endNotify();
    REQUIRE(calls == 0 && SoDB::isNotifying());
    fielddb::expectFailure([]() { SoDB::endNotify(); });
  }
  else {
    State & state = base ? node->state : field.state;
    state.pending = &queued;
    fielddb::expectFailure([&]() {
      if (base) node->startNotify(); else field.startNotify();
    });
    REQUIRE(state.notifications == 1);
  }
  REQUIRE(calls == 1 && !SoDB::isNotifying());
  REQUIRE(!queued.isScheduled());
  databaseOnOtherThread();
  futureSessionDrains(); node->unref();
}
void nested(const char * mode)
{
  int calls = 0;
  SoOneShotSensor queued(count, &calls);
  queued.setPriority(0);
  SessionField field;
  SessionBase * node = new SessionBase;
  node->ref();
  const bool baseInner = std::strstr(mode, "base") != NULL;
  const bool failure = std::strstr(mode, "caught") != NULL ||
                       std::strstr(mode, "escape") != NULL;
  const bool caught = std::strstr(mode, "caught") != NULL;
  State & inner = baseInner ? node->state : field.state;
  State & outer = baseInner ? field.state : node->state;
  inner.pending = &queued; inner.armed = failure;
  if (baseInner) outer.nestedBase = node;
  else outer.nestedField = &field;
  outer.catchNested = caught; outer.dispatched = &calls;
  const auto start = [&]() {
    if (baseInner) field.startNotify(); else node->startNotify();
  };
  if (failure && !caught) fielddb::expectFailure(start);
  else start();
  REQUIRE(inner.notifications == 1 && outer.notifications == 1);
  REQUIRE(outer.caughtNested == caught);
  REQUIRE(!SoDB::isNotifying());
  if (failure && !caught) {
    REQUIRE(calls == 0 && queued.isScheduled());
    databaseOnOtherThread();
  }
  REQUIRE(calls == 1 && !queued.isScheduled());
  databaseOnOtherThread();
  futureSessionDrains(); node->unref();
}
void normal()
{
  int calls = 0;
  SoOneShotSensor queued(count, &calls);
  queued.setPriority(0);
  SessionField field;
  SessionBase * node = new SessionBase;
  node->ref();
  field.state.pending = &queued; node->state.pending = &queued;
  SoDB::startNotify();
  field.startNotify(); node->startNotify();
  REQUIRE(calls == 0 && queued.isScheduled() && SoDB::isNotifying());
  SoDB::endNotify();
  REQUIRE(calls == 1 && !queued.isScheduled() && !SoDB::isNotifying());
  databaseOnOtherThread(); futureSessionDrains(); node->unref();
}
}
int main(int argc, char ** argv)
{
  REQUIRE(argc == 2); fielddb::init();
  if (!std::strcmp(argv[1], "field-failure")) propagationFailure(false);
  else if (!std::strcmp(argv[1], "base-failure")) propagationFailure(true);
  else if (!std::strncmp(argv[1], "end-", 4)) endCallbackFailure(argv[1]);
  else if (!std::strncmp(argv[1], "nested-", 7)) nested(argv[1]);
  else normal();
  return 0;
}
