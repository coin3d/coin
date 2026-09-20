#include "../CoinCleanup.h"

#include <Inventor/SoDB.h>
#include <Inventor/SoNodeKitPath.h>
#include <Inventor/SoPath.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/misc/SoTempPath.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/nodekits/SoSeparatorKit.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>

#include <cstdio>
#include <cstring>
#include <type_traits>

static_assert(std::is_base_of<SoPath, SoNodeKitPath>::value,
              "Coin 4 keeps public SoPath inheritance");
static_assert(std::is_convertible<SoNodeKitPath *, SoPath *>::value,
              "Coin 4 keeps SoPath inheritance public");

class FixtureKit : public SoBaseKit {
public:
  FixtureKit() : SoBaseKit() {}
  ~FixtureKit() override {}

  void addFixtureChild(SoNode * node) { this->children->append(node); }
};

class GenuineNodeKitPath : public SoNodeKitPath {
public:
  GenuineNodeKitPath() : SoNodeKitPath(8) {}
  ~GenuineNodeKitPath() override {}

  void setFixtureHead(SoNode * node) { SoPath::setHead(node); }
  void appendFixtureIndex(int index) { SoPath::append(index); }
  void copyFullRoute(const SoPath * path) { SoPath::append(path); }
};

class FactorySeparatorKit : public SoSeparatorKit {
public:
  FactorySeparatorKit() : SoSeparatorKit() {}
  ~FactorySeparatorKit() override {}

  SoNodeKitPath * createAnyPublic(const SbName & name, SbBool make)
  {
    return this->createPathToAnyPart(name, make, TRUE, TRUE, NULL);
  }
};

static int failures;

static int
fullLength(const SoPath & path)
{
  SoTempPath copy(8);
  copy.append(&path);
  return copy.getLength();
}

static SoNode *
fullTail(const SoPath & path)
{
  SoTempPath copy(8);
  copy.append(&path);
  return copy.getTail();
}

static void
expect(bool condition, const char * message)
{
  if (!condition) {
    std::fprintf(stderr, "[FAIL] %s\n", message);
    ++failures;
  }
}

static void
expectSameFullRoute(const SoPath & lhs, const SoPath & rhs, const char * message)
{
  const int length = fullLength(lhs);
  if (length != fullLength(rhs)) {
    expect(false, message);
    return;
  }
  for (int i = 0; i < length; ++i) {
    if (lhs.getNode(i) != rhs.getNode(i) || lhs.getIndex(i) != rhs.getIndex(i)) {
      expect(false, message);
      return;
    }
  }
}

struct Fixture {
  FixtureKit * root;
  FixtureKit * shared;
  FixtureKit * unrelated;
  SoSeparator * branch0;
  SoSeparator * branch1;
  SoSeparator * unrelatedBranch;
  SoTransform * leaf;

  Fixture()
    : root(new FixtureKit), shared(new FixtureKit), unrelated(new FixtureKit),
      branch0(new SoSeparator), branch1(new SoSeparator),
      unrelatedBranch(new SoSeparator), leaf(new SoTransform)
  {
    this->root->ref();
    this->unrelated->ref();

    this->branch0->addChild(this->shared);
    this->branch1->addChild(this->shared);
    this->root->addFixtureChild(this->branch0);
    this->root->addFixtureChild(this->branch1);
    this->root->addFixtureChild(this->leaf);

    this->unrelatedBranch->addChild(this->shared);
    this->unrelated->addFixtureChild(this->unrelatedBranch);
  }

  ~Fixture()
  {
    this->unrelated->unref();
    this->root->unref();
  }
};

static void
buildRoute(GenuineNodeKitPath & path, FixtureKit * head, int branch)
{
  path.setFixtureHead(head);
  path.appendFixtureIndex(branch);
  path.appendFixtureIndex(0);
}

static void
caseViewsAndBounds()
{
  expect(sizeof(SoNodeKitPath) == sizeof(SoPath),
         "Coin 4 preserves SoNodeKitPath object layout");
  Fixture f;
  SoSeparator * nonkithead = new SoSeparator;
  nonkithead->ref();
  nonkithead->addChild(f.branch0);

  GenuineNodeKitPath path;
  path.setFixtureHead(nonkithead);
  path.appendFixtureIndex(0);
  path.appendFixtureIndex(0);

  expect(fullLength(path) == 3, "F contains head, intermediate group, and kit");
  expect(path.SoPath::getLength() == 3, "V includes the first hidden node");
  expect(path.getLength() == 2, "K includes the non-kit head and later kit");
  expect(path.getNode(0) == nonkithead, "K[0] is always the head");
  expect(path.getNode(1) == f.shared, "K filters later non-kit nodes");
  expect(path.getNodeFromTail(0) == f.shared, "K tail is the later kit");
  expect(path.getNodeFromTail(1) == nonkithead, "reverse K includes head");
  expect(path.getNode(-1) == NULL, "negative nodekit index is safe");
  expect(path.getNode(2) == NULL, "past-end nodekit index is safe");
  expect(path.getNodeFromTail(-1) == NULL, "negative reverse index is safe");
  expect(path.getNodeFromTail(2) == NULL, "past-end reverse index is safe");

  GenuineNodeKitPath empty;
  expect(empty.getLength() == 0, "empty K has length zero");
  expect(empty.getTail() == NULL, "empty K has NULL tail");

  nonkithead->unref();
}

static void
caseEqualityAndFork()
{
  Fixture f;
  GenuineNodeKitPath left;
  GenuineNodeKitPath right;
  buildRoute(left, f.root, 0);
  buildRoute(right, f.root, 1);

  expect(left == right, "nodekit equality ignores intermediate route differences");
  expect(!(left != right), "nodekit inequality complements equality");
  expect(left.findFork(&right) == 1,
         "findFork follows equal K despite different intermediate indices");

  GenuineNodeKitPath otherhead;
  buildRoute(otherhead, f.unrelated, 0);
  expect(!(left == otherhead), "equality includes a different head");
  expect(left != otherhead, "inequality detects a different head");
  expect(left.findFork(&otherhead) == -1, "findFork rejects different heads");

  GenuineNodeKitPath prefix;
  prefix.setFixtureHead(f.root);
  expect(!(left == prefix), "equality detects a different K length");
  expect(left != prefix, "inequality detects a different K length");

  GenuineNodeKitPath empty;
  expect(left.findFork(&empty) == -1, "findFork with empty right path is -1");
  expect(empty.findFork(&left) == -1, "findFork with empty left path is -1");
  expect(empty.findFork(&empty) == -1, "findFork of two empty paths is -1");

  FixtureKit * alternate = new FixtureKit;
  SoSeparator * alternatebranch = new SoSeparator;
  alternatebranch->addChild(alternate);
  f.root->addFixtureChild(alternatebranch);
  GenuineNodeKitPath differenttail;
  buildRoute(differenttail, f.root, 3);
  expect(!(left == differenttail), "equality compares later K members");
  expect(left != differenttail, "inequality detects a different K tail");
  expect(left.findFork(&differenttail) == 0,
         "findFork stops after common head when later K differs");
}

static void
caseTruncateCurrent()
{
  Fixture f;
  GenuineNodeKitPath path;
  buildRoute(path, f.root, 0);
  SoPath * before = path.copy();
  before->ref();

  const int current = path.getLength();
  path.truncate(current);
  expectSameFullRoute(path, *before, "truncate(M) is an exact no-op");

  before->unref();
}

static void
caseTruncateAndPop()
{
  Fixture f;

  GenuineNodeKitPath one;
  buildRoute(one, f.root, 0);
  one.truncate(1);
  expect(one.getLength() == 1, "truncate(interior) keeps requested K prefix");
  expect(one.getTail() == f.root, "truncate(interior) keeps projected head");

  GenuineNodeKitPath zero;
  buildRoute(zero, f.root, 0);
  zero.truncate(0);
  expect(zero.getLength() == 0, "truncate(0) empties K");
  expect(zero.getTail() == NULL, "truncate(0) leaves NULL logical tail");

  GenuineNodeKitPath popped;
  buildRoute(popped, f.root, 0);
  popped.pop();
  expect(popped.getLength() == 1, "pop removes exactly the last projected node");
  expect(popped.getTail() == f.root, "pop exposes the prior projected node");
  popped.pop();
  expect(popped.getLength() == 0, "second pop removes projected head");
  popped.pop();
  expect(popped.getLength() == 0, "pop on empty path is a no-op");
}

static void
caseContains()
{
  Fixture f;
  GenuineNodeKitPath path;
  buildRoute(path, f.root, 0);
  expect(path.containsNode(f.root), "containsNode includes kit head");
  expect(path.containsNode(f.shared), "containsNode includes later projected kit");
  expect(!path.containsNode(f.unrelated), "containsNode rejects absent kit");
}

static void
caseAppendBelowNonKitTail()
{
  Fixture f;
  GenuineNodeKitPath path;
  path.setFixtureHead(f.root);
  path.appendFixtureIndex(2); // non-kit full tail below logical root kit

  expect(fullTail(path) == f.leaf, "control: full tail starts below logical tail");
  expect(path.getTail() == f.root, "control: logical tail is root kit");

  path.append(f.shared);
  expect(path.getLength() == 2, "append adds child below logical nodekit tail");
  expect(path.getTail() == f.shared, "append reaches requested child kit");
  expect(fullTail(path) == f.shared, "append replaces obsolete non-kit suffix");
  expect(fullLength(path) == 3, "append keeps the discovered full route");
  expect(path.SoPath::getIndex(1) == 0, "append uses first matching route");
}

static void
caseAppendPathPreservesRoute()
{
  Fixture f;
  GenuineNodeKitPath source;
  buildRoute(source, f.root, 1);
  GenuineNodeKitPath destination;

  destination.append(&source);
  expectSameFullRoute(destination, source,
                      "append(path) preserves source full nodes and indices");

  FixtureKit * joinhead = new FixtureKit;
  SoSeparator * joinbranch0 = new SoSeparator;
  SoSeparator * joinbranch1 = new SoSeparator;
  FixtureKit * jointail = new FixtureKit;
  joinbranch0->addChild(jointail);
  joinbranch1->addChild(jointail);
  joinhead->addFixtureChild(joinbranch0);
  joinhead->addFixtureChild(joinbranch1);
  f.root->addFixtureChild(joinhead);

  GenuineNodeKitPath nestedsource;
  buildRoute(nestedsource, joinhead, 1);
  GenuineNodeKitPath nonempty;
  nonempty.setFixtureHead(f.root);
  nonempty.append(&nestedsource);

  GenuineNodeKitPath expected;
  expected.setFixtureHead(f.root);
  expected.appendFixtureIndex(3);
  expected.appendFixtureIndex(1);
  expected.appendFixtureIndex(0);
  expectSameFullRoute(nonempty, expected,
                      "non-empty append(path) preserves source route indices");
}

static void
caseAppendPathFailureAtomic()
{
  Fixture f;
  GenuineNodeKitPath destination;
  destination.setFixtureHead(f.root);
  SoPath * before = destination.copy();
  before->ref();

  GenuineNodeKitPath source;
  buildRoute(source, f.unrelated, 0);
  destination.append(&source);

  expectSameFullRoute(destination, *before,
                      "failed append(path) leaves no partially appended kit");
  before->unref();
}

static void
checkFactoryResult(FactorySeparatorKit * kit, SoNode * transform,
                   SoNodeKitPath * path, const char * kind)
{
  expect(path != NULL, kind);
  if (path) {
    // This conversion and the member calls are intentionally ordinary public
    // uses.  UBSan/vptr must reject a fake derived pointer rather than letting
    // the test count ABI coincidence as success.
    SoPath * base = path;
    expect(dynamic_cast<SoNodeKitPath *>(base) == path,
           "factory result has genuine SoNodeKitPath dynamic type");
    expect(path->getRefCount() == 0, "factory result starts with refcount zero");
    path->ref();
    expect(path->getLength() == 1, "factory K contains its kit head");
    expect(path->getTail() == kit, "factory logical tail is the kit head");
    expect(fullTail(*path) == transform, "factory preserves hidden full tail");
    path->unref();
  }
}

static void
caseFactory(bool anypart)
{
  FactorySeparatorKit * kit = new FactorySeparatorKit;
  kit->ref();
  SoNode * transform = kit->getPart("transform", TRUE);
  SoNodeKitPath * path = anypart
    ? kit->createAnyPublic("transform", TRUE)
    : kit->createPathToPart("transform", TRUE);
  checkFactoryResult(kit, transform, path,
                     anypart ? "createPathToAnyPart returns a path"
                             : "createPathToPart returns a path");
  kit->unref();
}

static bool
selected(const char * requested, const char * name)
{
  return std::strcmp(requested, "all") == 0 || std::strcmp(requested, name) == 0;
}

int
main(int argc, char ** argv)
{
  const char * requested = argc > 1 ? argv[1] : "all";
  SoDB::init();
  CoinReproducerCleanup cleanup;
  SoNodeKit::init();

  if (selected(requested, "views")) caseViewsAndBounds();
  if (selected(requested, "equality")) caseEqualityAndFork();
  if (selected(requested, "truncate")) caseTruncateCurrent();
  if (selected(requested, "truncate-pop")) caseTruncateAndPop();
  if (selected(requested, "contains")) caseContains();
  if (selected(requested, "append-child")) caseAppendBelowNonKitTail();
  if (selected(requested, "append-route")) caseAppendPathPreservesRoute();
  if (selected(requested, "append-atomic")) caseAppendPathFailureAtomic();
  if (selected(requested, "factory-part")) caseFactory(false);
  if (selected(requested, "factory-any")) caseFactory(true);

  if (failures) {
    std::fprintf(stderr, "[FAIL] %d behavior(s)\n", failures);
    return 1;
  }
  std::fprintf(stderr, "[PASS] selected behaviors\n");
  return 0;
}
