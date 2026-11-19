// Native Annotation depth state: driver/element agreement and analytic pixels.
// No CoinRender backend, captured GPU image or production sampling is an
// oracle.
#include <Inventor/SoDB.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoDepthBufferElement.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoAnnotation.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoDepthBuffer.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/system/gl.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
const int side = 64;
bool good = true;
bool require(bool value, const std::string &label) {
  if (!value) {
    good = false;
    std::cerr << "annotation_depth FAIL " << label << '\n';
  }
  return value;
}
struct Depth {
  bool test, write;
  SoDepthBufferElement::DepthWriteFunction function;
  float lo, hi;
  Depth(
      bool t = true, bool w = true,
      SoDepthBufferElement::DepthWriteFunction f = SoDepthBufferElement::LEQUAL,
      float a = 0, float b = 1)
      : test(t), write(w), function(f), lo(a), hi(b) {}
};
GLenum glFunction(SoDepthBufferElement::DepthWriteFunction f) {
  switch (f) {
  case SoDepthBufferElement::NEVER:
    return GL_NEVER;
  case SoDepthBufferElement::ALWAYS:
    return GL_ALWAYS;
  case SoDepthBufferElement::LESS:
    return GL_LESS;
  case SoDepthBufferElement::LEQUAL:
    return GL_LEQUAL;
  case SoDepthBufferElement::EQUAL:
    return GL_EQUAL;
  case SoDepthBufferElement::GEQUAL:
    return GL_GEQUAL;
  case SoDepthBufferElement::GREATER:
    return GL_GREATER;
  case SoDepthBufferElement::NOTEQUAL:
    return GL_NOTEQUAL;
  }
  return 0;
}
bool state(SoGLRenderAction *action, const Depth &expected,
           const std::string &label) {
  SbBool test, write;
  SoDepthBufferElement::DepthWriteFunction function;
  SbVec2f range;
  SoDepthBufferElement::get(action->getState(), test, write, function, range);
  GLboolean mask = GL_FALSE;
  GLint comparison = 0;
  GLdouble interval[2] = {-1, -1};
  const bool enabled = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE;
  glGetBooleanv(GL_DEPTH_WRITEMASK, &mask);
  glGetIntegerv(GL_DEPTH_FUNC, &comparison);
  glGetDoublev(GL_DEPTH_RANGE, interval);
  const bool matched =
      bool(test) == expected.test && bool(write) == expected.write &&
      function == expected.function &&
      std::abs(range[0] - expected.lo) < 1e-6 &&
      std::abs(range[1] - expected.hi) < 1e-6 && enabled == bool(test) &&
      (mask == GL_TRUE) == bool(write) &&
      comparison == int(glFunction(function)) &&
      std::abs(interval[0] - range[0]) < 1e-6 &&
      std::abs(interval[1] - range[1]) < 1e-6;
  if (!matched)
    std::cerr << "annotation_state " << label
              << " delayed=" << action->isRenderingDelayedPaths()
              << " transparent=" << action->isRenderingTranspPaths()
              << " element=" << test << ',' << write << ',' << function << ','
              << range[0] << ',' << range[1] << " GL=" << enabled << ','
              << int(mask) << ',' << comparison << ',' << interval[0] << ','
              << interval[1] << " expected=" << expected.test << ','
              << expected.write << ',' << expected.function << ','
              << expected.lo << ',' << expected.hi << '\n';
  return require(matched, label + " element and GL depth state");
}
struct Probe {
  Depth expected;
  std::string label;
  unsigned visits = 0, delayed = 0, transparent = 0;
  bool mustBeDelayed = false, transparentWritePolicy = false;
  bool transparentWrites = false;
  static void callback(void *data, SoAction *base) {
    if (!base->isOfType(SoGLRenderAction::getClassTypeId()))
      return;
    auto &p = *static_cast<Probe *>(data);
    auto *action = static_cast<SoGLRenderAction *>(base);
    ++p.visits;
    p.delayed += action->isRenderingDelayedPaths();
    p.transparent += action->isRenderingTranspPaths();
    Depth effective = p.expected;
    if (p.transparentWritePolicy)
      effective.write = !action->isRenderingTranspPaths() || p.transparentWrites;
    state(action, effective, p.label);
    if (p.mustBeDelayed)
      require(action->isRenderingDelayedPaths(),
              p.label + " only delayed traversal executes annotation children");
  }
};
struct ChildFailure : std::runtime_error {
  ChildFailure() : std::runtime_error("intentional Annotation child failure") {}
};
struct Annotation : SoAnnotation {
  Depth entry;
  std::string label;
  unsigned below = 0, in = 0, off = 0, outerBelow = 0, outerIn = 0,
           forwardedBelow = 0;
  std::vector<Depth> bodies;
  std::vector<bool> inScopes;
  unsigned thrownScopes = 0;
  unsigned executions() const { return outerBelow + outerIn; }
  void renderScope(SoGLRenderAction *action, bool inPath) {
    const bool delayed = action->isRenderingDelayedPaths();
    if (!delayed) {
      if (inPath)
        SoAnnotation::GLRenderInPath(action);
      else
        SoAnnotation::GLRenderBelowPath(action);
      return;
    }
    const bool reentry = !bodies.empty();
    // SoSeparator::GLRenderInPath forwards a BELOW_PATH tail through the
    // virtual GLRenderBelowPath on this same node. The outer Annotation has
    // already disabled testing at this point. This is one child traversal,
    // not two annotations and not a second delayed path.
    const Depth expected = reentry ? bodies.back() : entry;
    if (inPath)
      ++in;
    else
      ++below;
    if (reentry) {
      ++forwardedBelow;
      require(!inPath && inScopes.back() &&
                  action->getCurPathCode() == SoAction::BELOW_PATH,
              label + " only native IN_PATH-to-BELOW_PATH forwarding reenters "
                      "this annotation");
    } else if (inPath)
      ++outerIn;
    else
      ++outerBelow;
    const std::string method = inPath ? " in-path" : " below";
    state(action, expected, label + method + " entry");
    Depth body = expected;
    body.test = false;
    const int entryDepth = action->getState()->getDepth();
    bodies.push_back(body);
    inScopes.push_back(inPath);
    try {
      if (inPath)
        SoAnnotation::GLRenderInPath(action);
      else
        SoAnnotation::GLRenderBelowPath(action);
    } catch (const ChildFailure &) {
      inScopes.pop_back();
      bodies.pop_back();
      // Observe only the outer public Annotation call. The forwarded virtual
      // call is part of this same scope, not a separate recovery boundary.
      if (!reentry) {
        ++thrownScopes;
        require(action->getState()->getDepth() == entryDepth,
                label + method + " restores local stack after child throw");
        state(action, expected, label + method + " restored after child throw");
      }
      throw;
    }
    require(action->getState()->getDepth() == entryDepth,
            label + method + " restores local stack");
    state(action, expected, label + method + " restored");
    inScopes.pop_back();
    bodies.pop_back();
  }
  void GLRenderBelowPath(SoGLRenderAction *action) override {
    renderScope(action, false);
  }
  void GLRenderInPath(SoGLRenderAction *action) override {
    renderScope(action, true);
  }
  void GLRenderOffPath(SoGLRenderAction *action) override {
    ++off;
    state(action, entry, label + " off-path entry");
    SoAnnotation::GLRenderOffPath(action);
    state(action, entry, label + " off-path restored");
  }
};
struct ReadAction : SoGLRenderAction {
  std::vector<float> depth;
  bool identified = false;
  explicit ReadAction() : SoGLRenderAction(SbViewportRegion(side, side)) {}
  using SoGLRenderAction::apply;
  void terminate() { this->setTerminated(TRUE); }
  void apply(SoNode *root) override {
    SoGLRenderAction::apply(root);
    read();
  }
  void apply(SoPath *path) override {
    SoGLRenderAction::apply(path);
    read();
  }
  void read() {
    if (!identified) {
      identified = true;
      std::cout << "annotation_GL vendor=" << glGetString(GL_VENDOR)
                << " renderer=" << glGetString(GL_RENDERER)
                << " version=" << glGetString(GL_VERSION) << '\n';
    }
    depth.resize(side * side);
    glReadPixels(0, 0, side, side, GL_DEPTH_COMPONENT, GL_FLOAT, depth.data());
  }
};
SoDepthBuffer *depthNode(const Depth &value) {
  auto *node = new SoDepthBuffer;
  node->test = value.test;
  node->write = value.write;
  node->function = int(value.function);
  node->range.setValue(value.lo, value.hi);
  return node;
}
SoFaceSet *quad(SoGroup *parent, float z, const SbColor &color,
                float transparency = 0) {
  auto *material = new SoMaterial;
  material->diffuseColor = color;
  material->transparency = transparency;
  parent->addChild(material);
  auto *coordinates = new SoCoordinate3;
  const SbVec3f points[] = {SbVec3f(-.85f, -.85f, z), SbVec3f(.85f, -.85f, z),
                            SbVec3f(.85f, .85f, z), SbVec3f(-.85f, .85f, z)};
  coordinates->point.setValues(0, 4, points);
  parent->addChild(coordinates);
  auto *face = new SoFaceSet;
  face->numVertices = 4;
  parent->addChild(face);
  return face;
}
struct Scene {
  SoSeparator *root = new SoSeparator;
  std::vector<std::unique_ptr<Probe>> probes;
  std::vector<Annotation *> annotations;
  Annotation *annotation = nullptr;
  Scene() {
    root->ref();
    root->renderCaching = SoSeparator::OFF;
    auto *camera = new SoOrthographicCamera;
    camera->position.setValue(0, 0, 5);
    camera->height = 2;
    camera->nearDistance = 1;
    camera->farDistance = 9;
    root->addChild(camera);
    auto *lighting = new SoLightModel;
    lighting->model = SoLightModel::BASE_COLOR;
    root->addChild(lighting);
    root->addChild(depthNode(Depth()));
    quad(root, 1, SbColor(0, 0, 1));
  }
  ~Scene() { root->unref(); }
  Probe *probe(SoGroup *parent, const Depth &expected, const std::string &label,
               bool delayed = false) {
    probes.emplace_back(new Probe);
    auto *p = probes.back().get();
    p->expected = expected;
    p->label = label;
    p->mustBeDelayed = delayed;
    auto *callback = new SoCallback;
    callback->setCallback(Probe::callback, p);
    parent->addChild(callback);
    return p;
  }
  Annotation *addAnnotation(SoGroup *parent, const Depth &entry,
                            const std::string &label) {
    auto *node = new Annotation;
    node->entry = entry;
    node->label = label;
    node->renderCaching = SoSeparator::OFF;
    parent->addChild(node);
    annotations.push_back(node);
    return node;
  }
  void seen() const {
    for (const auto &p : probes)
      require(p->visits > 0, p->label + " callback actually observed");
  }
};
struct Runner {
  ReadAction action;
  SoOffscreenRenderer renderer{SbViewportRegion(side, side)};
  bool first = true;
  Runner() {
    renderer.setGLRenderAction(&action);
    renderer.setComponents(SoOffscreenRenderer::RGB);
    renderer.setBackgroundColor(SbColor(0, 0, 0));
    action.setTransparencyType(SoGLRenderAction::NONE);
    action.setTransparentDelayedObjectRenderType(SoGLRenderAction::ONE_PASS);
  }
  bool render(Scene &scene, const std::string &label, const SbColor &color,
              float depth, SoPath *path = nullptr) {
    const auto source = scene.root->getNodeId();
    std::vector<unsigned> executions, visits;
    for (const auto *annotation : scene.annotations)
      executions.push_back(annotation->executions());
    for (const auto &probe : scene.probes)
      visits.push_back(probe->visits);
    const bool rendered =
        path ? renderer.render(path) : renderer.render(scene.root);
    require(scene.root->getNodeId() == source,
            label + " rendering preserves source revision");
    if (!rendered && first) {
      std::cerr << "annotation_depth GL context unavailable\n";
      return false;
    }
    first = false;
    if (!require(rendered, label + " native render"))
      return true;
    for (size_t i = 0; i < scene.annotations.size(); ++i) {
      const auto *annotation = scene.annotations[i];
      std::cout << "annotation_dispatch " << label
                << " node=" << annotation->label << " external_delta="
                << (annotation->executions() - executions[i])
                << " below_total=" << annotation->below
                << " in_total=" << annotation->in
                << " forwarded_total=" << annotation->forwardedBelow
                << " open_scopes=" << annotation->bodies.size() << '\n';
      require(
          annotation->executions() == executions[i] + 1 &&
              annotation->bodies.empty() && annotation->inScopes.empty(),
          label +
              " one balanced delayed annotation scope per authored occurrence");
    }
    for (size_t i = 0; i < scene.probes.size(); ++i)
      if (scene.probes[i]->mustBeDelayed)
        require(scene.probes[i]->visits == visits[i] + 1,
                label + " delayed child callback executes exactly once");
    const auto *rgb = renderer.getBuffer();
    unsigned checked = 0, bad = 0;
    require(rgb && action.depth.size() == size_t(side * side),
            label + " real RGB and depth readback");
    if (rgb && action.depth.size() == size_t(side * side))
      for (int y = 20; y < 44; ++y)
        for (int x = 20; x < 44; ++x) {
          const size_t p = size_t(y * side + x);
          ++checked;
          for (int c = 0; c < 3; ++c)
            bad += std::abs(int(rgb[3 * p + c]) -
                            int(std::lround(255 * color[c]))) > 3;
          bad += !std::isfinite(action.depth[p]) ||
                 std::abs(action.depth[p] - depth) > 1e-5;
        }
    std::cout << "annotation_pixels " << label << " interior=" << checked
              << " bad=" << bad << " expected_depth=" << depth << '\n';
    require(checked == 576 && bad == 0,
            label + " independent interior RGB3/depth");
    scene.seen();
    return true;
  }
};
// The camera maps z=1,0,2 to (5-z-1)/(9-1)=.375,.5,.25.
// GL depth testing disabled never writes depth, even with depthMask=true.
bool fields(Runner &runner) {
  for (int variant = 0; variant < 14; ++variant) {
    Scene scene;
    Depth outer, body(false, true), effective;
    bool child = variant != 0;
    SbColor color(0, 0, 1);
    float resultDepth = .375f;
    bool foreground = false, equal = false;
    if (variant == 0 || variant == 3 || variant == 6) {
      color = SbColor(1, 0, 0);
      effective.test = false;
    }
    if (variant == 2)
      effective.function = SoDepthBufferElement::LESS;
    if (variant == 3)
      effective.test = false;
    if (variant == 4 || variant == 5 || variant == 7 || variant == 10 ||
        variant == 11) {
      foreground = true;
      color = SbColor(0, 1, 0);
    }
    if (variant == 4 || variant == 7)
      effective.write = false;
    if (variant == 5)
      resultDepth = .25f;
    if (variant == 7) {
      outer.write = false;
      body.write = false;
    }
    if (variant == 8) {
      outer.function = SoDepthBufferElement::LESS;
      body.function = outer.function;
      effective.function = outer.function;
      equal = true;
    }
    if (variant == 9) {
      equal = true;
      color = SbColor(1, 0, 0);
    }
    if (variant == 10) {
      effective.lo = .1f;
      effective.hi = .9f;
      resultDepth = .3f;
    }
    if (variant == 11) {
      outer.lo = .2f;
      outer.hi = .8f;
      body.lo = outer.lo;
      body.hi = outer.hi;
      effective.lo = outer.lo;
      effective.hi = outer.hi;
      resultDepth = .35f;
    }
    if (variant == 12)
      outer.test = false;
    if (variant == 13) {
      outer = Depth(false, false, SoDepthBufferElement::GREATER, .2f, .8f);
    }
    body = outer;
    body.test = false;
    scene.root->addChild(depthNode(outer));
    scene.probe(scene.root, outer, "outer before");
    auto *ann = scene.addAnnotation(scene.root, outer,
                                    "fields " + std::to_string(variant));
    scene.probe(ann, body, "default annotation body", true);
    if (child) {
      auto *node = depthNode(effective);
      if (variant == 6) {
        node->test = TRUE;
        node->test.setIgnored(TRUE);
      }
      if (variant == 7) {
        node->write = TRUE;
        node->write.setIgnored(TRUE);
      }
      if (variant == 8) {
        node->function = SoDepthBuffer::LEQUAL;
        node->function.setIgnored(TRUE);
      }
      if (variant == 11) {
        node->range.setValue(0, 1);
        node->range.setIgnored(TRUE);
      }
      ann->addChild(node);
      scene.probe(ann, effective, "explicit/ignored child depth", true);
    }
    if (foreground)
      quad(ann, 2, SbColor(0, 1, 0));
    quad(ann, equal ? 1 : 0, SbColor(1, 0, 0));
    scene.probe(scene.root, outer, "ordinary sibling after annotation");
    if (!runner.render(scene, "fields=" + std::to_string(variant), color,
                       resultDepth))
      return false;
    require(ann->executions() == 1 && ann->below == 1 && ann->in <= 1,
            "ordinary delayed annotation reaches BELOW_PATH once, possibly "
            "forwarded through IN_PATH");
  }
  return true;
}
bool nested(Runner &runner) {
  Scene scene;
  Depth initial;
  auto *outer = scene.addAnnotation(scene.root, initial, "outer nested");
  scene.probe(outer, Depth(false, true), "outer default", true);
  const Depth parent(true, false, SoDepthBufferElement::LESS, .2f, .8f);
  outer->addChild(depthNode(parent));
  scene.probe(outer, parent, "outer explicit", true);
  auto *inner = scene.addAnnotation(outer, parent, "inner nested");
  scene.probe(inner, Depth(false, false, SoDepthBufferElement::LESS, .2f, .8f),
              "inner default", true);
  const Depth local(true, true, SoDepthBufferElement::GEQUAL, .1f, .9f);
  inner->addChild(depthNode(local));
  scene.probe(inner, local, "inner explicit", true);
  quad(inner, 0, SbColor(1, 0, 0));
  scene.probe(outer, parent, "outer after inner", true);
  quad(outer, 2, SbColor(0, 1, 0));
  scene.probe(scene.root, initial, "root after outer");
  if (!runner.render(scene, "nested independent depth scopes", SbColor(0, 1, 0),
                     .5f))
    return false;
  require(outer->below > 0 && inner->below > 0,
          "both annotations traverse delayed BELOW_PATH");
  Scene siblings;
  auto *first = siblings.addAnnotation(siblings.root, Depth(), "first sibling");
  first->addChild(
      depthNode(Depth(true, false, SoDepthBufferElement::GEQUAL, .2f, .8f)));
  quad(first, 2, SbColor(0, 1, 0));
  auto *second =
      siblings.addAnnotation(siblings.root, Depth(), "second sibling");
  siblings.probe(second, Depth(false, true),
                 "second sibling inherits original state", true);
  auto *inherited = depthNode(Depth());
  inherited->write.setIgnored(TRUE);
  inherited->function.setIgnored(TRUE);
  inherited->range.setIgnored(TRUE);
  second->addChild(inherited);
  siblings.probe(second, Depth(),
                 "second sibling explicit test with inherited remaining fields",
                 true);
  quad(second, 0, SbColor(1, 0, 0));
  if (!runner.render(siblings, "ordered delayed annotation siblings",
                     SbColor(0, 0, 1), .375f))
    return false;
  require(first->below > 0 && second->below > 0,
          "both sibling delayed occurrences execute");
  return true;
}
struct QueuePath {
  SoPath *path = nullptr;
  Annotation *annotation = nullptr;
  ReadAction *action = nullptr;
  bool queued = false;
  unsigned enqueues = 0, normalVisits = 0, delayedVisits = 0, pruned = 0;
  static void enqueue(void *data, SoAction *base) {
    if (!base->isOfType(SoGLRenderAction::getClassTypeId()))
      return;
    auto &q = *static_cast<QueuePath *>(data);
    if (q.action->isRenderingDelayedPaths())
      ++q.delayedVisits;
    else
      ++q.normalVisits;
    if (!q.action->isRenderingDelayedPaths() && !q.queued) {
      q.action->addDelayedPath(q.path->copy());
      q.queued = true;
      ++q.enqueues;
    }
  }
  static SoGLRenderAction::AbortCode pruneInitial(void *data) {
    auto &q = *static_cast<QueuePath *>(data);
    if (!q.action->isRenderingDelayedPaths() &&
        q.action->getCurPath()->getTail() == q.annotation) {
      ++q.pruned;
      return SoGLRenderAction::PRUNE;
    }
    return SoGLRenderAction::CONTINUE;
  }
};
struct Excluded {
  Annotation *annotation = nullptr;
  unsigned body = 0, invocations = 0;
  ~Excluded() {
    if (annotation)
      annotation->unref();
  }
  static void bodyCallback(void *data, SoAction *action) {
    if (action->isOfType(SoGLRenderAction::getClassTypeId()))
      ++static_cast<Excluded *>(data)->body;
  }
  static void callback(void *data, SoAction *base) {
    if (!base->isOfType(SoGLRenderAction::getClassTypeId()))
      return;
    auto &e = *static_cast<Excluded *>(data);
    auto *action = static_cast<SoGLRenderAction *>(base);
    ++e.invocations;
    state(action, Depth(), "excluded entry");
    e.annotation->GLRenderOffPath(action);
    state(action, Depth(), "OFF_PATH preserves state");
  }
};
bool paths(Runner &runner) {
  {
    Scene scene;
    auto *ann = scene.addAnnotation(scene.root, Depth(), "public path");
    scene.probe(ann, Depth(false, true), "path annotation body", true);
    ann->addChild(depthNode(Depth()));
    scene.probe(ann, Depth(), "path explicit test", true);
    // Applying a descendant path omits ordinary geometry siblings. Establish
    // the analytic occluder inside the selected Annotation path itself.
    quad(ann, 1, SbColor(0, 0, 1));
    auto *face = quad(ann, 0, SbColor(1, 0, 0));
    SoPath *path = new SoPath(scene.root);
    path->ref();
    path->append(scene.root->findChild(ann));
    path->append(ann->findChild(face));
    const bool rendered = runner.render(scene, "public descendant SoPath",
                                        SbColor(0, 0, 1), .375f, path);
    path->unref();
    if (!rendered)
      return false;
    require(ann->below + ann->in > 0,
            "public path actually reaches delayed Annotation");
  }
  {
    // A full public delayed path reaches Annotation IN_PATH. Prune its normal
    // occurrence only; otherwise Annotation would enqueue a second prefix path.
    Scene scene;
    QueuePath queue;
    queue.action = &runner.action;
    auto *enqueue = new SoCallback;
    enqueue->setCallback(QueuePath::enqueue, &queue);
    scene.root->addChild(enqueue);
    auto *ann =
        scene.addAnnotation(scene.root, Depth(), "queued descendant path");
    queue.annotation = ann;
    scene.probe(ann, Depth(false, true), "queued IN_PATH annotation body",
                true);
    ann->addChild(depthNode(Depth()));
    scene.probe(ann, Depth(), "queued IN_PATH explicit test", true);
    auto *face = quad(ann, 0, SbColor(1, 0, 0));
    queue.path = new SoPath(scene.root);
    queue.path->ref();
    queue.path->append(scene.root->findChild(ann));
    queue.path->append(ann->findChild(face));
    runner.action.setAbortCallback(QueuePath::pruneInitial, &queue);
    const bool rendered = runner.render(scene, "delayed full descendant path",
                                        SbColor(0, 0, 1), .375f);
    runner.action.setAbortCallback(nullptr, nullptr);
    queue.path->unref();
    if (!rendered)
      return false;
    std::cout << "annotation_prefix enqueued=" << queue.enqueues
              << " pruned=" << queue.pruned << " normal=" << queue.normalVisits
              << " delayed=" << queue.delayedVisits << '\n';
    require(queue.queued && queue.enqueues == 1 && queue.pruned == 1 &&
                queue.normalVisits == 1 && queue.delayedVisits == 1 &&
                ann->outerIn == 1 && ann->outerBelow == 0 && ann->in == 1 &&
                ann->below == 0,
            "one normal prefix, one queued full path and one delayed IN_PATH "
            "child traversal");
  }
  {
    Scene scene;
    Excluded excluded;
    excluded.annotation = new Annotation;
    excluded.annotation->ref();
    excluded.annotation->label = "excluded annotation";
    auto *body = new SoCallback;
    body->setCallback(Excluded::bodyCallback, &excluded);
    excluded.annotation->addChild(body);
    excluded.annotation->addChild(
        depthNode(Depth(false, false, SoDepthBufferElement::ALWAYS, .1f, .2f)));
    quad(excluded.annotation, 2, SbColor(1, 0, 0));
    auto *callback = new SoCallback;
    callback->setCallback(Excluded::callback, &excluded);
    scene.root->addChild(callback);
    if (!runner.render(scene, "OFF_PATH exclusion",
                       SbColor(0, 0, 1), .375f))
      return false;
    require(excluded.invocations == 1 && excluded.body == 0 &&
                excluded.annotation->off == 1,
            "excluded paths neither traverse children nor publish geometry");
  }
  return true;
}
struct CacheWitness {
  unsigned visits = 0, compilations = 0;
  static void callback(void *data, SoAction *base) {
    if (!base->isOfType(SoGLRenderAction::getClassTypeId()))
      return;
    auto &w = *static_cast<CacheWitness *>(data);
    ++w.visits;
    GLint list = 0;
    glGetIntegerv(GL_LIST_INDEX, &list);
    // Diagnostic only: no comparison of driver depth with the element while
    // GL_COMPILE records commands. A replay skips this CPU callback entirely.
    w.compilations += list != 0;
  }
};
bool cachePolicy(Runner &runner) {
  // Exercise a child list, then Annotation's own ON and AUTO lists. Root is
  // OFF so entry/exit state probes are outside every compiled list.
  for (int location = 0; location < 3; ++location) {
    Scene scene;
    CacheWitness witness;
    auto *ann = scene.addAnnotation(scene.root, Depth(), "cache annotation");
    ann->renderCaching = location == 0   ? SoSeparator::OFF
                         : location == 1 ? SoSeparator::ON
                                         : SoSeparator::AUTO;
    auto *cached = new SoSeparator;
    cached->renderCaching = location == 0 ? SoSeparator::ON : SoSeparator::OFF;
    ann->addChild(cached);
    auto *node = depthNode(Depth());
    cached->addChild(node);
    auto *callback = new SoCallback;
    callback->setCallback(CacheWitness::callback, &witness);
    cached->addChild(callback);
    quad(cached, 0, SbColor(1, 0, 0));
    scene.probe(scene.root, Depth(), "ordinary state outside cache");
    unsigned replays = 0;
    // Eight scheduled renders cover Coin's AUTO eligibility period. Every
    // one must pass; these are cache lifecycle inputs, not presentation
    // retries.
    for (int frame = 0; frame < 8; ++frame) {
      const unsigned before = witness.visits;
      if (!runner.render(scene,
                         "cache location=" + std::to_string(location) +
                             " frame=" + std::to_string(frame),
                         SbColor(0, 0, 1), .375f))
        return false;
      replays += witness.compilations > 0 && witness.visits == before;
    }
    std::cout << "annotation_cache location=" << location
              << " compiled=" << witness.compilations << " replayed=" << replays
              << '\n';
    // AUTO may legitimately decline to cache this small scene. Explicit ON
    // must compile/replay; if AUTO does compile, its replay is checked too.
    require((location == 2 && witness.compilations == 0) ||
                (witness.compilations > 0 && replays > 0),
            "ON compiles/replays; AUTO replays any list it elects to create");
    const auto before = node->getNodeId();
    node->test = FALSE;
    require(node->getNodeId() != before,
            "depth-field mutation changes source revision");
    if (!runner.render(scene, "cached test FALSE mutation", SbColor(1, 0, 0),
                       .375f))
      return false;
    node->test = TRUE;
    if (!runner.render(scene, "cached explicit TRUE restored", SbColor(0, 0, 1),
                       .375f))
      return false;
  }
  // Action-owned writes in transparent replay and Annotation's later delayed
  // pass are distinct phases. Explicit depth in the transparent sibling cannot
  // override replay's write=false; in Annotation it remains effective.
  for (int sorted = 0; sorted < 2; ++sorted) {
    for (int writes = 0; writes < 2; ++writes) {
      Scene scene;
      auto *transparent = new SoSeparator;
      transparent->renderCaching = SoSeparator::OFF;
      scene.root->addChild(transparent);
      transparent->addChild(depthNode(Depth()));
      auto *probe = scene.probe(transparent, Depth(),
                                "transparent write belongs to GLRenderAction");
      probe->transparentWritePolicy = true;
      probe->transparentWrites = writes != 0;
      quad(transparent, 2, SbColor(0, 1, 0), .5f);
      auto *ann = scene.addAnnotation(scene.root, Depth(), "transparent annotation");
      scene.probe(ann, Depth(false, true), "delayed annotation after transparency", true);
      ann->addChild(depthNode(Depth()));
      scene.probe(ann, Depth(), "annotation explicit depth is local", true);
      quad(ann, 2, SbColor(1, 0, 0), .5f);
      runner.action.setTransparencyType(sorted ? SoGLRenderAction::SORTED_OBJECT_BLEND
                                              : SoGLRenderAction::DELAYED_BLEND);
      runner.action.setDelayedObjDepthWrite(writes != 0);
      const bool rendered = runner.render(
          scene, "native transparent policy sorted=" + std::to_string(sorted) +
                 " writes=" + std::to_string(writes),
          SbColor(.5f, .25f, .25f), .25f);
      runner.action.setDelayedObjDepthWrite(FALSE);
      runner.action.setTransparencyType(SoGLRenderAction::NONE);
      if (!rendered) return false;
      require(probe->transparent > 0,
              "write policy observed inside a real transparent path");
    }
  }
  return true;
}

struct Stop {
  enum Kind { TERMINATE, ABORT, THROW } kind = TERMINATE;
  SoGLRenderAction *action = nullptr;
  SoNode *target = nullptr;
  bool enabled = true;
  unsigned stopped = 0, after = 0;
  static void callback(void *data, SoAction *base) {
    if (!base->isOfType(SoGLRenderAction::getClassTypeId())) return;
    auto &s = *static_cast<Stop *>(data);
    auto *action = static_cast<SoGLRenderAction *>(base);
    if (!s.enabled || !action->isRenderingDelayedPaths()) return;
    ++s.stopped;
    if (s.kind == THROW) throw ChildFailure();
    if (s.kind == TERMINATE) static_cast<ReadAction *>(action)->terminate();
  }
  static void afterCallback(void *data, SoAction *base) {
    if (base->isOfType(SoGLRenderAction::getClassTypeId()))
      ++static_cast<Stop *>(data)->after;
  }
  static SoGLRenderAction::AbortCode abortCallback(void *data) {
    auto &s = *static_cast<Stop *>(data);
    if (s.enabled && s.action->isRenderingDelayedPaths() &&
        s.action->getCurPath()->getTail() == s.target) {
      ++s.stopped;
      return SoGLRenderAction::ABORT;
    }
    return SoGLRenderAction::CONTINUE;
  }
};
Annotation *stoppingScene(Scene &scene, Stop &stop) {
  const Depth inherited(true, false, SoDepthBufferElement::GREATER, .2f, .8f);
  scene.root->addChild(depthNode(inherited));
  auto *ann = scene.addAnnotation(scene.root, inherited, "interrupted annotation");
  scene.probe(ann, Depth(false, false, SoDepthBufferElement::GREATER, .2f, .8f),
              "interrupted body inherits nondefault tuple", true);
  ann->addChild(depthNode(Depth(true, true, SoDepthBufferElement::ALWAYS, .1f, .9f)));
  auto *callback = new SoCallback;
  callback->setCallback(Stop::callback, &stop);
  ann->addChild(callback);
  stop.target = callback;
  auto *after = new SoCallback;
  after->setCallback(Stop::afterCallback, &stop);
  ann->addChild(after);
  quad(ann, 0, SbColor(1, 0, 0));
  return ann;
}
bool interrupted(Runner &runner) {
  for (int kind = 0; kind < 2; ++kind) {
    Scene scene;
    Stop stop;
    stop.kind = kind == 0 ? Stop::TERMINATE : Stop::ABORT;
    stop.action = &runner.action;
    auto *ann = stoppingScene(scene, stop);
    if (stop.kind == Stop::ABORT)
      runner.action.setAbortCallback(Stop::abortCallback, &stop);
    const std::string label = kind == 0 ? "child setTerminated" : "abort callback ABORT";
    const bool rendered = runner.render(scene, label, SbColor(0, 0, 1), .375f);
    runner.action.setAbortCallback(nullptr, nullptr);
    if (!rendered) return false;
    require(stop.stopped == 1 && stop.after == 0 && ann->executions() == 1,
            label + " stops delayed children and returns through Annotation");
    stop.enabled = false;
    if (!runner.render(scene, label + " subsequent render", SbColor(1, 0, 0), .5f))
      return false;
    require(stop.after == 1 && ann->executions() == 2,
            label + " subsequent ordinary render is not terminated");
  }
  return true;
}
// Run this mode in its own process. #782/#788 cover global action/path and
// separator recovery; this test observes only Annotation's local boundary.
// Exit after the observation, avoiding any claim that an unwound offscreen
// renderer or the whole action is safe to reuse without those dependencies.
bool throwing(Runner &runner, bool inPath) {
  Scene scene;
  Stop stop;
  stop.kind = Stop::THROW;
  stop.action = &runner.action;
  auto *ann = stoppingScene(scene, stop);
  QueuePath queue;
  SoPath *path = nullptr;
  if (inPath) {
    // Queue the full descendant path, rather than Annotation's own prefix.
    // This reaches its real IN_PATH dispatch during the delayed traversal.
    auto *enqueue = new SoCallback;
    queue.action = &runner.action;
    queue.annotation = ann;
    enqueue->setCallback(QueuePath::enqueue, &queue);
    scene.root->insertChild(enqueue, scene.root->findChild(ann));
    path = new SoPath(scene.root);
    path->ref();
    path->append(scene.root->findChild(ann));
    path->append(ann->findChild(stop.target));
    queue.path = path;
    runner.action.setAbortCallback(QueuePath::pruneInitial, &queue);
  }
  try {
    if (!runner.renderer.render(scene.root)) return false;
  } catch (const ChildFailure &) {
    require(stop.stopped == 1 && stop.after == 0 && ann->thrownScopes == 1 &&
                ann->bodies.empty() && ann->inScopes.empty(),
            "child throw propagated through exactly one outer Annotation scope");
    if (inPath)
      require(ann->outerIn == 1 && ann->outerBelow == 0,
              "child throw observed through actual delayed IN_PATH");
    else
      require(ann->below == 1,
              "child throw observed through delayed BELOW_PATH or native forwarding");
    std::cout << "annotation_throw local scope observed; global recovery outside test scope\n";
    std::cout.flush();
    std::cerr.flush();
    std::_Exit(good ? 0 : 1);
  }
  require(false, "intentional child exception must reach test boundary");
  if (path) path->unref();
  return true;
}
} // namespace
int main(int argc, char **argv) {
  const std::string mode = argc == 2 ? argv[1] : "--all";
  if (argc > 2 ||
      (mode != "--all" && mode != "--fields" && mode != "--nested" &&
       mode != "--paths" && mode != "--cache-policy" &&
       mode != "--interrupt" && mode != "--throw-below" && mode != "--throw-in"))
    return 2;
#ifndef _WIN32
  setenv("COIN_GLX_PIXMAP_DIRECT_RENDERING", "1", 0);
#endif
  SoDB::init();
  Runner runner;
  bool available = true;
  if (mode == "--all" || mode == "--fields")
    available = fields(runner);
  if (available && (mode == "--all" || mode == "--nested"))
    available = nested(runner);
  if (available && (mode == "--all" || mode == "--paths"))
    available = paths(runner);
  if (available && (mode == "--all" || mode == "--cache-policy"))
    available = cachePolicy(runner);
  if (available && (mode == "--all" || mode == "--interrupt"))
    available = interrupted(runner);
  if (available && (mode == "--throw-below" || mode == "--throw-in"))
    available = throwing(runner, mode == "--throw-in");
  return available ? (good ? 0 : 1) : 77;
}
