#include <Inventor/SoDB.h>
#include <Inventor/SoPath.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSeparator.h>

#include <climits>
#include <cstdio>

static int failures;

static SoPath *
hold(SoPath * path)
{
  if (path != NULL) path->ref();
  return path;
}

static int
fullLength(const SoPath * path)
{
  return path->fullPath().getLength();
}

static void
check(const bool condition, const char * message)
{
  if (!condition) {
    std::fprintf(stderr, "FAIL: %s\n", message);
    ++failures;
  }
}

struct Fixture {
  Fixture()
    : root(new SoSeparator), branch(new SoSeparator), leaf(new SoCube),
      path(NULL)
  {
    this->root->ref();
    this->root->addChild(new SoCube);
    this->root->addChild(this->branch);
    this->branch->addChild(this->leaf);
    this->path = hold(new SoPath(this->root));
    this->path->append(this->branch);
    this->path->append(this->leaf);
  }

  ~Fixture()
  {
    this->path->unref();
    this->root->unref();
  }

  SoSeparator * root;
  SoSeparator * branch;
  SoCube * leaf;
  SoPath * path;
};

static void
checkTruncateAndPop()
{
  Fixture invalid;
  SoPath * before = hold(new SoPath(*invalid.path));
  invalid.path->truncate(-1);
  check(*invalid.path == *before, "negative truncate changed the path");
  invalid.path->truncate(4);
  check(*invalid.path == *before, "past-end truncate changed the path");
  before->unref();

  Fixture middle;
  middle.path->truncate(2);
  check(fullLength(middle.path) == 2 && middle.path->getTail() == middle.branch,
        "truncate did not preserve the requested prefix");

  Fixture current;
  before = hold(new SoPath(*current.path));
  current.path->truncate(3);
  check(*current.path == *before, "truncate at current length changed the path");
  before->unref();

  SoPath * empty = hold(new SoPath);
  empty->pop();
  check(fullLength(empty) == 0, "pop changed an empty path");
  empty->unref();

  Fixture popped;
  popped.path->pop();
  check(fullLength(popped.path) == 2 && popped.path->getTail() == popped.branch,
        "pop did not remove exactly one node");
}

static void
checkCopyRanges()
{
  Fixture fixture;

  SoPath * suffix = hold(fixture.path->copy(1));
  check(suffix != NULL && fullLength(suffix) == 2 &&
        suffix->getHead() == fixture.branch &&
        suffix->getTail() == fixture.leaf &&
        suffix->getIndex(0) == 1,
        "copy from a nonzero start did not preserve the suffix");
  if (suffix != NULL) suffix->unref();

  SoPath * prefix = hold(fixture.path->copy(0, 2));
  check(prefix != NULL && fullLength(prefix) == 2 &&
        prefix->getHead() == fixture.root && prefix->getTail() == fixture.branch,
        "bounded copy returned the wrong prefix");
  if (prefix != NULL) prefix->unref();

  SoPath * oversized = fixture.path->copy(1, INT_MAX);
  check(oversized == NULL, "oversized copy length was accepted");

  check(fixture.path->copy(-1) == NULL, "negative copy start was accepted");
  SoPath * end = hold(fixture.path->copy(3));
  check(end != NULL && fullLength(end) == 0 && end->getHead() == NULL,
        "copy at the end did not return an empty path");
  if (end != NULL) end->unref();

  check(fixture.path->copy(3, 1) == NULL,
        "nonempty copy at the end was accepted");
  check(fixture.path->copy(4) == NULL, "past-end copy start was accepted");
  check(fixture.path->copy(0, -1) == NULL, "negative copy length was accepted");

  SoPath * empty = hold(new SoPath);
  SoPath * emptycopy = hold(empty->copy());
  check(emptycopy != NULL && fullLength(emptycopy) == 0 &&
        emptycopy->getHead() == NULL,
        "copy from an empty path did not return an empty path");
  if (emptycopy != NULL) emptycopy->unref();

  SoEventCallback * callback = new SoEventCallback;
  callback->ref();
  callback->setPath(empty);
  const SoPath * filter = callback->getPath();
  check(filter != NULL && fullLength(filter) == 0 &&
        !fixture.path->containsPath(filter),
        "empty callback filter was lost or became unrestricted");
  callback->setPath(NULL);
  check(callback->getPath() == NULL, "NULL did not clear the callback filter");
  callback->unref();
  empty->unref();
}

int
main()
{
  SoDB::init();
  checkTruncateAndPop();
  checkCopyRanges();
  SoDB::finish();
  return failures == 0 ? 0 : 1;
}
