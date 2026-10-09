#include <Inventor/SoDB.h>
#include <Inventor/nodes/SoGroup.h>

#include <cstdio>

class DetachingGroup : public SoGroup {
public:
  explicit DetachingGroup(SoGroup * target) : observed(target), calls(0) {}

  void notify(SoNotList *) override
  {
    ++calls;
    observed->removeAuditor(this, SoNotRec::PARENT);
  }

  SoGroup * observed;
  int calls;
};

int
main()
{
  SoDB::init();
  SoGroup * observed = new SoGroup;
  SoGroup * first = new SoGroup;
  SoGroup * second = new SoGroup;
  DetachingGroup * third = new DetachingGroup(observed);
  observed->ref();
  first->ref();
  second->ref();
  third->ref();

  observed->addAuditor(first, SoNotRec::PARENT);
  observed->addAuditor(second, SoNotRec::PARENT);
  observed->addAuditor(third, SoNotRec::PARENT);
  observed->startNotify();
  observed->startNotify();
  const bool ok = third->calls == 1;

  observed->removeAuditor(first, SoNotRec::PARENT);
  observed->removeAuditor(second, SoNotRec::PARENT);
  observed->unref();
  first->unref();
  second->unref();
  third->unref();
  SoDB::finish();
  if (!ok) std::fprintf(stderr, "detached auditor was notified again\n");
  return ok ? 0 : 1;
}
