#include "Support.h"
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/misc/SoNotification.h>
#include <cstring>

namespace {
class NotifyField : public SoSFInt32 {
public:
  bool armed, recurse;
  int records;
  NotifyField() : armed(false), recurse(false), records(0) {}
protected:
  SoNotRec createNotRec(SoBase * container) override {
    ++records;
    if (recurse) {
      SoNotList nested;
      this->notify(&nested);
    }
    if (armed) throw &fielddb::failure;
    return SoSFInt32::createNotRec(container);
  }
};
class NotifyContainer : public SoSeparator {
public:
  bool armed, recurse;
  SoField * field;
  int calls;
  NotifyContainer() : armed(false), recurse(false), field(NULL), calls(0) {}
  void notify(SoNotList *) override {
    ++calls;
    if (recurse) { SoNotList nested; field->notify(&nested); }
    if (armed) throw &fielddb::failure;
  }
};
class Auditor : public SoSFInt32 {
public:
  bool armed; int calls;
  Auditor() : armed(false), calls(0) {}
  void notify(SoNotList *) override {
    ++calls;
    if (armed) throw &fielddb::failure;
  }
};
class ValueField : public SoSFInt32 {
public:
  bool armed, recurse;
  int starts;
  ValueField() : armed(false), recurse(false), starts(0) {}
  void startNotify() override {
    ++starts;
    REQUIRE(this->isReadOnly());
    if (recurse) {
      this->setValue(82);
      // The inner valueChanged did not acquire READONLY and must not clear it.
      REQUIRE(this->isReadOnly());
      REQUIRE(starts == 1);
    }
    if (armed) throw &fielddb::failure;
  }
};
void notifyOnce(SoField & field)
{
  SoNotList list;
  field.notify(&list);
}
void recordFailure()
{
  NotifyField field;
  field.armed = true; field.recurse = true;
  field.setDirty(FALSE); field.setDefault(TRUE);
  fielddb::expectFailure([&]() { notifyOnce(field); });
  REQUIRE(field.records == 1);
  REQUIRE(!field.isDefault() && !field.getDirty() && field.isNotifyEnabled());
  field.armed = false;
  notifyOnce(field);
  REQUIRE(field.records == 2);
  notifyOnce(field);
  REQUIRE(field.records == 3);
}
void downstreamFailure(bool containerFailure)
{
  NotifyContainer * container = new NotifyContainer;
  container->ref();
  NotifyField field;
  field.setContainer(container); container->field = &field;
  container->recurse = true;
  Auditor auditor;
  field.addAuditor(&auditor, SoNotRec::FIELD);
  container->armed = containerFailure;
  auditor.armed = !containerFailure;
  fielddb::expectFailure([&]() { notifyOnce(field); });
  REQUIRE(field.records == 1 && container->calls == 1);
  REQUIRE(auditor.calls == (containerFailure ? 0 : 1));
  container->armed = false; auditor.armed = false;
  notifyOnce(field);
  REQUIRE(field.records == 2 && container->calls == 2);
  REQUIRE(auditor.calls == (containerFailure ? 1 : 2));
  notifyOnce(field);
  REQUIRE(field.records == 3 && container->calls == 3);
  field.removeAuditor(&auditor, SoNotRec::FIELD);
  field.setContainer(NULL); container->unref();
}
void valueFailure(bool recurse)
{
  NotifyContainer * container = new NotifyContainer;
  container->ref();
  ValueField field;
  field.setContainer(container);
  field.setDefault(TRUE); field.setDirty(TRUE);
  field.recurse = recurse; field.armed = true;
  fielddb::expectFailure([&]() { field.setValue(71); });
  REQUIRE(field.starts == 1);
  REQUIRE(!field.isReadOnly());
  REQUIRE(!field.getDirty() && !field.isDefault() && field.isNotifyEnabled());
  REQUIRE(field.getValue() == (recurse ? 82 : 71));
  field.armed = false; field.recurse = false;
  field.setValue(93);
  REQUIRE(field.starts == 2 && field.getValue() == 93 && !field.isReadOnly());
  field.setContainer(NULL); container->unref();
}
void controls(bool silent)
{
  NotifyField a, b;
  REQUIRE(a.connectFrom(&b, TRUE));
  REQUIRE(b.connectFrom(&a, TRUE));
  a.recurse = true; b.recurse = true;
  a.setDefault(TRUE); a.setDirty(FALSE);
  if (silent) {
    a.enableNotify(FALSE);
    SoNotList list;
    SoNotRec incoming(NULL);
    incoming.setType(SoNotRec::FIELD);
    list.append(&incoming);
    a.notify(&list);
    REQUIRE(a.records == 0 && b.records == 0);
    REQUIRE(!a.isDefault() && a.getDirty() && !a.isNotifyEnabled());
    a.enableNotify(TRUE);
  }
  notifyOnce(a);
  REQUIRE(a.records == 1 && b.records == 1);
  notifyOnce(a);
  REQUIRE(a.records == 2 && b.records == 2);
  REQUIRE(a.isNotifyEnabled() && b.isNotifyEnabled());
  a.disconnect(); b.disconnect();
}
}
int main(int argc, char ** argv)
{
  REQUIRE(argc == 2); fielddb::init();
  if (!std::strcmp(argv[1], "record")) recordFailure();
  else if (!std::strcmp(argv[1], "container")) downstreamFailure(true);
  else if (!std::strcmp(argv[1], "auditor")) downstreamFailure(false);
  else if (!std::strcmp(argv[1], "value")) valueFailure(false);
  else if (!std::strcmp(argv[1], "value-reentrant")) valueFailure(true);
  else controls(!std::strcmp(argv[1], "silent"));
  REQUIRE(!SoDB::isNotifying());
  return 0;
}
