// Independent public-API contract test for safe SoPath child appends.
// Each named case runs in a fresh process (see run.sh), so a crash in one
// invalid-input case does not mask the remaining cases.

#include <cstdio>
#include <cstring>

#include <Inventor/SoDB.h>
#include <Inventor/SoPath.h>
#include <Inventor/SoNodeKitPath.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/nodekits/SoShapeKit.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSphere.h>

static int failures = 0;

static void
check(const bool condition, const char * message)
{
  if (!condition) {
    std::fprintf(stderr, "FAIL: %s\n", message);
    ++failures;
  }
}

static SoPath *
retain(SoPath * path)
{
  path->ref();
  return path;
}

static bool
same_public_path(const SoPath * actual, const SoPath * expected)
{
  if (!(*actual == *expected)) return false;
  if (actual->getLength() != expected->getLength()) return false;
  if (actual->getHead() != expected->getHead()) return false;
  if (actual->getLength() > 0 && actual->getTail() != expected->getTail()) return false;
  for (int i = 0; i < actual->getLength(); ++i) {
    if (actual->getNode(i) != expected->getNode(i)) return false;
    if (actual->getIndex(i) != expected->getIndex(i)) return false;
  }
  return true;
}

static SoPath *
snapshot(const SoPath * path)
{
  return retain(new SoPath(*path));
}

static void
expect_unchanged(SoPath * path, SoPath * before, const char * label)
{
  check(same_public_path(path, before), label);
  before->unref();
}

static void
invalid_int_empty(const bool usepush)
{
  SoPath * path = retain(new SoPath);
  SoPath * before = snapshot(path);
  if (usepush) path->push(0); else path->append(0);
  expect_unchanged(path, before, "empty path changed after integer append");
  path->unref();
}

static void
invalid_int_leaf(const bool usepush)
{
  SoCube * leaf = new SoCube;
  leaf->ref();
  SoPath * path = retain(new SoPath(leaf));
  SoPath * before = snapshot(path);
  if (usepush) path->push(0); else path->append(0);
  expect_unchanged(path, before, "leaf-tail path changed after integer append");
  path->unref();
  leaf->unref();
}

static void
invalid_int_boundary(const bool usepush, const bool negative)
{
  SoSeparator * root = new SoSeparator;
  root->ref();
  root->addChild(new SoCube);
  SoPath * path = retain(new SoPath(root));
  SoPath * before = snapshot(path);
  const int index = negative ? -1 : root->getNumChildren();
  if (usepush) path->push(index); else path->append(index);
  expect_unchanged(path, before,
                   negative ? "path changed after negative index"
                            : "path changed after index == child_count");
  path->unref();
  root->unref();
}

static void
invalid_node_leaf(void)
{
  SoCube * leaf = new SoCube;
  SoSphere * candidate = new SoSphere;
  leaf->ref();
  candidate->ref();
  SoPath * path = retain(new SoPath(leaf));
  SoPath * before = snapshot(path);
  path->append(candidate);
  expect_unchanged(path, before, "leaf-tail path changed after node append");
  path->unref();
  candidate->unref();
  leaf->unref();
}

static void
invalid_node_empty_group(void)
{
  SoSeparator * empty = new SoSeparator;
  SoCube * candidate = new SoCube;
  empty->ref();
  candidate->ref();
  SoPath * path = retain(new SoPath(empty));
  SoPath * before = snapshot(path);
  path->append(candidate);
  expect_unchanged(path, before, "zero-child group path changed after node append");
  path->unref();
  candidate->unref();
  empty->unref();
}

static void
invalid_node_nonchild(void)
{
  SoSeparator * root = new SoSeparator;
  SoSphere * outsider = new SoSphere;
  root->ref();
  outsider->ref();
  root->addChild(new SoCube);
  SoPath * path = retain(new SoPath(root));
  SoPath * before = snapshot(path);
  path->append(outsider);
  expect_unchanged(path, before, "path changed after non-child node append");
  path->unref();
  outsider->unref();
  root->unref();
}

static SoPath *
make_suffix(void)
{
  SoSeparator * head = new SoSeparator;
  SoCube * leaf = new SoCube;
  head->addChild(leaf);
  SoPath * suffix = retain(new SoPath(head));
  suffix->append(leaf);
  return suffix;
}

static void
invalid_path_leaf(void)
{
  SoCube * leaf = new SoCube;
  leaf->ref();
  SoPath * path = retain(new SoPath(leaf));
  SoPath * suffix = make_suffix();
  SoPath * before = snapshot(path);
  path->append(suffix);
  expect_unchanged(path, before, "leaf-tail path changed after path append");
  suffix->unref();
  path->unref();
  leaf->unref();
}

static void
invalid_path_empty_group(void)
{
  SoSeparator * empty = new SoSeparator;
  empty->ref();
  SoPath * path = retain(new SoPath(empty));
  SoPath * suffix = make_suffix();
  SoPath * before = snapshot(path);
  path->append(suffix);
  expect_unchanged(path, before, "zero-child group path changed after path append");
  suffix->unref();
  path->unref();
  empty->unref();
}

static void
valid_int(const bool usepush)
{
  SoSeparator * root = new SoSeparator;
  SoCube * child = new SoCube;
  root->ref();
  root->addChild(child);
  SoPath * path = retain(new SoPath(root));
  if (usepush) path->push(0); else path->append(0);
  check(path->getLength() == 2, "valid integer append did not grow path");
  check(path->getTail() == child, "valid integer append selected wrong child");
  check(path->getIndex(1) == 0, "valid integer append stored wrong index");
  path->unref();
  root->unref();
}

static void
valid_node(void)
{
  SoSeparator * root = new SoSeparator;
  SoCube * child = new SoCube;
  root->ref();
  root->addChild(child);
  SoPath * path = retain(new SoPath(root));
  path->append(child);
  check(path->getLength() == 2, "valid node append did not grow path");
  check(path->getTail() == child, "valid node append selected wrong child");
  path->unref();
  root->unref();
}

static void
valid_path(void)
{
  SoSeparator * root = new SoSeparator;
  SoSeparator * branch = new SoSeparator;
  SoCube * leaf = new SoCube;
  root->ref();
  root->addChild(branch);
  branch->addChild(leaf);

  SoPath * receiver = retain(new SoPath(root));
  SoPath * suffix = retain(new SoPath(branch));
  suffix->append(leaf);
  receiver->append(suffix);

  check(receiver->getLength() == 3, "valid path append did not append full suffix");
  check(receiver->getNode(1) == branch, "valid path append omitted suffix head");
  check(receiver->getTail() == leaf, "valid path append selected wrong tail");
  suffix->unref();
  receiver->unref();
  root->unref();
}

static SoPath *
hidden_path_to_shape_separator(SoShapeKit * kit)
{
  check(kit->setPart("shape", new SoCube), "could not create shape part");
  SoNodeKitPath * nodekitpath = kit->createPathToPart("shape", TRUE);
  check(nodekitpath != NULL, "could not create hidden nodekit path");
  if (nodekitpath == NULL) return NULL;
  SoPath * path = retain(static_cast<SoPath *>(nodekitpath));
  // Call the base API deliberately: removing the shape leaves the hidden
  // shapeSeparator as the full tail while the public SoPath view ends at kit.
  path->pop();
  return path;
}

static SoPath *
make_hidden_tail_path(SoShapeKit *& kit)
{
  kit = new SoShapeKit;
  kit->ref();
  SoPath * path = hidden_path_to_shape_separator(kit);
  if (path == NULL) {
    kit->unref();
    kit = NULL;
  }
  return path;
}

static void
hidden_child_valid(void)
{
  SoShapeKit * kit = NULL;
  SoPath * path = make_hidden_tail_path(kit);
  if (path == NULL) return;

  const int visiblelength = path->getLength();
  SoNode * const visibletail = path->getTail();
  SoNode * const shape = kit->getPart("shape", FALSE);
  check(shape != NULL, "shape part was not created");

  SoPath * beforevalid = snapshot(path);
  path->append(shape);
  check(!(*path == *beforevalid), "valid append below hidden tail did not mutate full path");
  check(path->getLength() == visiblelength,
        "valid append below hidden tail changed visible length");
  check(path->getTail() == visibletail,
        "valid append below hidden tail exposed hidden tail");
  beforevalid->unref();

  path->unref();
  kit->unref();
}

static void
hidden_child_invalid(void)
{
  SoShapeKit * kit = NULL;
  SoPath * path = make_hidden_tail_path(kit);
  if (path == NULL) return;

  SoSphere * outsider = new SoSphere;
  outsider->ref();
  SoPath * beforeinvalid = snapshot(path);
  path->append(outsider);
  expect_unchanged(path, beforeinvalid,
                   "invalid append below hidden tail mutated or exposed path");
  outsider->unref();
  path->unref();
  kit->unref();
}

struct NamedCase {
  const char * name;
  void (*run)(void);
};

static void append_int_empty(void) { invalid_int_empty(false); }
static void push_int_empty(void) { invalid_int_empty(true); }
static void append_int_leaf(void) { invalid_int_leaf(false); }
static void push_int_leaf(void) { invalid_int_leaf(true); }
static void append_int_negative(void) { invalid_int_boundary(false, true); }
static void push_int_negative(void) { invalid_int_boundary(true, true); }
static void append_int_at_count(void) { invalid_int_boundary(false, false); }
static void push_int_at_count(void) { invalid_int_boundary(true, false); }
static void append_int_valid(void) { valid_int(false); }
static void push_int_valid(void) { valid_int(true); }

static const NamedCase cases[] = {
  { "append-int-empty", append_int_empty },
  { "push-int-empty", push_int_empty },
  { "append-int-leaf", append_int_leaf },
  { "push-int-leaf", push_int_leaf },
  { "append-int-negative", append_int_negative },
  { "push-int-negative", push_int_negative },
  { "append-int-at-count", append_int_at_count },
  { "push-int-at-count", push_int_at_count },
  { "append-node-leaf", invalid_node_leaf },
  { "append-node-empty-group", invalid_node_empty_group },
  { "append-node-nonchild", invalid_node_nonchild },
  { "append-path-leaf", invalid_path_leaf },
  { "append-path-empty-group", invalid_path_empty_group },
  { "append-int-valid", append_int_valid },
  { "push-int-valid", push_int_valid },
  { "append-node-valid", valid_node },
  { "append-path-valid", valid_path },
  { "hidden-child-valid", hidden_child_valid },
  { "hidden-child-invalid", hidden_child_invalid }
};

int
main(int argc, char ** argv)
{
  if (argc == 2 && std::strcmp(argv[1], "--list") == 0) {
    for (unsigned int i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i)
      std::puts(cases[i].name);
    return 0;
  }
  if (argc != 2) {
    std::fprintf(stderr, "usage: %s --list | CASE\n", argv[0]);
    return 2;
  }

  const NamedCase * selected = NULL;
  for (unsigned int i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
    if (std::strcmp(argv[1], cases[i].name) == 0) selected = &cases[i];
  }
  if (selected == NULL) {
    std::fprintf(stderr, "unknown case: %s\n", argv[1]);
    return 2;
  }

  SoDB::init();
  SoNodeKit::init();
  selected->run();
  SoDB::finish();

  if (failures != 0) {
    std::fprintf(stderr, "[%s] FAIL: %d check(s)\n", selected->name, failures);
    return 1;
  }
  std::fprintf(stderr, "[%s] PASS\n", selected->name);
  return 0;
}
