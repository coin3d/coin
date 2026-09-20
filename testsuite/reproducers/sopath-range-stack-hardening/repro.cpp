#include <Inventor/SoDB.h>
#include <Inventor/SoFullPath.h>
#include <Inventor/SoPath.h>
#include <Inventor/misc/SoTempPath.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSeparator.h>

#include <cstdio>
#include <cstring>
#include <exception>
#include <vector>

struct Snapshot {
  int length;
  std::vector<SoNode *> nodes;
  std::vector<int> indices;
};

static Snapshot
snapshot(const SoFullPath * path)
{
  Snapshot value;
  const int length = path->getLength();
  value.length = length;
  for (int i = 0; i < length; ++i) {
    value.nodes.push_back(path->getNode(i));
    value.indices.push_back(path->getIndex(i));
  }
  return value;
}

static bool
same(const Snapshot & lhs, const Snapshot & rhs)
{
  return lhs.length == rhs.length && lhs.nodes == rhs.nodes &&
    lhs.indices == rhs.indices;
}

class PathFixture {
public:
  PathFixture(const int length)
    : root(NULL), branch(NULL), leaf(NULL), path(new SoTempPath(4))
  {
    this->path->ref();
    if (length == 0) return;

    this->root = new SoSeparator;
    this->root->ref();
    this->path->setHead(this->root);
    if (length == 1) return;

    this->branch = new SoSeparator;
    this->root->addChild(this->branch);
    this->path->append(this->branch);
    if (length == 2) return;

    this->leaf = new SoCube;
    this->branch->addChild(this->leaf);
    this->path->append(this->leaf);
  }

  ~PathFixture()
  {
    this->path->unref();
    if (this->root != NULL) this->root->unref();
  }

  SoPath * base() { return this->path; }
  SoFullPath * full() { return this->path; }

  SoSeparator * root;
  SoSeparator * branch;
  SoCube * leaf;
  SoTempPath * path;
};

static bool
is_prefix(const Snapshot & before, const Snapshot & after, const int length)
{
  if (after.length != length || before.length < length) return false;
  for (int i = 0; i < length; ++i) {
    if (after.nodes[i] != before.nodes[i] ||
        after.indices[i] != before.indices[i]) {
      return false;
    }
  }
  return true;
}

static bool
truncate_negative_empty()
{
  PathFixture f(0);
  const Snapshot before = snapshot(f.full());
  f.base()->truncate(-1);
  return same(before, snapshot(f.full()));
}

static bool
truncate_negative_nonempty()
{
  PathFixture f(3);
  const Snapshot before = snapshot(f.full());
  f.base()->truncate(-1);
  return same(before, snapshot(f.full()));
}

static bool
truncate_above_empty()
{
  PathFixture f(0);
  const Snapshot before = snapshot(f.full());
  f.base()->truncate(1);
  return same(before, snapshot(f.full()));
}

static bool
truncate_above_nonempty()
{
  PathFixture f(3);
  const Snapshot before = snapshot(f.full());
  f.base()->truncate(4);
  return same(before, snapshot(f.full()));
}

static bool
truncate_zero()
{
  PathFixture f(3);
  f.base()->truncate(0);
  return f.full()->getLength() == 0;
}

static bool
truncate_current()
{
  PathFixture f(3);
  const Snapshot before = snapshot(f.full());
  f.base()->truncate(3);
  return same(before, snapshot(f.full()));
}

static bool
truncate_intermediate()
{
  PathFixture f(3);
  const Snapshot before = snapshot(f.full());
  f.base()->truncate(2);
  return is_prefix(before, snapshot(f.full()), 2);
}

static bool
sopath_pop_empty()
{
  PathFixture f(0);
  const Snapshot before = snapshot(f.full());
  f.base()->pop();
  return same(before, snapshot(f.full()));
}

static bool
sofullpath_pop_empty()
{
  PathFixture f(0);
  const Snapshot before = snapshot(f.full());
  f.full()->pop();
  return same(before, snapshot(f.full()));
}

static bool
sopath_pop_nonempty()
{
  PathFixture f(3);
  const Snapshot before = snapshot(f.full());
  f.base()->pop();
  return is_prefix(before, snapshot(f.full()), 2);
}

static bool
sofullpath_pop_nonempty()
{
  PathFixture f(3);
  const Snapshot before = snapshot(f.full());
  f.full()->pop();
  return is_prefix(before, snapshot(f.full()), 2);
}

struct Case {
  const char * name;
  bool (*run)();
};

static const Case cases[] = {
  { "truncate_negative_empty", truncate_negative_empty },
  { "truncate_negative_nonempty", truncate_negative_nonempty },
  { "truncate_above_empty", truncate_above_empty },
  { "truncate_above_nonempty", truncate_above_nonempty },
  { "truncate_zero", truncate_zero },
  { "truncate_current", truncate_current },
  { "truncate_intermediate", truncate_intermediate },
  { "sopath_pop_empty", sopath_pop_empty },
  { "sofullpath_pop_empty", sofullpath_pop_empty },
  { "sopath_pop_nonempty", sopath_pop_nonempty },
  { "sofullpath_pop_nonempty", sofullpath_pop_nonempty }
};

int
main(int argc, char ** argv)
{
  if (argc != 2) {
    std::fprintf(stderr, "usage: %s CASE\n", argv[0]);
    return 2;
  }

  SoDB::init();
  int result = 2;
  for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
    if (std::strcmp(argv[1], cases[i].name) != 0) continue;
    try {
      result = cases[i].run() ? 0 : 1;
    }
    catch (const std::exception & e) {
      std::fprintf(stderr, "[repro] %s FAIL: exception: %s\n", argv[1], e.what());
      result = 1;
    }
    catch (...) {
      std::fprintf(stderr, "[repro] %s FAIL: unknown exception\n", argv[1]);
      result = 1;
    }
    break;
  }
  SoDB::finish();

  if (result == 0) std::fprintf(stderr, "[repro] %s PASS\n", argv[1]);
  else if (result == 1) std::fprintf(stderr, "[repro] %s FAIL\n", argv[1]);
  else std::fprintf(stderr, "[repro] unknown case: %s\n", argv[1]);
  return result;
}
