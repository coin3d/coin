#include <GL/glx.h>
#include <X11/Xlib.h>

// Reuse the public test probes; the offscreen entry point is never called.
#define main annotation_offscreen_entry_unused
#include "SoAnnotationDepthStateTest.cpp"
#undef main

namespace {
struct NativeContext {
  Display *display = XOpenDisplay(nullptr);
  XVisualInfo *visual = nullptr;
  GLXContext context = nullptr;
  Colormap colormap = 0;
  Window window = 0;
  NativeContext() {
    if (!display) return;
    int attributes[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, None};
    visual = glXChooseVisual(display, DefaultScreen(display), attributes);
    if (!visual) return;
    context = glXCreateContext(display, visual, nullptr, True);
    if (!context) return;
    colormap = XCreateColormap(display, RootWindow(display, visual->screen), visual->visual, AllocNone);
    XSetWindowAttributes attrs = {}; attrs.colormap = colormap;
    window = XCreateWindow(display, RootWindow(display, visual->screen), 0, 0,
      side, side, 0, visual->depth, InputOutput, visual->visual, CWColormap, &attrs);
    if (!glXMakeCurrent(display, window, context)) window = 0;
  }
  ~NativeContext() {
    if (context) { glXMakeCurrent(display, None, nullptr); glXDestroyContext(display, context); }
    if (window) XDestroyWindow(display, window);
    if (colormap) XFreeColormap(display, colormap);
    if (visual) XFree(visual);
    if (display) XCloseDisplay(display);
  }
};
void recovery(bool inPath) {
  ReadAction action;
  action.setCacheContext(1882);
  action.setTransparencyType(SoGLRenderAction::NONE);
  Scene scene;
  Stop stop;
  stop.kind = Stop::THROW;
  stop.action = &action;
  auto *ann = stoppingScene(scene, stop);
  QueuePath queue;
  queue.action = &action;
  queue.annotation = ann;
  if (inPath) {
    auto *enqueue = new SoCallback;
    enqueue->setCallback(QueuePath::enqueue, &queue);
    scene.root->insertChild(enqueue, scene.root->findChild(ann));
    queue.path = new SoPath(scene.root);
    queue.path->ref();
    queue.path->append(scene.root->findChild(ann));
    queue.path->append(ann->findChild(stop.target));
    action.setAbortCallback(QueuePath::pruneInitial, &queue);
  }
  const int stack = action.getState()->getDepth();
  const int refs = scene.root->getRefCount();
  for (int iteration = 0; iteration < 3; ++iteration) {
    stop.enabled = true;
    queue.queued = false;
    bool caught = false;
    const unsigned throws = ann->thrownScopes;
    try { action.apply(scene.root); }
    catch (const ChildFailure &) { caught = true; }
    require(caught && ann->thrownScopes == throws + 1,
      "direct action child exception propagates through local annotation boundary");
    require(ann->bodies.empty() && ann->inScopes.empty(), "local scopes balanced after throw");
    require(action.getState()->getDepth() == stack, "global action stack recovered");
    require(!action.isRenderingDelayedPaths() && !action.isRenderingTranspPaths(),
      "global action traversal flags recovered");
    require(scene.root->getRefCount() == refs, "delayed path references recovered");
    const unsigned executed = ann->executions();
    const unsigned visited = stop.after;
    stop.enabled = false;
    queue.queued = false;
    action.apply(scene.root);
    require(ann->executions() == executed + 1, "same action re-enters annotation once after throw");
    // A selected descendant path stops at the callback; its later sibling is
    // excluded by the path, independently of exception recovery.
    require(stop.after == visited + (inPath ? 0 : 1), "reused action respects selected path");
    require(action.getState()->getDepth() == stack && scene.root->getRefCount() == refs,
      "reused action balanced");
    require(ann->bodies.empty() && ann->inScopes.empty(), "reused local scope balanced");
  }
  if (inPath) require(ann->outerIn == 6 && ann->outerBelow == 0,
    "throw and reuse both use actual delayed IN_PATH");
  action.setAbortCallback(nullptr, nullptr);
  if (queue.path) queue.path->unref();
  // Fresh graph on the same action also detects stale delayed-path queues.
  Scene fresh;
  auto *freshAnn = fresh.addAnnotation(fresh.root, Depth(), "fresh graph after throw");
  fresh.probe(freshAnn, Depth(false, true), "fresh body", true);
  action.apply(fresh.root);
  require(freshAnn->executions() == 1, "fresh graph visits only its own delayed path");
  fresh.seen();
}
}
int main(int argc, char **argv) {
  if (argc != 2 || (std::string(argv[1]) != "below" && std::string(argv[1]) != "in")) return 2;
  NativeContext context;
  if (!context.window) return 77;
  SoDB::init();
  recovery(std::string(argv[1]) == "in");
  require(glGetError() == GL_NO_ERROR, "no GL error in direct recovery test");
  return good ? 0 : 1;
}
