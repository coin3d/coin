#include <Inventor/SoDB.h>
#include <Inventor/SoFullPath.h>
#include <Inventor/SoNodeKitPath.h>
#include <Inventor/SoPath.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/nodekits/SoSeparatorKit.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>

#include <cstdio>

class FixtureKit : public SoBaseKit {
public:
  FixtureKit() : SoBaseKit() {}
  ~FixtureKit() override {}
  void addFixtureChild(SoNode * node) { this->children->append(node); }
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
  return static_cast<const SoFullPath *>(path)->getLength();
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
          static_cast<SoFullPath *>(static_cast<SoPath *>(path))->getTail() == transform,
          "factory path lost its projected or full tail");
    path->unref();
  }
  kit->unref();
}

int
main()
{
  SoDB::init();
  SoNodeKit::init();
  checkProjectionAndOwnership();
  checkAppendBelowLogicalTail();
  checkFactoryType();
  SoDB::finish();
  return failures == 0 ? 0 : 1;
}
