// Public-API regression test for SoTempPath::replaceTail().

#include <cstdio>
#include <cstring>

#include <Inventor/SoDB.h>
#include <Inventor/misc/SoTempPath.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSeparator.h>

static int
valid_sentinel()
{
  SoTempPath path(1);
  path.simpleAppend(static_cast<SoNode *>(NULL), -1);
  path.replaceTail(static_cast<SoNode *>(NULL), -1);

  if (path.getLength() != 1 || path.getTail() != NULL ||
      path.getIndexFromTail(0) != -1) {
    std::fprintf(stderr,
                 "FAIL: replaceTail(NULL, -1) changed a valid sentinel tail\n");
    return 1;
  }
  return 0;
}

static int
valid_replacement()
{
  SoGroup * root = new SoGroup;
  root->ref();
  SoSeparator * first = new SoSeparator;
  SoSeparator * second = new SoSeparator;
  root->addChild(first);
  root->addChild(second);

  int result = 0;
  {
    SoTempPath path(2);
    path.setHead(root);
    path.simpleAppend(first, 0);
    path.replaceTail(second, 1);
    if (path.getLength() != 2 || path.getNode(0) != root ||
        path.getTail() != second || path.getIndexFromTail(0) != 1) {
      std::fprintf(stderr,
                   "FAIL: replaceTail() did not replace a valid non-empty tail\n");
      result = 1;
    }
  }
  root->unref();
  return result;
}

static int
empty_noop()
{
  SoTempPath path(1);
  path.replaceTail(static_cast<SoNode *>(NULL), -1);

  if (path.getLength() != 0 || path.getHead() != NULL) {
    std::fprintf(stderr, "FAIL: replaceTail() changed an empty path\n");
    return 1;
  }

  // A no-op must leave the object reusable; the next append detects damage
  // to the empty-path storage without relying only on a sanitizer diagnostic.
  path.simpleAppend(static_cast<SoNode *>(NULL), -1);
  if (path.getLength() != 1 || path.getTail() != NULL ||
      path.getIndexFromTail(0) != -1) {
    std::fprintf(stderr, "FAIL: empty-path replace left path unusable\n");
    return 1;
  }
  return 0;
}

int
main(int argc, char ** argv)
{
  if (argc != 2) {
    std::fprintf(stderr,
                 "usage: %s valid-sentinel|valid-replacement|empty-noop\n",
                 argv[0]);
    return 2;
  }
  SoDB::init();
  int result = 2;
  if (std::strcmp(argv[1], "valid-sentinel") == 0)
    result = valid_sentinel();
  else if (std::strcmp(argv[1], "valid-replacement") == 0)
    result = valid_replacement();
  else if (std::strcmp(argv[1], "empty-noop") == 0)
    result = empty_noop();
  else
    std::fprintf(stderr, "unknown case: %s\n", argv[1]);
  SoDB::finish();
  return result;
}
