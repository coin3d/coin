#include <Inventor/SoDB.h>
#include <Inventor/lists/SoAuditorList.h>
#include <Inventor/nodes/SoGroup.h>

#include <cstdio>

int
main()
{
  SoDB::init();
  SoGroup * node = new SoGroup;
  node->ref();
  int first = 0, second = 0, third = 0;
  node->addAuditor(&first, SoNotRec::PARENT);
  node->addAuditor(&second, SoNotRec::PARENT);
  node->addAuditor(&third, SoNotRec::PARENT);

  const SoAuditorList & initial = node->getAuditors();
  bool ok = initial.getLength() == 3 &&
    initial.find(&first, SoNotRec::PARENT) >= 0 &&
    initial.find(&second, SoNotRec::PARENT) >= 0 &&
    initial.find(&third, SoNotRec::PARENT) >= 0;

  node->removeAuditor(&second, SoNotRec::PARENT);
  const SoAuditorList & refreshed = node->getAuditors();
  ok = ok && refreshed.getLength() == 2 &&
    refreshed.find(&first, SoNotRec::PARENT) >= 0 &&
    refreshed.find(&second, SoNotRec::PARENT) == -1 &&
    refreshed.find(&third, SoNotRec::PARENT) >= 0;

  node->removeAuditor(&first, SoNotRec::PARENT);
  node->removeAuditor(&third, SoNotRec::PARENT);
  node->unref();
  if (!ok) std::fprintf(stderr, "auditor cache did not match the tree\n");
  return ok ? 0 : 1;
}
