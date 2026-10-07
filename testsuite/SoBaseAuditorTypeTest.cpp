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
  int auditor = 0;
  node->addAuditor(&auditor, SoNotRec::PARENT);
  node->addAuditor(&auditor, SoNotRec::SENSOR);
  node->removeAuditor(&auditor, SoNotRec::SENSOR);

  const SoAuditorList & remaining = node->getAuditors();
  const bool ok = remaining.getLength() == 1 &&
    remaining.find(&auditor, SoNotRec::PARENT) >= 0 &&
    remaining.find(&auditor, SoNotRec::SENSOR) == -1;

  node->removeAuditor(&auditor, SoNotRec::PARENT);
  node->unref();

  SoGroup * larger = new SoGroup;
  larger->ref();
  int a = 0, b = 0, c = 0, target = 0;
  larger->addAuditor(&a, SoNotRec::PARENT);
  larger->addAuditor(&b, SoNotRec::PARENT);
  larger->addAuditor(&c, SoNotRec::PARENT);
  larger->addAuditor(&target, SoNotRec::PARENT);
  larger->addAuditor(&target, SoNotRec::SENSOR);
  larger->removeAuditor(&target, SoNotRec::ENGINE); // absent type leaves both entries
  const SoAuditorList & before = larger->getAuditors();
  const bool absent_type_preserved = before.getLength() == 5 &&
    before.find(&target, SoNotRec::PARENT) >= 0 &&
    before.find(&target, SoNotRec::SENSOR) >= 0;
  larger->removeAuditor(&target, SoNotRec::SENSOR);
  const SoAuditorList & after = larger->getAuditors();
  const bool tree_type_preserved = after.getLength() == 4 &&
    after.find(&target, SoNotRec::PARENT) >= 0 &&
    after.find(&target, SoNotRec::SENSOR) == -1;
  larger->removeAuditor(&target, SoNotRec::PARENT);
  larger->removeAuditor(&a, SoNotRec::PARENT);
  larger->removeAuditor(&b, SoNotRec::PARENT);
  larger->removeAuditor(&c, SoNotRec::PARENT);
  larger->unref();

  if (!(ok && absent_type_preserved && tree_type_preserved)) {
    std::fprintf(stderr, "removeAuditor removed the wrong type\n");
    return 1;
  }
  return 0;
}
