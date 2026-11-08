#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/manips/SoHandleBoxManip.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodekits/SoSeparatorKit.h>
#include <Inventor/nodes/SoTransform.h>

#include <cstdio>

class FixtureKit : public SoBaseKit {
public:
  FixtureKit() : SoBaseKit() {}
  ~FixtureKit() override {}
  void addFixtureChild(SoNode * node) { this->children->append(node); }
};

static int
runTest()
{
  FixtureKit * outer = new FixtureKit;
  outer->ref();
  SoSeparatorKit * inner = new SoSeparatorKit;
  outer->addFixtureChild(inner);
  SoNode * original = inner->getPart("transform", TRUE);

  outer->setSearchingChildren(TRUE);
  inner->setSearchingChildren(TRUE);
  SoSearchAction search;
  search.setNode(original);
  search.apply(outer);
  SoPath * path = search.getPath();
  if (path == NULL || path->getTail() != outer ||
      path->fullPath().getTail() != original) {
    std::fprintf(stderr, "failed to construct a nested nodekit path\n");
    outer->unref();
    return 2;
  }

  SoHandleBoxManip * manip = new SoHandleBoxManip;
  manip->ref();
  const bool replaced = manip->replaceNode(path);
  bool correct = replaced && inner->getPart("transform", FALSE) == manip;
  if (!correct) {
    std::fprintf(stderr, "replaceNode selected the wrong nodekit\n");
  }

  if (correct) {
    SoSearchAction reverseSearch;
    reverseSearch.setNode(manip);
    reverseSearch.apply(outer);
    SoPath * reversePath = reverseSearch.getPath();
    SoTransform * replacement = new SoTransform;
    replacement->ref();
    correct = reversePath != NULL && manip->replaceManip(reversePath, replacement) &&
              inner->getPart("transform", FALSE) == replacement;
    if (!correct) {
      std::fprintf(stderr, "replaceManip selected the wrong nodekit\n");
    }
    replacement->unref();
  }
  manip->unref();
  outer->unref();
  return correct ? 0 : 1;
}

int
main()
{
  SoDB::init();
  SoInteraction::init();
  const int result = runTest();
  SoDB::finish();
  return result;
}
