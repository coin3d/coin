// Contract probe for SoPath copy-assignment auditor ownership.

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <Inventor/SoDB.h>
#include <Inventor/SoPath.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSeparator.h>

static int
fail_without_cleanup(const char * message)
{
  std::fprintf(stderr, "FAIL: %s\n", message);
  std::fflush(stderr);
  std::_Exit(1);
}

static int
test_reassignment()
{
  SoSeparator * oldroot = new SoSeparator;
  oldroot->ref();
  SoGroup * oldchild = new SoGroup;
  oldroot->addChild(oldchild);

  SoSeparator * newroot = new SoSeparator;
  newroot->ref();
  SoGroup * newchild = new SoGroup;
  newroot->addChild(newchild);

  SoPath * destination = new SoPath(oldroot);
  destination->ref();
  destination->append(oldchild);
  SoPath * source = new SoPath(newroot);
  source->ref();
  source->append(newchild);

  *destination = *source;

  oldroot->insertChild(new SoGroup, 0);
  if (destination->getHead() != newroot ||
      destination->getNode(1) != newchild ||
      destination->getIndex(0) != 0 ||
      destination->getIndex(1) != 0) {
    return fail_without_cleanup(
      "the abandoned child list still changed the assigned path");
  }

  newroot->insertChild(new SoGroup, 0);
  if (destination->getIndex(1) != 1) {
    return fail_without_cleanup(
      "the assigned path did not audit its new child list exactly once");
  }

  destination->unref();
  source->unref();

  // These mutations also exercise the absence of dangling auditor pointers.
  oldroot->insertChild(new SoGroup, 0);
  newroot->insertChild(new SoGroup, 0);

  oldroot->unref();
  newroot->unref();
  return 0;
}

static int
test_self_assignment()
{
  SoSeparator * root = new SoSeparator;
  root->ref();
  SoGroup * selected = new SoGroup;
  root->addChild(selected);

  SoPath * path = new SoPath(root);
  path->ref();
  path->append(selected);

  *path = *path;
  root->insertChild(new SoGroup, 0);
  if (path->getLength() != 2 || path->getNode(1) != selected ||
      path->getIndex(1) != 1) {
    return fail_without_cleanup(
      "self-assignment duplicated or corrupted child-list auditing");
  }

  path->unref();

  // A duplicate registration would now call the destroyed path.
  root->insertChild(new SoGroup, 0);
  root->unref();
  return 0;
}

int
main(int argc, char ** argv)
{
  if (argc != 2) {
    std::fprintf(stderr, "usage: %s reassign|self-assign\n", argv[0]);
    return 2;
  }

  SoDB::init();
  int result = 2;
  if (std::strcmp(argv[1], "reassign") == 0)
    result = test_reassignment();
  else if (std::strcmp(argv[1], "self-assign") == 0)
    result = test_self_assignment();
  else
    std::fprintf(stderr, "unknown case: %s\n", argv[1]);
  SoDB::finish();
  return result;
}
