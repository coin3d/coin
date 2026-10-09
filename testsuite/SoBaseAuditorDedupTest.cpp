#include <Inventor/SoDB.h>
#include <Inventor/misc/SoNotification.h>
#include <Inventor/nodes/SoGroup.h>
#include <cstdio>
#include <cstring>
#include <stdexcept>

static int failures;
static void check(bool ok, const char * message) {
  if (!ok) { std::fprintf(stderr, "FAIL: %s\n", message); ++failures; }
}
static void notify_auditors(SoGroup * source) {
  SoNotList list;
  SoNotRec rec(source);
  list.append(&rec);
  list.setLastType(SoNotRec::CONTAINER);
  source->SoBase::notify(&list);
  check(list.getLastRec()->getType() == SoNotRec::CONTAINER,
        "notification modified the caller's record");
}
class CountingGroup : public SoGroup {
public:
  CountingGroup() : calls(0), source(NULL), victim(NULL), added(NULL), reenter(false), mutate(false), throwing(false) {}
  void notify(SoNotList *) override {
    ++calls;
    if (throwing) throw std::runtime_error("auditor failure");
    if (reenter) { reenter = false; notify_auditors(source); }
    if (mutate) {
      mutate = false;
      source->removeAuditor(victim, SoNotRec::PARENT);
      source->addAuditor(added, SoNotRec::PARENT);
    }
  }
  int calls;
  SoGroup * source;
  CountingGroup * victim;
  CountingGroup * added;
  bool reenter, mutate, throwing;
};
static void basic() {
  SoGroup * source = new SoGroup;
  CountingGroup * first = new CountingGroup;
  CountingGroup * second = new CountingGroup;
  source->ref(); first->ref(); second->ref();
  notify_auditors(source);
  source->addAuditor(first, SoNotRec::PARENT);
  source->addAuditor(first, SoNotRec::PARENT);
  notify_auditors(source);
  check(first->calls == 1, "inline duplicate was notified twice");
  source->addAuditor(second, SoNotRec::PARENT);
  notify_auditors(source);
  check(first->calls == 2 && second->calls == 1, "heap duplicate was notified twice");
  source->addAuditor(first, SoNotRec::CONTAINER);
  notify_auditors(source);
  check(first->calls == 3 && second->calls == 2, "mixed types notified the same pointer twice");
  source->unref(); first->unref(); second->unref();
}
static void reentrant() {
  SoGroup * source = new SoGroup;
  CountingGroup * first = new CountingGroup;
  CountingGroup * second = new CountingGroup;
  source->ref(); first->ref(); second->ref();
  source->addAuditor(first, SoNotRec::PARENT);
  source->addAuditor(first, SoNotRec::PARENT);
  source->addAuditor(second, SoNotRec::PARENT);
  first->source = source; first->reenter = true;
  notify_auditors(source);
  check(first->calls == 2 && second->calls == 2, "nested notifications shared dedup state");
  first->throwing = true;
  bool caught = false;
  try { notify_auditors(source); }
  catch (const std::runtime_error & e) { caught = std::strcmp(e.what(), "auditor failure") == 0; }
  check(caught, "auditor exception did not propagate");
  first->throwing = false;
  notify_auditors(source);
  check(first->calls == 4 && second->calls == 3, "notification could not be reused after exception");
  source->unref(); first->unref(); second->unref();
}
static void mutation() {
  SoGroup * source = new SoGroup;
  CountingGroup * first = new CountingGroup;
  CountingGroup * victim = new CountingGroup;
  CountingGroup * keeper = new CountingGroup;
  CountingGroup * added = new CountingGroup;
  source->ref(); first->ref(); victim->ref(); keeper->ref(); added->ref();
  source->addAuditor(first, SoNotRec::PARENT);
  source->addAuditor(keeper, SoNotRec::PARENT);
  source->addAuditor(victim, SoNotRec::PARENT);
  source->addAuditor(first, SoNotRec::PARENT);
  first->source = source; first->victim = victim; first->added = added; first->mutate = true;
  notify_auditors(source);
  check(first->calls == 1 && keeper->calls == 1 && victim->calls == 0 && added->calls == 0,
        "mutation delivered a removed/new auditor or duplicated a retained auditor");
  notify_auditors(source);
  check(first->calls == 2 && keeper->calls == 2 && victim->calls == 0 && added->calls == 1,
        "next notification did not use the updated auditors");
  source->unref(); first->unref(); victim->unref(); keeper->unref(); added->unref();
}
static int run(const char * mode) {
  if (std::strcmp(mode, "basic") == 0) basic();
  else if (std::strcmp(mode, "reentrant") == 0) reentrant();
  else if (std::strcmp(mode, "mutation") == 0) mutation();
  else return 2;
  return failures == 0 ? 0 : 1;
}
int main(int argc, char ** argv) {
  SoDB::init();
  const int result = run(argc > 1 ? argv[1] : "basic");
  SoDB::finish();
  return result;
}
