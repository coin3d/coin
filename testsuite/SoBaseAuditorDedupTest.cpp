#include <Inventor/SoDB.h>
#include <Inventor/misc/SoNotification.h>
#include <Inventor/nodes/SoGroup.h>

#include <cstdio>

class CountingGroup : public SoGroup {
public:
  CountingGroup() : calls(0) {}
  void notify(SoNotList *) override { ++calls; }
  int calls;
};

static void
notify_auditors(SoGroup * source)
{
  SoNotList list;
  SoNotRec rec(source);
  list.append(&rec);
  list.setLastType(SoNotRec::CONTAINER);
  source->SoBase::notify(&list);
}

int
main()
{
  SoDB::init();
  SoGroup * source = new SoGroup;
  CountingGroup * first = new CountingGroup;
  CountingGroup * second = new CountingGroup;
  source->ref();
  first->ref();
  second->ref();

  source->addAuditor(first, SoNotRec::PARENT);
  source->addAuditor(first, SoNotRec::PARENT);
  notify_auditors(source);
  const bool inline_ok = first->calls == 1;

  source->addAuditor(second, SoNotRec::PARENT);
  notify_auditors(source);
  const bool tree_ok = first->calls == 2 && second->calls == 1;

  source->addAuditor(first, SoNotRec::CONTAINER);
  notify_auditors(source);
  const bool mixed_type_ok = first->calls == 3 && second->calls == 2;

  source->unref();
  first->unref();
  second->unref();
  if (!(inline_ok && tree_ok && mixed_type_ok)) {
    std::fprintf(stderr, "duplicate auditor was notified more than once\n");
    return 1;
  }
  return 0;
}
