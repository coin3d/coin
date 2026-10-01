#include <Inventor/SoDB.h>
#include <Inventor/SoNodeKitPath.h>
#include <Inventor/SoPath.h>
#include <Inventor/misc/SoTempPath.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/nodekits/SoSeparatorKit.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>

#include <cstdio>
#include <new>

class FixtureKit : public SoBaseKit {
public:
  FixtureKit() : SoBaseKit() {}
  ~FixtureKit() override {}
  void addFixtureChild(SoNode * node) { this->children->append(node); }
};

class ThrowingGroup : public SoSeparator {
public:
  ThrowingGroup() : failNextChildren(false) {}
  ~ThrowingGroup() override {}
  SoChildList * getChildren() const override {
    if (this->failNextChildren) {
      this->failNextChildren = false;
      throw std::bad_alloc();
    }
    return SoSeparator::getChildren();
  }
  mutable bool failNextChildren;
};

class ThrowingSearchKit : public FixtureKit {
public:
  ThrowingSearchKit() : failNextSearch(false) {}
  void search(SoSearchAction * action) override {
    if (this->failNextSearch) {
      this->failNextSearch = false;
      throw std::bad_alloc();
    }
    FixtureKit::search(action);
  }
  bool failNextSearch;
};

class TestNodeKitPath : public SoNodeKitPath {
public:
  TestNodeKitPath() : SoNodeKitPath(8) {}
  ~TestNodeKitPath() override {}
  void setFixtureHead(SoNode * node) { SoPath::setHead(node); }
};

static int failures;

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

struct Route {
  Route()
    : head(new SoSeparator), rootkit(new FixtureKit),
      hidden(new SoSeparator), childkit(new FixtureKit),
      leaf(new SoTransform), source(NULL)
  {
    this->head->ref();
    this->head->addChild(this->rootkit);
    this->rootkit->addFixtureChild(this->hidden);
    this->hidden->addChild(this->childkit);
    this->childkit->addFixtureChild(this->leaf);

    this->source = new SoPath(this->head);
    this->source->ref();
    for (int i = 0; i < 4; ++i) this->source->append(0);
  }

  ~Route()
  {
    this->source->unref();
    this->head->unref();
  }

  SoSeparator * head;
  FixtureKit * rootkit;
  SoSeparator * hidden;
  FixtureKit * childkit;
  SoTransform * leaf;
  SoPath * source;
};

static void
checkProjectionAndOwnership()
{
  check(SoNodeKitPath::fromPath(NULL) == NULL, "fromPath(NULL) was accepted");
  Route route;
  SoNodeKitPath * projected = SoNodeKitPath::fromPath(route.source);
  check(projected != NULL, "fromPath did not create a path");
  if (projected == NULL) return;

  check(projected->getRefCount() == 0, "new path did not start unreferenced");
  projected->ref();
  check(dynamic_cast<SoNodeKitPath *>(static_cast<SoPath *>(projected)) == projected,
        "result is not a genuine SoNodeKitPath object");
  check(fullLength(projected) == 5, "full route was not preserved");
  check(projected->getLength() == 3 &&
        projected->getNode(0) == route.head &&
        projected->getNode(1) == route.rootkit &&
        projected->getNode(2) == route.childkit,
        "nodekit projection is incorrect");
  check(projected->getNode(-1) == NULL && projected->getNode(3) == NULL,
        "nodekit accessor accepted an invalid index");
  check(projected->getNodeFromTail(0) == route.childkit &&
        projected->getNodeFromTail(2) == route.head,
        "reverse nodekit projection is incorrect");

  projected->truncate(projected->getLength());
  check(fullLength(projected) == 5, "truncate at current length changed the route");
  projected->pop();
  check(projected->getLength() == 2 && projected->getTail() == route.rootkit,
        "pop did not remove one projected node");
  check(fullLength(route.source) == 5, "mutating the copy changed the source");
  projected->unref();
}

static void
checkAppendBelowLogicalTail()
{
  FixtureKit * root = new FixtureKit;
  SoSeparator * hidden = new SoSeparator;
  FixtureKit * child = new FixtureKit;
  root->ref();
  root->addFixtureChild(hidden);
  hidden->addChild(child);

  TestNodeKitPath path;
  path.setFixtureHead(root);
  path.append(child);
  check(path.getLength() == 2 && path.getTail() == child &&
        fullLength(&path) == 3,
        "append did not retain the hidden route to the child kit");
  root->unref();
}

static void
checkAppendPathAndPop()
{
  Route route;
  TestNodeKitPath destination;
  destination.setFixtureHead(route.rootkit);

  SoPath * suffix = new SoPath(route.childkit);
  suffix->ref();
  suffix->append(route.leaf);
  SoNodeKitPath * source = SoNodeKitPath::fromPath(suffix);
  source->ref();
  destination.append(source);
  check(destination.getLength() == 2 &&
        destination.getTail() == route.childkit &&
        fullLength(&destination) == 4 &&
        destination.fullPath().getTail() == route.leaf,
        "appending a nodekit path lost its hidden bridge or suffix");

  destination.pop();
  check(destination.getLength() == 1 &&
        destination.getTail() == route.rootkit &&
        fullLength(&destination) == 2,
        "pop did not truncate at the last projected nodekit");
  destination.append(route.childkit);
  check(destination.getLength() == 2 &&
        destination.getTail() == route.childkit &&
        fullLength(&destination) == 3 &&
        fullLength(source) == 2,
        "appending a child kit after pop changed the route or source");

  source->unref();
  suffix->unref();
}

static void
checkFactoryType()
{
  SoSeparatorKit * kit = new SoSeparatorKit;
  kit->ref();
  SoNode * transform = kit->getPart("transform", TRUE);
  SoNodeKitPath * path = kit->createPathToPart("transform", TRUE);
  check(path != NULL, "factory did not create a path");
  if (path != NULL) {
    check(dynamic_cast<SoNodeKitPath *>(static_cast<SoPath *>(path)) == path,
          "factory returned a fake derived pointer");
    path->ref();
    check(path->getLength() == 1 && path->getTail() == kit &&
          static_cast<SoPath *>(path)->fullPath().getTail() == transform,
          "factory path lost its projected or full tail");
    path->unref();
  }
  kit->unref();
}

static void
checkFactoryTypeWithExtension()
{
  SoSeparator * root = new SoSeparator;
  SoSeparatorKit * kit = new SoSeparatorKit;
  root->ref();
  root->addChild(kit);

  SoPath * prefix = new SoPath(root);
  prefix->ref();
  prefix->append(kit);

  SoNode * transform = kit->getPart("transform", TRUE);
  SoNodeKitPath * path = kit->createPathToPart("transform", TRUE, prefix);
  check(path != NULL, "factory did not extend the supplied path");
  if (path != NULL) {
    check(dynamic_cast<SoNodeKitPath *>(static_cast<SoPath *>(path)) == path,
          "extended factory path is not a genuine SoNodeKitPath object");
    path->ref();
    const SoFullPathView complete = static_cast<SoPath *>(path)->fullPath();
    check(path->getLength() == 2 &&
          path->getNode(0) == root && path->getTail() == kit &&
          complete.getLength() >= 3 &&
          complete.getNodeFromTail(complete.getLength() - 1) == root &&
          complete.getTail() == transform,
          "extended factory path lost its projected or complete route");
    path->unref();
  }

  prefix->unref();
  root->unref();
}

static void
checkMaterializationFailure()
{
  SoSeparator * head = new SoSeparator;
  ThrowingGroup * child = new ThrowingGroup;
  head->ref();
  head->addChild(child);
  SoPath * source = new SoPath(head);
  source->ref();
  source->append(child);
  const int before = head->getRefCount();
  child->failNextChildren = true;
  bool caught = false;
  try {
    SoNodeKitPath * projected = SoNodeKitPath::fromPath(source);
    projected->ref();
    projected->unref();
  }
  catch (const std::bad_alloc &) { caught = true; }
  check(caught && head->getRefCount() == before,
        "fromPath leaked a partial copy after child lookup failed");
  source->unref();
  head->unref();
}

static void
checkHeadIndexPreserved()
{
  SoSeparator * root = new SoSeparator;
  root->ref();
  root->addChild(new SoSeparator);
  SoSeparatorKit * kit = new SoSeparatorKit;
  root->addChild(kit);

  SoPath * full = new SoPath(root);
  full->ref();
  full->append(1);
  SoPath * suffix = full->copy(1);
  suffix->ref();
  SoNodeKitPath * projected = SoNodeKitPath::fromPath(suffix);
  projected->ref();
  check(suffix->getIndex(0) == 1 &&
        static_cast<SoPath *>(projected)->getIndex(0) == 1 &&
        projected->getHead() == kit,
        "fromPath changed the head index of a copied suffix");

  projected->unref();
  suffix->unref();
  full->unref();
  root->unref();
}

static void
checkTemporarySentinel()
{
  SoSeparator * head = new SoSeparator;
  head->ref();
  SoTempPath source(2);
  source.simpleAppend(head, -1);
  source.simpleAppend(static_cast<SoNode *>(NULL), -1);

  SoNodeKitPath * projected = SoNodeKitPath::fromPath(&source);
  projected->ref();
  check(projected->getLength() == 1 && projected->getTail() == head &&
        projected->getNode(0) == head &&
        projected->getNodeFromTail(0) == head,
        "nodekit projection dereferenced a temporary null sentinel");
  projected->unref();
  head->unref();
}

static void
checkCopiedPathTracksTreeEdits()
{
  Route route;
  SoNodeKitPath * projected = SoNodeKitPath::fromPath(route.source);
  projected->ref();
  route.hidden->removeChild(0);
  check(fullLength(projected) == 3 &&
        projected->getLength() == 2 &&
        projected->getTail() == route.rootkit &&
        fullLength(route.source) == 3,
        "copied path did not track a removed hidden child");

  route.head->removeChild(0);
  check(fullLength(projected) == 1 &&
        projected->getLength() == 1 &&
        projected->getTail() == route.head &&
        fullLength(route.source) == 1,
        "copied path retained stale nodes after a second tree edit");
  projected->unref();
}

static void
checkSearchFailureRestoresGlobalState()
{
  ThrowingSearchKit * root = new ThrowingSearchKit;
  FixtureKit * child = new FixtureKit;
  root->ref();
  root->addFixtureChild(child);
  TestNodeKitPath destination;
  destination.setFixtureHead(root);
  const SbBool before = SoBaseKit::isSearchingChildren();

  root->failNextSearch = true;
  bool caught = false;
  try { destination.append(child); }
  catch (const std::bad_alloc &) { caught = true; }
  check(caught && SoBaseKit::isSearchingChildren() == before &&
        fullLength(&destination) == 1,
        "append(childKit) left global search state changed after an exception");

  SoPath * sourcepath = new SoPath(child);
  sourcepath->ref();
  SoNodeKitPath * source = SoNodeKitPath::fromPath(sourcepath);
  source->ref();
  root->failNextSearch = true;
  caught = false;
  try { destination.append(source); }
  catch (const std::bad_alloc &) { caught = true; }
  check(caught && SoBaseKit::isSearchingChildren() == before &&
        fullLength(&destination) == 1,
        "append(path) left global search state changed after an exception");

  destination.append(child);
  check(destination.getTail() == child && fullLength(&destination) == 2 &&
        SoBaseKit::isSearchingChildren() == before,
        "a failed search prevented a later successful append");

  source->unref();
  sourcepath->unref();
  root->unref();
}

int
main()
{
  SoDB::init();
  SoNodeKit::init();
  checkProjectionAndOwnership();
  checkAppendBelowLogicalTail();
  checkAppendPathAndPop();
  checkFactoryType();
  checkFactoryTypeWithExtension();
  checkTemporarySentinel();
  checkHeadIndexPreserved();
  checkMaterializationFailure();
  checkSearchFailureRestoresGlobalState();
  checkCopiedPathTracksTreeEdits();
  SoDB::finish();
  return failures == 0 ? 0 : 1;
}
