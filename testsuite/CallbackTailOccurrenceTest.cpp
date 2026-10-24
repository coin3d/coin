#include <Inventor/SoDB.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/lists/SoPathList.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoTranslation.h>
#ifdef TEST_NODEKITS
#include <Inventor/SoNodeKitPath.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/nodekits/SoShapeKit.h>
#include <Inventor/nodes/SoCube.h>
#endif

#include <cstdio>
#include <string>

struct Context {
  SoPath * expected;
  SoNode * leaf;
  int pre;
  int post;
  int visits;
  int failures;
};

static SoCallbackAction::Response
visit(void * userdata, SoCallbackAction *, const SoNode * node)
{
  Context & context = *static_cast<Context *>(userdata);
  if (node == context.leaf) ++context.visits;
  return SoCallbackAction::CONTINUE;
}

static bool
matches(SoCallbackAction * action, SoPath * path)
{
  const SoFullPathView current = action->getCurPath()->fullPath();
  const SoFullPathView expected = path->fullPath();
  if (current.getLength() != expected.getLength()) return false;
  for (int i = 0; i < expected.getLength(); ++i) {
    if (current.getNodeFromTail(i) != expected.getNodeFromTail(i)) return false;
    if (i + 1 < expected.getLength() &&
        current.getIndexFromTail(i) != expected.getIndexFromTail(i)) return false;
  }
  return true;
}

static SoCallbackAction::Response
tailpre(void * userdata, SoCallbackAction * action, const SoNode * node)
{
  Context & context = *static_cast<Context *>(userdata);
  ++context.pre;
  if (node != context.leaf || !matches(action, context.expected)) {
    std::fprintf(stderr, "pre-tail at wrong physical occurrence\n");
    ++context.failures;
  }
  return SoCallbackAction::CONTINUE;
}

static SoCallbackAction::Response
tailpost(void * userdata, SoCallbackAction * action, const SoNode * node)
{
  Context & context = *static_cast<Context *>(userdata);
  ++context.post;
  if (node != context.leaf || !matches(action, context.expected)) {
    std::fprintf(stderr, "post-tail at wrong physical occurrence\n");
    ++context.failures;
  }
  return SoCallbackAction::CONTINUE;
}

static int
run(SoPath * path, SoNode * leaf, int visits, bool controls)
{
  int failures = 0;
  for (int kind = controls ? 1 : 0; kind < (controls ? 4 : 1); ++kind) {
    Context context = { path, leaf, 0, 0, 0, 0 };
    SoCallbackAction action;
    action.addPreCallback(SoNode::getClassTypeId(), visit, &context);
    action.addPreTailCallback(tailpre, &context);
    action.addPostTailCallback(tailpost, &context);
    if (kind == 0) action.apply(path);
    else if (kind == 3) action.apply(path->getHead());
    else {
      SoPathList paths;
      paths.append(path);
      action.apply(paths, kind == 2);
    }
    // Tail callbacks remain limited to apply(SoPath *). A path list with
    // one entry still uses PATH_LIST, with either value of obeysrules.
    const int tails = kind == 0 ? 1 : 0;
    if (context.pre != tails || context.post != tails || context.visits != visits) {
      std::fprintf(stderr, "kind=%d pre=%d post=%d visits=%d wanted=%d\n",
                   kind, context.pre, context.post, context.visits, visits);
      ++context.failures;
    }
    failures += context.failures;
  }
  return failures;
}

int
main(int argc, char ** argv)
{
  SoDB::init();
  struct Cleanup {
    ~Cleanup() { SoDB::finish(); }
  } cleanup;
  const bool controls = argc > 1 && std::string(argv[1]) == "path-list";
  int failures = 0;
#ifdef TEST_NODEKITS
  SoNodeKit::init();
#endif
  SoGroup * root = new SoGroup;
  root->ref();
  SoTranslation * shared = new SoTranslation;
  shared->translation.setValue(1, 2, 3);
  // State-affecting left siblings must be visited even when off the path.
  root->addChild(shared);
  root->addChild(shared);
  SoPath * path = new SoPath(root);
  path->ref();
  path->append(1);
  failures += run(path, shared, 2, controls);
  path->unref();

  path = new SoPath(root);
  path->ref();
  failures += run(path, root, 1, controls);
  path->unref();
  root->unref();

  // Also share the leaf through different parents at the same depth.
  root = new SoGroup;
  root->ref();
  SoGroup * left = new SoGroup;
  SoGroup * right = new SoGroup;
  shared = new SoTranslation;
  left->addChild(shared);
  right->addChild(shared);
  root->addChild(left);
  root->addChild(right);
  path = new SoPath(root);
  path->ref();
  path->append(1);
  path->append(0);
  failures += run(path, shared, 2, controls);
  path->unref();
  root->unref();

#ifdef TEST_NODEKITS
  SoShapeKit * kit = new SoShapeKit;
  kit->ref();
  SoCube * shape = new SoCube;
  if (!kit->setPart("shape", shape)) return 1;
  path = kit->createPathToPart("shape", TRUE);
  if (path == NULL) return 1;
  path->ref();
  if (path->getTail() != kit || path->fullPath().getTail() != shape) {
    std::fprintf(stderr, "nodekit fixture did not hide the physical tail\n");
    ++failures;
  }
  failures += run(path, shape, 1, controls);
  path->unref();
  kit->unref();
#endif
  return failures ? 1 : 0;
}
