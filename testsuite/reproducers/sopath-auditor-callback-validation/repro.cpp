// Regression test for SoPath's public child-list auditor callbacks.

#include <cstdio>
#include <cstring>

#include <Inventor/SoDB.h>
#include <Inventor/SoPath.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSeparator.h>

struct PathFixture {
  PathFixture()
    : root(new SoSeparator), selected(new SoGroup), path(new SoPath(root))
  {
    root->ref();
    root->addChild(selected);
    path->ref();
    path->append(selected);
  }

  ~PathFixture()
  {
    path->unref();
    root->unref();
  }

  bool unchanged() const
  {
    return path->getLength() == 2 && path->getNode(0) == root &&
           path->getNode(1) == selected && path->getIndex(0) == 0 &&
           path->getIndex(1) == 0;
  }

  SoSeparator * root;
  SoGroup * selected;
  SoPath * path;
};

static int
test_empty()
{
  SoPath * path = new SoPath;
  path->ref();
  SoGroup * parent = new SoGroup;
  parent->ref();
  SoGroup * replacement = new SoGroup;
  replacement->ref();

  path->insertIndex(parent, 0);
  path->removeIndex(parent, 0);
  path->replaceIndex(parent, 0, replacement);
  const bool ok = path->getLength() == 0 && path->getHead() == NULL;

  path->unref();
  parent->unref();
  replacement->unref();
  if (!ok) {
    std::fprintf(stderr, "FAIL: a callback changed an empty path\n");
    return 1;
  }
  return 0;
}

static int
test_foreign_parent()
{
  PathFixture fixture;
  SoGroup * foreign = new SoGroup;
  foreign->ref();
  SoGroup * replacement = new SoGroup;
  replacement->ref();

  fixture.path->insertIndex(foreign, 0);
  fixture.path->removeIndex(foreign, 0);
  fixture.path->replaceIndex(foreign, 0, replacement);
  fixture.path->insertIndex(NULL, 0);
  fixture.path->removeIndex(NULL, 0);
  fixture.path->replaceIndex(NULL, 0, replacement);
  const bool ok = fixture.unchanged();

  foreign->unref();
  replacement->unref();
  if (!ok) {
    std::fprintf(stderr, "FAIL: a foreign parent changed the path\n");
    return 1;
  }
  return 0;
}

static int
test_negative_index()
{
  {
    PathFixture fixture;
    fixture.path->insertIndex(fixture.root, -1);
    if (!fixture.unchanged()) {
      std::fprintf(stderr, "FAIL: negative insertion index changed the path\n");
      return 1;
    }
  }
  {
    PathFixture fixture;
    fixture.path->removeIndex(fixture.root, -1);
    if (!fixture.unchanged()) {
      std::fprintf(stderr, "FAIL: negative removal index changed the path\n");
      return 1;
    }
  }
  return 0;
}

static int
test_null_replacement()
{
  PathFixture fixture;
  fixture.path->replaceIndex(fixture.root, 0, NULL);
  if (!fixture.unchanged()) {
    std::fprintf(stderr, "FAIL: null replacement changed the path\n");
    return 1;
  }
  return 0;
}

static int
test_valid_callbacks()
{
  PathFixture fixture;

  fixture.root->insertChild(new SoGroup, 0);
  if (fixture.path->getIndex(1) != 1) {
    std::fprintf(stderr, "FAIL: valid insertion did not update the index\n");
    return 1;
  }

  SoGroup * replacement = new SoGroup;
  fixture.root->replaceChild(1, replacement);
  if (fixture.path->getLength() != 2 ||
      fixture.path->getNode(1) != replacement ||
      fixture.path->getIndex(1) != 1) {
    std::fprintf(stderr, "FAIL: valid replacement did not update the path\n");
    return 1;
  }

  fixture.root->removeChild(0);
  if (fixture.path->getIndex(1) != 0) {
    std::fprintf(stderr, "FAIL: valid removal did not update the index\n");
    return 1;
  }

  fixture.root->removeChild(0);
  if (fixture.path->getLength() != 1) {
    std::fprintf(stderr, "FAIL: removing the selected child did not truncate\n");
    return 1;
  }
  return 0;
}

int
main(int argc, char ** argv)
{
  if (argc != 2) {
    std::fprintf(stderr,
                 "usage: %s empty|foreign-parent|negative-index|"
                 "null-replacement|valid-callbacks\n", argv[0]);
    return 2;
  }

  SoDB::init();
  int result = 2;
  if (std::strcmp(argv[1], "empty") == 0)
    result = test_empty();
  else if (std::strcmp(argv[1], "foreign-parent") == 0)
    result = test_foreign_parent();
  else if (std::strcmp(argv[1], "negative-index") == 0)
    result = test_negative_index();
  else if (std::strcmp(argv[1], "null-replacement") == 0)
    result = test_null_replacement();
  else if (std::strcmp(argv[1], "valid-callbacks") == 0)
    result = test_valid_callbacks();
  else
    std::fprintf(stderr, "unknown case: %s\n", argv[1]);
  SoDB::finish();
  return result;
}
