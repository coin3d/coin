#include <Inventor/SoDB.h>
#include <Inventor/SoFullPath.h>
#include <Inventor/SoPath.h>
#include <Inventor/misc/SoTempPath.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSeparator.h>

#include <climits>
#include <cstdio>
#include <cstring>

static void
release_path(SoPath * path)
{
  if (path == NULL) return;
  path->ref();
  path->unref();
}

class Fixture {
public:
  explicit Fixture(const bool temporary)
    : root(new SoSeparator), branch(new SoSeparator), leaf(new SoCube),
      path(NULL)
  {
    this->root->ref();
    this->root->addChild(new SoCube);
    this->root->addChild(this->branch);
    this->branch->addChild(new SoCube);
    this->branch->addChild(new SoCube);
    this->branch->addChild(this->leaf);

    if (temporary) {
      SoTempPath * tmp = new SoTempPath(3);
      this->path = tmp;
      this->path->ref();
      tmp->simpleAppend(this->root, 0);
      tmp->simpleAppend(this->branch, 1);
      tmp->simpleAppend(this->leaf, 2);
    }
    else {
      this->path = new SoPath(this->root);
      this->path->ref();
      this->path->append(this->branch);
      this->path->append(this->leaf);
    }
  }

  ~Fixture()
  {
    this->path->unref();
    this->root->unref();
  }

  SoFullPath * full(SoPath * value) const
  {
    return static_cast<SoFullPath *>(value);
  }

  SoSeparator * root;
  SoSeparator * branch;
  SoCube * leaf;
  SoPath * path;
};

static bool
empty_default()
{
  SoPath * path = new SoPath;
  path->ref();
  SoPath * result = path->copy();
  const bool ok = result == NULL &&
    static_cast<SoFullPath *>(path)->getLength() == 0;
  release_path(result);
  path->unref();
  return ok;
}

static bool
partial_new_head()
{
  Fixture f(false);
  SoPath * result = f.path->copy(1, 2);
  bool ok = result != NULL;
  if (result != NULL) {
    result->ref();
    SoFullPath * full = f.full(result);
    ok = full->getLength() == 2 && full->getNode(0) == f.branch &&
      full->getNode(1) == f.leaf && full->getIndex(0) == 0 &&
      full->getIndex(1) == 2;
    result->unref();
  }
  return ok && f.full(f.path)->getLength() == 3 &&
    f.full(f.path)->getIndex(1) == 1 && f.full(f.path)->getIndex(2) == 2;
}

static bool
int_max_count()
{
  Fixture f(false);
  SoPath * result = f.path->copy(1, INT_MAX);
  bool ok = result != NULL;
  if (result != NULL) {
    result->ref();
    SoFullPath * full = f.full(result);
    ok = full->getLength() == 2 && full->getNode(0) == f.branch &&
      full->getNode(1) == f.leaf && full->getIndex(0) == 0 &&
      full->getIndex(1) == 2;
    result->unref();
  }
  return ok && f.full(f.path)->getLength() == 3;
}

static bool
temppath_result_audits()
{
  Fixture f(true);
  SoPath * result = f.path->copy();
  if (result == NULL) return false;
  result->ref();
  f.root->insertChild(new SoCube, 0);
  SoFullPath * copied = f.full(result);
  const bool ok = copied->getLength() == 3 && copied->getIndex(1) == 2 &&
    f.full(f.path)->getLength() == 3 && f.full(f.path)->getIndex(1) == 1;
  result->unref();
  return ok;
}

int
main(int argc, char ** argv)
{
  if (argc != 2) {
    std::fprintf(stderr, "usage: %s CASE\n", argv[0]);
    return 2;
  }

  SoDB::init();
  bool known = true;
  bool ok = false;
  if (std::strcmp(argv[1], "empty_default") == 0) ok = empty_default();
  else if (std::strcmp(argv[1], "partial_new_head") == 0) ok = partial_new_head();
  else if (std::strcmp(argv[1], "int_max_count") == 0) ok = int_max_count();
  else if (std::strcmp(argv[1], "temppath_result_audits") == 0) {
    ok = temppath_result_audits();
  }
  else known = false;
  SoDB::finish();

  if (!known) return 2;
  std::fprintf(stderr, "[old-header] %s %s\n", argv[1],
               ok ? "PRESERVED" : "VIOLATED");
  return ok ? 0 : 1;
}
