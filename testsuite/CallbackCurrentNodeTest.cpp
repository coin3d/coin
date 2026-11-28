#include <Inventor/SoDB.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/lists/SoPathList.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoTranslation.h>

#include <cstdio>
#include <string>

struct NestedException {};

struct Context {
  SoNode * trigger;
  SoNode * nested;
  SoPath * nestedpath;
  std::string mode;
  bool inside;
  int pre;
  int post;
  int failures;
};

static void
check(Context & context, SoCallbackAction * action, const SoNode * node,
      const char * where)
{
  if (action->getCurPathTail() != node) {
    std::fprintf(stderr, "current node differs in %s for %s\n", where,
                 node->getTypeId().getName().getString());
    ++context.failures;
  }
}

static void
reenter(Context & context, SoCallbackAction * action, const SoNode * node)
{
  if (context.inside || node != context.trigger) return;
  context.inside = true;
  SoPathList paths;
  paths.append(context.nestedpath);
  // All three apply overloads reach beginTraversal(). Use a leaf so the
  // nested traversals do not depend on restoring element or path state.
  for (int kind = 0; kind < 3; ++kind) {
    bool caught = false;
    try {
      if (kind == 0) action->apply(context.nested);
      else if (kind == 1) action->apply(context.nestedpath);
      else action->apply(paths);
    }
    catch (const NestedException &) {
      caught = true;
    }
    if (caught != (context.mode == "nested-exception")) ++context.failures;
    check(context, action, node, "after nested apply");
  }
  context.inside = false;
}

static SoCallbackAction::Response
pre(void * userdata, SoCallbackAction * action, const SoNode * node)
{
  Context & context = *static_cast<Context *>(userdata);
  ++context.pre;
  check(context, action, node, "pre");
  if (context.inside && context.mode == "nested-exception") {
    throw NestedException();
  }
  if (context.mode == "nested" || context.mode == "nested-exception") {
    reenter(context, action, node);
  }
  return SoCallbackAction::CONTINUE;
}

static SoCallbackAction::Response
post(void * userdata, SoCallbackAction * action, const SoNode * node)
{
  Context & context = *static_cast<Context *>(userdata);
  ++context.post;
  check(context, action, node, "post");
  if (context.mode == "nested-post") reenter(context, action, node);
  return SoCallbackAction::CONTINUE;
}

int
main(int argc, char ** argv)
{
  SoDB::init();
  struct Cleanup {
    ~Cleanup() { SoDB::finish(); }
  } cleanup;
  const std::string mode = argc > 1 ? argv[1] : "tree";
  SoGroup * root = new SoGroup;
  root->ref();
  SoGroup * child = new SoGroup;
  root->addChild(child);
  child->addChild(new SoTranslation);
  root->addChild(new SoTranslation);
  SoTranslation * nested = new SoTranslation;
  nested->ref();
  SoPath * path = new SoPath(nested);
  path->ref();
  Context context = { child, nested, path, mode, false, 0, 0, 0 };
  SoCallbackAction action;
  action.addPreCallback(SoNode::getClassTypeId(), pre, &context);
  action.addPostCallback(SoNode::getClassTypeId(), post, &context);
  // Reuse also checks that no current-node context leaks across applications.
  for (int i = 0; i < 2; ++i) action.apply(root);
  const int expectedpre = mode == "tree" ? 8 : 14;
  const int expectedpost = mode == "tree" || mode == "nested-exception" ? 8 : 14;
  if (context.pre != expectedpre || context.post != expectedpost) {
    std::fprintf(stderr, "callback counts pre=%d/%d post=%d/%d\n",
                 context.pre, expectedpre, context.post, expectedpost);
    ++context.failures;
  }
  path->unref();
  nested->unref();
  root->unref();
  return context.failures ? 1 : 0;
}
