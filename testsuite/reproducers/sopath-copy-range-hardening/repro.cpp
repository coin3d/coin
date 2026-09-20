#include <Inventor/SoDB.h>
#include <Inventor/SoFullPath.h>
#include <Inventor/SoPath.h>
#include <Inventor/misc/SoTempPath.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSeparator.h>

#include <climits>
#include <cstdio>
#include <cstring>
#include <exception>
#include <vector>

struct Snapshot {
  std::vector<SoNode *> nodes;
  std::vector<int> indices;
};

static Snapshot
snapshot(const SoPath * path)
{
  const SoFullPath * full = static_cast<const SoFullPath *>(path);
  Snapshot value;
  const int length = full->getLength();
  for (int i = 0; i < length; ++i) {
    value.nodes.push_back(full->getNode(i));
    value.indices.push_back(full->getIndex(i));
  }
  return value;
}

static bool
same(const Snapshot & lhs, const Snapshot & rhs)
{
  return lhs.nodes == rhs.nodes && lhs.indices == rhs.indices;
}

static void
release_path(SoPath * path)
{
  if (path == NULL) return;
  path->ref();
  path->unref();
}

class PathFixture {
public:
  explicit PathFixture(const bool temporary = false)
    : root(new SoSeparator), branch(new SoSeparator), leaf(new SoCube),
      path(NULL)
  {
    this->root->ref();

    this->root->addChild(new SoCube);
    this->root->addChild(this->branch); // relationship index 1
    this->root->addChild(new SoCube);

    this->branch->addChild(new SoCube);
    this->branch->addChild(new SoCube);
    this->branch->addChild(this->leaf); // relationship index 2

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

  ~PathFixture()
  {
    this->path->unref();
    this->root->unref();
  }

  SoFullPath * full()
  {
    return static_cast<SoFullPath *>(this->path);
  }

  SoSeparator * root;
  SoSeparator * branch;
  SoCube * leaf;
  SoPath * path;
};

class HiddenFixture {
public:
  HiddenFixture()
    : root(new SoSeparator), kit(new SoBaseKit), branch(new SoSeparator),
      leaf(new SoCube), path(new SoTempPath(4))
  {
    this->root->ref();
    this->kit->ref();
    this->branch->ref();
    this->leaf->ref();
    this->path->ref();

    this->branch->addChild(new SoCube);
    this->branch->addChild(new SoCube);
    this->branch->addChild(this->leaf);

    this->path->simpleAppend(this->root, 0);
    this->path->simpleAppend(this->kit, 1);
    this->path->simpleAppend(this->branch, 7);
    this->path->simpleAppend(this->leaf, 2);
  }

  ~HiddenFixture()
  {
    this->path->unref();
    this->leaf->unref();
    this->branch->unref();
    this->kit->unref();
    this->root->unref();
  }

  SoFullPath * full()
  {
    return static_cast<SoFullPath *>(this->path);
  }

  SoSeparator * root;
  SoBaseKit * kit;
  SoSeparator * branch;
  SoCube * leaf;
  SoTempPath * path;
};

static bool
matches_slice(const Snapshot & source, const SoPath * result,
              const int start, const int count)
{
  if (result == NULL || start < 0 || count <= 0 ||
      start + count > static_cast<int>(source.nodes.size())) return false;

  const Snapshot copied = snapshot(result);
  if (static_cast<int>(copied.nodes.size()) != count ||
      copied.indices.size() != copied.nodes.size()) return false;

  for (int i = 0; i < count; ++i) {
    if (copied.nodes[i] != source.nodes[start + i]) return false;
    const int expected_index = i == 0 ? 0 : source.indices[start + i];
    if (copied.indices[i] != expected_index) return false;
  }
  return true;
}

static bool
expect_null(SoPath * source, const int start, const int count,
            const bool use_defaults = false)
{
  const Snapshot before = snapshot(source);
  SoPath * result = use_defaults ? source->copy() : source->copy(start, count);
  const bool ok = result == NULL && same(before, snapshot(source));
  release_path(result);
  return ok;
}

static bool
expect_copy(SoPath * source, const int start, const int count,
            const int expected_count, const bool use_defaults = false)
{
  const Snapshot before = snapshot(source);
  SoPath * result = use_defaults ? source->copy() : source->copy(start, count);
  const bool ok = same(before, snapshot(source)) &&
    matches_slice(before, result, start, expected_count);
  release_path(result);
  return ok;
}

static bool empty_default()
{
  SoPath * path = new SoPath;
  path->ref();
  const bool ok = expect_null(path, 0, 0, true);
  path->unref();
  return ok;
}

static bool start_negative()
{
  PathFixture f;
  return expect_null(f.path, -1, 0);
}

static bool start_equal_full_length()
{
  PathFixture f;
  return expect_null(f.path, f.full()->getLength(), 0);
}

static bool start_above_full_length()
{
  PathFixture f;
  return expect_null(f.path, f.full()->getLength() + 1, 1);
}

static bool start_int_max()
{
  PathFixture f;
  return expect_null(f.path, INT_MAX, 1);
}

static bool count_negative()
{
  PathFixture f;
  return expect_null(f.path, 0, -1);
}

static bool count_int_min()
{
  PathFixture f;
  return expect_null(f.path, 1, INT_MIN);
}

static bool full_default()
{
  PathFixture f;
  return expect_copy(f.path, 0, 0, 3, true);
}

static bool full_explicit()
{
  PathFixture f;
  return expect_copy(f.path, 0, 3, 3);
}

static bool partial_from_head()
{
  PathFixture f;
  return expect_copy(f.path, 0, 2, 2);
}

static bool partial_new_head_one()
{
  PathFixture f;
  return expect_copy(f.path, 1, 1, 1);
}

static bool partial_new_head_exact()
{
  PathFixture f;
  return expect_copy(f.path, 1, 2, 2);
}

static bool zero_to_end_new_head()
{
  PathFixture f;
  return expect_copy(f.path, 1, 0, 2);
}

static bool excessive_from_head()
{
  PathFixture f;
  return expect_copy(f.path, 0, 4, 3);
}

static bool excessive_new_head()
{
  PathFixture f;
  return expect_copy(f.path, 1, 99, 2);
}

static bool int_max_count()
{
  PathFixture f;
  return expect_copy(f.path, 1, INT_MAX, 2);
}

static bool hidden_full_default()
{
  HiddenFixture f;
  if (static_cast<SoPath *>(f.path)->getLength() != 2 ||
      f.full()->getLength() != 4) return false;
  return expect_copy(f.path, 0, 0, 4, true);
}

static bool hidden_start_after_visible()
{
  HiddenFixture f;
  if (static_cast<SoPath *>(f.path)->getLength() != 2 ||
      f.full()->getLength() != 4) return false;
  return expect_copy(f.path, 2, 0, 2);
}

static bool temppath_result_audits()
{
  PathFixture f(true);
  const Snapshot before = snapshot(f.path);
  SoPath * result = f.path->copy();
  if (result == NULL) return false;
  result->ref();

  bool ok = matches_slice(before, result, 0, 3) &&
    same(before, snapshot(f.path));
  f.root->insertChild(new SoCube, 0);

  const Snapshot source_after = snapshot(f.path);
  const Snapshot result_after = snapshot(result);
  ok = ok && same(before, source_after) &&
    result_after.nodes == before.nodes && result_after.indices.size() == 3 &&
    result_after.indices[0] == 0 && result_after.indices[1] == 2 &&
    result_after.indices[2] == 2;

  result->unref();
  return ok;
}

struct Case {
  const char * name;
  bool (*run)();
};

static const Case cases[] = {
  { "empty_default", empty_default },
  { "start_negative", start_negative },
  { "start_equal_full_length", start_equal_full_length },
  { "start_above_full_length", start_above_full_length },
  { "start_int_max", start_int_max },
  { "count_negative", count_negative },
  { "count_int_min", count_int_min },
  { "full_default", full_default },
  { "full_explicit", full_explicit },
  { "partial_from_head", partial_from_head },
  { "partial_new_head_one", partial_new_head_one },
  { "partial_new_head_exact", partial_new_head_exact },
  { "zero_to_end_new_head", zero_to_end_new_head },
  { "excessive_from_head", excessive_from_head },
  { "excessive_new_head", excessive_new_head },
  { "int_max_count", int_max_count },
  { "hidden_full_default", hidden_full_default },
  { "hidden_start_after_visible", hidden_start_after_visible },
  { "temppath_result_audits", temppath_result_audits }
};

int
main(int argc, char ** argv)
{
  if (argc != 2) {
    std::fprintf(stderr, "usage: %s CASE\n", argv[0]);
    return 2;
  }

  SoDB::init();
  SoNodeKit::init();
  int result = 2;
  for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
    if (std::strcmp(argv[1], cases[i].name) != 0) continue;
    try {
      result = cases[i].run() ? 0 : 1;
    }
    catch (const std::exception & e) {
      std::fprintf(stderr, "[repro] %s FAIL: exception: %s\n",
                   argv[1], e.what());
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
