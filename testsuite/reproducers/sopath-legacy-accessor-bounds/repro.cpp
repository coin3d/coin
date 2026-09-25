// Process-isolated public-API regression test for SoPath accessor bounds.

#include <climits>
#include <cstdio>
#include <cstring>

#include <Inventor/SoDB.h>
#include <Inventor/SoFullPath.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoTempPath.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSphere.h>

namespace {

int diagnosticcount = 0;

void
countDiagnostic(const SoError *, void *)
{
  ++diagnosticcount;
}

struct Fixture {
  Fixture()
    : root(new SoSeparator), path(new SoTempPath(3)), middle(new SoSeparator),
      leaf(new SoCube)
  {
    this->root->ref();
    this->path->ref();

    this->root->addChild(new SoSphere);
    this->root->addChild(this->middle);
    this->middle->addChild(new SoSphere);
    this->middle->addChild(this->leaf);

    this->path->setHead(this->root);
    this->path->append(this->middle);
    this->path->append(this->leaf);
  }

  ~Fixture()
  {
    this->path->unref();
    this->root->unref();
  }

  SoSeparator * root;
  SoTempPath * path;
  SoSeparator * middle;
  SoCube * leaf;
};

int
fail(const char * test, const char * reason)
{
  std::fprintf(stderr, "[sopath-bounds] FAIL %s: %s\n", test, reason);
  return 1;
}

bool
unchanged(const Fixture & fixture)
{
  SoPath * path = fixture.path;
  SoFullPath * full = fixture.path;
  const bool result = path->getLength() == 3 && full->getLength() == 3 &&
    path->getNode(0) == fixture.root &&
    path->getNode(1) == fixture.middle &&
    path->getNode(2) == fixture.leaf &&
    path->getIndex(0) == 0 && path->getIndex(1) == 1 && path->getIndex(2) == 1;
  if (!result) {
    std::fprintf(stderr,
                 "[sopath-bounds] state: visible=%d full=%d nodes=%p,%p,%p "
                 "expected=%p,%p,%p indices=%d,%d,%d\n",
                 path->getLength(), full->getLength(),
                 static_cast<void *>(path->getNode(0)),
                 static_cast<void *>(path->getNode(1)),
                 static_cast<void *>(path->getNode(2)),
                 static_cast<void *>(fixture.root),
                 static_cast<void *>(fixture.middle),
                 static_cast<void *>(fixture.leaf), path->getIndex(0),
                 path->getIndex(1), path->getIndex(2));
  }
  return result;
}

int
boundaryIndex(const char * boundary, int limit)
{
  if (std::strcmp(boundary, "neg-one") == 0) return -1;
  if (std::strcmp(boundary, "neg-min") == 0) return INT_MIN;
  if (std::strcmp(boundary, "limit") == 0) return limit;
  if (std::strcmp(boundary, "above") == 0) return limit + 1;
  if (std::strcmp(boundary, "max") == 0) return INT_MAX;
  return 0;
}

int
runInvalid(const char * test, const char * accessor, const char * boundary)
{
  Fixture fixture;
  SoPath * path = fixture.path;
  const int index = boundaryIndex(boundary, 3);

  SoErrorCB * oldhandler = SoDebugError::getHandlerCallback();
  void * olddata = SoDebugError::getHandlerData();
  diagnosticcount = 0;
  SoDebugError::setHandlerCallback(countDiagnostic, NULL);

  bool sentinel = false;
  if (std::strcmp(accessor, "node") == 0) {
    sentinel = path->getNode(index) == NULL;
  }
  else if (std::strcmp(accessor, "node-tail") == 0) {
    sentinel = path->getNodeFromTail(index) == NULL;
  }
  else if (std::strcmp(accessor, "index") == 0) {
    sentinel = path->getIndex(index) == -1;
  }
  else if (std::strcmp(accessor, "index-tail") == 0) {
    sentinel = path->getIndexFromTail(index) == -1;
  }
  else {
    SoDebugError::setHandlerCallback(oldhandler, olddata);
    return fail(test, "unknown accessor in test registration");
  }

  const int observeddiagnostics = diagnosticcount;
  SoDebugError::setHandlerCallback(oldhandler, olddata);

  if (!sentinel) return fail(test, "invalid access did not return its sentinel");
  if (!unchanged(fixture)) return fail(test, "invalid access mutated the path");
#if !COIN_DEBUG
  if (observeddiagnostics != 0) {
    return fail(test, "invalid access emitted a Release diagnostic");
  }
#else
  (void) observeddiagnostics;
#endif
  return 0;
}

int
runEmpty(const char * test, const char * accessor)
{
  SoTempPath * storage = new SoTempPath(0);
  storage->ref();
  SoPath * path = storage;

  SoErrorCB * oldhandler = SoDebugError::getHandlerCallback();
  void * olddata = SoDebugError::getHandlerData();
  diagnosticcount = 0;
  SoDebugError::setHandlerCallback(countDiagnostic, NULL);

  bool sentinel = false;
  if (std::strcmp(accessor, "node") == 0) sentinel = path->getNode(0) == NULL;
  else if (std::strcmp(accessor, "node-tail") == 0) {
    sentinel = path->getNodeFromTail(0) == NULL;
  }
  else if (std::strcmp(accessor, "index") == 0) {
    sentinel = path->getIndex(0) == -1;
  }
  else if (std::strcmp(accessor, "index-tail") == 0) {
    sentinel = path->getIndexFromTail(0) == -1;
  }

  const int observeddiagnostics = diagnosticcount;
  SoDebugError::setHandlerCallback(oldhandler, olddata);
  const bool stillEmpty = path->getLength() == 0 && storage->getLength() == 0;
  storage->unref();

  if (!sentinel) return fail(test, "empty-path access did not return its sentinel");
  if (!stillEmpty) return fail(test, "empty-path access mutated the path");
#if !COIN_DEBUG
  if (observeddiagnostics != 0) {
    return fail(test, "empty-path access emitted a Release diagnostic");
  }
#else
  (void) observeddiagnostics;
#endif
  return 0;
}

int
runAccessor(const char * accessor)
{
  const char * boundaries[] = { "neg-one", "neg-min", "limit", "above", "max" };
  int result = 0;
  for (size_t i = 0; i < sizeof(boundaries) / sizeof(boundaries[0]); ++i) {
    if (runInvalid(accessor, accessor, boundaries[i]) != 0) result = 1;
  }
  if (runEmpty(accessor, accessor) != 0) result = 1;
  return result;
}

int
runValid(const char * test)
{
  Fixture fixture;
  SoPath * path = fixture.path;

  if (!unchanged(fixture)) return fail(test, "direct valid access changed");
  if (path->getNodeFromTail(0) != fixture.leaf ||
      path->getNodeFromTail(1) != fixture.middle ||
      path->getNodeFromTail(2) != fixture.root) {
    return fail(test, "valid node tail order changed");
  }
  if (path->getIndexFromTail(0) != 1 ||
      path->getIndexFromTail(1) != 1 ||
      path->getIndexFromTail(2) != 0) {
    return fail(test, "valid child-index tail order changed");
  }
  return 0;
}

int
runVisibility(const char * test)
{
  SoSeparator * root = new SoSeparator;
  root->ref();
  SoBaseKit * kit = new SoBaseKit;
  root->addChild(kit);
  SoNode * hidden = kit->getPart("callbackList", TRUE);
  if (hidden == NULL) {
    root->unref();
    return fail(test, "could not construct the public nodekit fixture");
  }

  SoTempPath * storage = new SoTempPath(3);
  storage->ref();
  storage->setHead(root);
  storage->append(kit);
  storage->append(hidden);
  SoPath * visible = storage;
  SoFullPath * full = storage;

  if (visible->getLength() != 2 || full->getLength() != 3) {
    storage->unref();
    root->unref();
    return fail(test, "visible/full lengths are not distinct");
  }
  if (visible->getNode(2) != hidden || visible->getIndex(2) < 0 ||
      visible->getNodeFromTail(0) != kit ||
      full->getNodeFromTail(0) != hidden) {
    storage->unref();
    root->unref();
    return fail(test, "valid visible/full accessor semantics changed");
  }

  const int fullhiddenindex = visible->getIndex(2);
  SoErrorCB * oldhandler = SoDebugError::getHandlerCallback();
  void * olddata = SoDebugError::getHandlerData();
  diagnosticcount = 0;
  SoDebugError::setHandlerCallback(countDiagnostic, NULL);
  const bool nodesentinel = visible->getNodeFromTail(2) == NULL;
  const bool indexsentinel = visible->getIndexFromTail(2) == -1;
  const int observeddiagnostics = diagnosticcount;
  SoDebugError::setHandlerCallback(oldhandler, olddata);

  const bool stillDistinct = visible->getLength() == 2 && full->getLength() == 3 &&
    visible->getNode(2) == hidden && visible->getIndex(2) == fullhiddenindex &&
    visible->getNodeFromTail(0) == kit && full->getNodeFromTail(0) == hidden;
  storage->unref();
  root->unref();

  if (!nodesentinel || !indexsentinel) {
    return fail(test, "visible tail limit did not return its sentinel");
  }
  if (!stillDistinct) return fail(test, "visible-limit access mutated the path");
#if !COIN_DEBUG
  if (observeddiagnostics != 0) {
    return fail(test, "visible-limit access emitted a Release diagnostic");
  }
#else
  (void) observeddiagnostics;
#endif
  return 0;
}

} // namespace

int
main(int argc, char ** argv)
{
  SoInteraction::init();
  if (argc < 2) return fail("arguments", "missing case name");

  const char * test = argv[1];
  int result = 1;
  if (std::strcmp(test, "valid") == 0) result = runValid(test);
  else if (std::strcmp(test, "visibility") == 0) result = runVisibility(test);
  else if (argc == 3 && std::strcmp(argv[2], "all") == 0) {
    result = runAccessor(argv[1]);
  }
  else if (argc == 3 && std::strcmp(argv[2], "empty") == 0) {
    result = runEmpty(test, argv[1]);
  }
  else if (argc == 3) result = runInvalid(test, argv[1], argv[2]);
  else result = fail(test, "invalid test arguments");

  SoDB::finish();
  return result;
}
