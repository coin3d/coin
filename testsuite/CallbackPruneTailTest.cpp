#include <Inventor/SoDB.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoTranslation.h>

#include <cstdio>
#include <vector>

struct Context {
  SoNode * selected;
  SoCallbackAction::Response normalresponse;
  SoCallbackAction::Response tailresponse;
  std::vector<int> events;
  int children;
};

static SoCallbackAction::Response
pre(void * userdata, SoCallbackAction *, const SoNode * node)
{
  Context & context = *static_cast<Context *>(userdata);
  if (node == context.selected) {
    context.events.push_back(1);
    return context.normalresponse;
  }
  ++context.children;
  return SoCallbackAction::CONTINUE;
}

static SoCallbackAction::Response
pretail(void * userdata, SoCallbackAction *, const SoNode *)
{
  Context & context = *static_cast<Context *>(userdata);
  context.events.push_back(2);
  return context.tailresponse;
}

static SoCallbackAction::Response
post(void * userdata, SoCallbackAction *, const SoNode * node)
{
  Context & context = *static_cast<Context *>(userdata);
  if (node == context.selected) context.events.push_back(3);
  return SoCallbackAction::CONTINUE;
}

static SoCallbackAction::Response
posttail(void * userdata, SoCallbackAction *, const SoNode *)
{
  static_cast<Context *>(userdata)->events.push_back(4);
  return SoCallbackAction::CONTINUE;
}

int
main()
{
  SoDB::init();
  struct Cleanup {
    ~Cleanup() { SoDB::finish(); }
  } cleanup;
  SoGroup * root = new SoGroup;
  root->ref();
  root->addChild(new SoTranslation);
  SoPath * path = new SoPath(root);
  path->ref();
  int failures = 0;
  const SoCallbackAction::Response responses[] = {
    SoCallbackAction::CONTINUE, SoCallbackAction::PRUNE, SoCallbackAction::ABORT
  };
  for (int normal = 0; normal < 3; ++normal) {
    for (int tail = 0; tail < 3; ++tail) {
      Context context = { root, responses[normal], responses[tail], {}, 0 };
      SoCallbackAction action;
      action.addPreCallback(SoNode::getClassTypeId(), pre, &context);
      action.addPostCallback(SoNode::getClassTypeId(), post, &context);
      action.addPreTailCallback(pretail, &context);
      action.addPostTailCallback(posttail, &context);
      action.apply(path);

      // ABORT skips pre-tail dispatch. Existing post dispatch is preserved.
      const std::vector<int> expected = normal == 2 ?
        std::vector<int>{1, 3, 4} : std::vector<int>{1, 2, 3, 4};
      const int children = normal == 0 && tail == 0 ? 1 : 0;
      const bool terminated = normal == 2 || tail == 2;
      if (context.events != expected || context.children != children ||
          bool(action.hasTerminated()) != terminated) {
        std::fprintf(stderr, "normal=%d tail=%d children=%d terminated=%d events:",
                     normal, tail, context.children, int(action.hasTerminated()));
        for (int event : context.events) std::fprintf(stderr, " %d", event);
        std::fprintf(stderr, "\n");
        ++failures;
      }
    }
  }
  path->unref();
  root->unref();
  return failures ? 1 : 0;
}
