#include <Inventor/SoDB.h>
#include <Inventor/SoFullPath.h>
#include <Inventor/SoNodeKitPath.h>
#include <Inventor/SoPath.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/nodekits/SoShapeKit.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSphere.h>

#include <cstdio>

static int failures;

static SoPath *
hold(SoPath * path)
{
  path->ref();
  return path;
}

static void
check(const bool condition, const char * message)
{
  if (!condition) {
    std::fprintf(stderr, "FAIL: %s\n", message);
    ++failures;
  }
}

static void
checkUnchanged(SoPath * path, SoPath * before, const char * message)
{
  check(*path == *before, message);
  before->unref();
}

static void
checkInvalidAppends()
{
  SoPath * empty = hold(new SoPath);
  SoPath * before = hold(new SoPath(*empty));
  empty->append(0);
  checkUnchanged(empty, before, "integer append changed an empty path");
  empty->push(0);
  check(static_cast<SoFullPath *>(empty)->getLength() == 0,
        "integer push changed an empty path");
  empty->unref();

  SoCube * leaf = new SoCube;
  leaf->ref();
  SoPath * leafpath = hold(new SoPath(leaf));
  before = hold(new SoPath(*leafpath));
  leafpath->append(0);
  checkUnchanged(leafpath, before, "integer append changed a leaf path");

  SoPath * suffix = hold(new SoPath(new SoSeparator));
  before = hold(new SoPath(*leafpath));
  leafpath->append(suffix);
  checkUnchanged(leafpath, before, "path append changed a leaf path");
  suffix->unref();
  leafpath->unref();
  leaf->unref();

  SoSeparator * root = new SoSeparator;
  root->ref();
  SoCube * child = new SoCube;
  root->addChild(child);
  SoPath * path = hold(new SoPath(root));
  before = hold(new SoPath(*path));
  path->append(-1);
  checkUnchanged(path, before, "negative index changed the path");
  before = hold(new SoPath(*path));
  path->append(root->getNumChildren());
  checkUnchanged(path, before, "past-end index changed the path");

  SoSphere * outsider = new SoSphere;
  outsider->ref();
  before = hold(new SoPath(*path));
  path->append(outsider);
  checkUnchanged(path, before, "non-child node changed the path");
  outsider->unref();

  path->append(child);
  check(static_cast<SoFullPath *>(path)->getLength() == 2 &&
        path->getTail() == child,
        "valid child append did not extend the path");
  path->unref();
  root->unref();
}

static void
checkHiddenTail()
{
  SoShapeKit * kit = new SoShapeKit;
  kit->ref();
  check(kit->setPart("shape", new SoCube), "could not create shape part");
  SoNodeKitPath * kitpath = kit->createPathToPart("shape", TRUE);
  check(kitpath != NULL, "could not create nodekit path");
  if (kitpath != NULL) {
    SoPath * path = hold(static_cast<SoPath *>(kitpath));
    path->pop();
    const int visible = path->getLength();
    const int full = static_cast<SoFullPath *>(path)->getLength();
    path->append(kit->getPart("shape", FALSE));
    check(path->getLength() == visible &&
          static_cast<SoFullPath *>(path)->getLength() == full + 1,
          "valid append below a hidden tail was not preserved");
    path->unref();
  }
  kit->unref();
}

int
main()
{
  SoDB::init();
  SoNodeKit::init();
  checkInvalidAppends();
  checkHiddenTail();
  SoDB::finish();
  return failures == 0 ? 0 : 1;
}
