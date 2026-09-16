#include "../CoinCleanup.h"

#include <Inventor/SoDB.h>
#include <Inventor/SoNodeKitPath.h>
#include <Inventor/SoPath.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/misc/SoTempPath.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>

#include <cstdio>

class FromPathFixtureKit : public SoBaseKit {
public:
  FromPathFixtureKit() : SoBaseKit() {}
  ~FromPathFixtureKit() override {}
  void addFixtureChild(SoNode * node) { this->children->append(node); }
};

static int failures;

static int
fullLength(const SoPath & path)
{
  SoTempPath copy(8);
  copy.append(&path);
  return copy.getLength();
}

static void
expect(bool condition, const char * message)
{
  if (!condition) {
    std::fprintf(stderr, "[VIOLATED] %s\n", message);
    ++failures;
  }
}

int
main()
{
  SoDB::init();
  CoinReproducerCleanup cleanup;
  SoNodeKit::init();

  expect(SoNodeKitPath::fromPath(NULL) == NULL, "fromPath(NULL) returns NULL");

  SoSeparator * head = new SoSeparator;
  head->ref();
  FromPathFixtureKit * rootkit = new FromPathFixtureKit;
  SoSeparator * hidden = new SoSeparator;
  FromPathFixtureKit * childkit = new FromPathFixtureKit;
  SoTransform * leaf = new SoTransform;
  head->addChild(rootkit);
  rootkit->addFixtureChild(hidden);
  hidden->addChild(childkit);
  childkit->addFixtureChild(leaf);

  SoPath * source = new SoPath(head);
  source->ref();
  source->append(0);
  source->append(0);
  source->append(0);
  source->append(0);

  SoNodeKitPath * projected = SoNodeKitPath::fromPath(source);
  expect(projected != NULL, "fromPath(non-NULL) returns a path");
  if (projected) {
    SoPath * projectedbase = projected;
    expect(dynamic_cast<SoNodeKitPath *>(projectedbase) == projected,
           "fromPath result has genuine dynamic type");
    expect(projected->getRefCount() == 0, "fromPath result starts unreferenced");
    projected->ref();

    expect(fullLength(*projected) == 5, "fromPath copies full hidden route");
    for (int i = 0; i < fullLength(*source); ++i) {
      expect(projected->SoPath::getNode(i) == source->getNode(i),
             "fromPath preserves full nodes");
      expect(projected->SoPath::getIndex(i) == source->getIndex(i),
             "fromPath preserves child indices");
    }
    expect(projected->SoPath::getLength() == 2,
           "fromPath retains visible-prefix semantics");
    expect(projected->getLength() == 3,
           "fromPath K includes non-kit head and later kits");
    expect(projected->getNode(0) == head, "fromPath K starts at original head");
    expect(projected->getNode(1) == rootkit, "fromPath K contains root kit");
    expect(projected->getNode(2) == childkit, "fromPath K contains nested kit");

    projected->truncate(1);
    expect(fullLength(*source) == 5,
           "mutating materialized path does not mutate source");
    projected->unref();

    SoNodeKitPath * projected2 = SoNodeKitPath::fromPath(source);
    expect(projected2 != NULL, "second fromPath materialization succeeds");
    if (projected2) {
      projected2->ref();
      source->truncate(1);
      expect(fullLength(*projected2) == 5,
             "mutating source does not mutate materialized path");
      projected2->unref();
    }
  }

  source->unref();
  head->unref();

  if (failures) {
    std::fprintf(stderr, "[VIOLATED] %d fromPath observable(s)\n", failures);
    return 1;
  }
  std::fprintf(stderr, "[PRESERVED] fromPath observables\n");
  return 0;
}
