// Exercise the real GLRender node traversal without requiring a window or GL
// context. Only the element's GL side effects and action GL setup are replaced.
#include "CoinTest.h"
#include "DepthCountingEngine.h"
#include <Inventor/SoDB.h>
#include <Inventor/caches/SoCache.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLDepthBufferElement.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoDepthBuffer.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/sensors/SoFieldSensor.h>

namespace {
class RecordingDepthElement : public SoGLDepthBufferElement {
  SO_ELEMENT_HEADER(RecordingDepthElement);
public:
  static void initClass() {
    SO_ELEMENT_INIT_CLASS(RecordingDepthElement, SoGLDepthBufferElement);
  }
  void init(SoState * state) override { SoDepthBufferElement::init(state); }
  void push(SoState * state) override { SoDepthBufferElement::push(state); }
  void pop(SoState * state, const SoElement * previous) override {
    SoDepthBufferElement::pop(state, previous);
  }
protected:
  void setElt(SbBool test, SbBool write, DepthWriteFunction function,
              SbVec2f range) override {
    SoDepthBufferElement::setElt(test, write, function, range);
  }
};
SO_ELEMENT_SOURCE(RecordingDepthElement);

class RecordingRenderAction : public SoGLRenderAction {
public:
  RecordingRenderAction() : SoGLRenderAction(SbViewportRegion(16, 16)),
                            elements(NULL) {
    elements.enable(SoCacheElement::getClassTypeId(), SoCacheElement::getClassStackIndex());
    elements.enable(RecordingDepthElement::getClassTypeId(),
                    RecordingDepthElement::getClassStackIndex());
  }
protected:
  const SoEnabledElementsList & getEnabledElements() const override {
    return elements;
  }
  void beginTraversal(SoNode * root) override { traverse(root); }
private:
  SoEnabledElementsList elements;
};

struct Depth {
  SbBool test, write;
  SoDepthBufferElement::DepthWriteFunction function;
  SbVec2f range;
  Depth(SbBool t, SbBool w, SoDepthBufferElement::DepthWriteFunction f,
        float nearvalue, float farvalue)
    : test(t), write(w), function(f), range(nearvalue, farvalue) { }
};

struct Probe {
  Depth expected;
  int visits;
  explicit Probe(const Depth & value) : expected(value), visits(0) { }
  static void observe(void * userdata, SoAction * action) {
    Probe * self = static_cast<Probe *>(userdata);
    ++self->visits;
    BOOST_CHECK(action->isOfType(SoGLRenderAction::getClassTypeId()));
    BOOST_CHECK(action->getState()->getConstElement(
      SoDepthBufferElement::getClassStackIndex())->getTypeId() ==
      RecordingDepthElement::getClassTypeId());
    SbBool test, write;
    SoDepthBufferElement::DepthWriteFunction function;
    SbVec2f range;
    SoDepthBufferElement::get(action->getState(), test, write, function, range);
    BOOST_CHECK_EQUAL(test, self->expected.test);
    BOOST_CHECK_EQUAL(write, self->expected.write);
    BOOST_CHECK_EQUAL(function, self->expected.function);
    BOOST_CHECK(range == self->expected.range);
  }
  SoCallback * node() {
    SoCallback * result = new SoCallback;
    result->setCallback(observe, this);
    return result;
  }
};

struct Scene {
  SoGroup * root;
  Scene() : root(new SoGroup) { root->ref(); }
  ~Scene() { root->unref(); }
};

SoDepthBuffer * depth(const Depth & value) {
  SoDepthBuffer * node = new SoDepthBuffer;
  node->test = value.test;
  node->write = value.write;
  node->function = value.function;
  node->range = value.range;
  return node;
}

SoSeparator * branch() {
  SoSeparator * node = new SoSeparator;
  node->renderCaching = SoSeparator::OFF;
  node->renderCulling = SoSeparator::OFF;
  return node;
}

void ignore(SoDepthBuffer * node, int mask) {
  node->test.setIgnored((mask & 1) != 0);
  node->write.setIgnored((mask & 2) != 0);
  node->function.setIgnored((mask & 4) != 0);
  node->range.setIgnored((mask & 8) != 0);
}

void notified(void * userdata, SoSensor *) {
  ++*static_cast<int *>(userdata);
}
}

BOOST_AUTO_TEST_CASE(depth_ignored_fields_inherit_independently) {
  const Depth parent(FALSE, FALSE, SoDepthBufferElement::GREATER, .2f, .7f);
  // Reversed endpoints are valid and must reach the element unchanged.
  const Depth child(TRUE, TRUE, SoDepthBufferElement::NOTEQUAL, .9f, .1f);
  for (int mask = 0; mask != 16; ++mask) {
    Scene scene;
    scene.root->addChild(depth(parent));
    SoDepthBuffer * node = depth(child);
    ignore(node, mask);
    scene.root->addChild(node);
    Probe probe(Depth(mask & 1 ? parent.test : child.test,
                      mask & 2 ? parent.write : child.write,
                      mask & 4 ? parent.function : child.function,
                      mask & 8 ? parent.range[0] : child.range[0],
                      mask & 8 ? parent.range[1] : child.range[1]));
    scene.root->addChild(probe.node());
    RecordingRenderAction action;
    for (int repeat = 0; repeat != 3; ++repeat) action.apply(scene.root);
    BOOST_CHECK_EQUAL(probe.visits, 3);
  }
}

BOOST_AUTO_TEST_CASE(depth_ignored_range_is_read_only) {
  for (int connected = 0; connected != 2; ++connected) {
    for (int isdefault = 0; isdefault != 2; ++isdefault) {
      Scene scene;
      SoDepthBuffer * parent = depth(Depth(FALSE, FALSE,
        SoDepthBufferElement::ALWAYS, .17f, .83f));
      scene.root->addChild(parent);
      SoDepthBuffer * node = new SoDepthBuffer;
      scene.root->addChild(node);
      SoDepthBuffer * source = new SoDepthBuffer;
      source->ref();
      source->range.setValue(.73f, .29f);
      node->range.setValue(.61f, .31f);
      if (connected) node->range.connectFrom(&source->range);
      const SbVec2f stored = node->range.getValue();
      node->range.setIgnored(TRUE);
      node->range.setDefault(isdefault != 0);
      SoField * master = NULL;
      if (connected) BOOST_REQUIRE(node->range.getConnectedField(master));
      int nodeNotifications = 0, fieldNotifications = 0;
      SoNodeSensor nodeSensor(notified, &nodeNotifications);
      SoFieldSensor fieldSensor(notified, &fieldNotifications);
      nodeSensor.setPriority(0);
      fieldSensor.setPriority(0);
      nodeSensor.attach(node);
      fieldSensor.attach(&node->range);
      Probe probe(Depth(TRUE, TRUE, SoDepthBufferElement::LESS, .17f, .83f));
      scene.root->addChild(probe.node());
      RecordingRenderAction action;
      for (int repeat = 0; repeat != 3; ++repeat) {
        action.apply(scene.root);
        BOOST_CHECK(node->range.getValue() == stored);
        BOOST_CHECK(node->range.isIgnored());
        BOOST_CHECK_EQUAL(node->range.isDefault(), isdefault != 0);
        BOOST_CHECK_EQUAL(node->range.isConnected(), connected != 0);
        if (connected) {
          SoField * current = NULL;
          BOOST_CHECK(node->range.getConnectedField(current));
          BOOST_CHECK(current == master);
          BOOST_CHECK(node->range.isConnectionEnabled());
        }
      }
      BOOST_CHECK_EQUAL(probe.visits, 3);
      BOOST_CHECK_EQUAL(nodeNotifications, 0);
      BOOST_CHECK_EQUAL(fieldNotifications, 0);
      nodeSensor.detach();
      fieldSensor.detach();
      // The stored explicit value must still be available after inheritance.
      node->range.setIgnored(FALSE);
      probe.expected.range = stored;
      action.apply(scene.root);
      BOOST_CHECK_EQUAL(probe.visits, 4);
      source->unref();
    }
  }
}

BOOST_AUTO_TEST_CASE(depth_shared_ignored_node_and_separator_restore) {
  Scene scene;
  const Depth outer(TRUE, TRUE, SoDepthBufferElement::LEQUAL, 0, 1);
  const Depth first(FALSE, FALSE, SoDepthBufferElement::NEVER, .12f, .42f);
  const Depth second(TRUE, FALSE, SoDepthBufferElement::EQUAL, .88f, .63f);
  scene.root->addChild(depth(outer));
  SoDepthBuffer * shared = depth(Depth(TRUE, TRUE,
    SoDepthBufferElement::LESS, .34f, .56f));
  ignore(shared, 15);
  Probe firstProbe(first), secondProbe(second), restoredProbe(outer);
  SoSeparator * left = branch();
  scene.root->addChild(left);
  left->addChild(depth(first));
  left->addChild(shared);
  left->addChild(firstProbe.node());
  scene.root->addChild(restoredProbe.node());
  SoSeparator * right = branch();
  scene.root->addChild(right);
  right->addChild(depth(second));
  right->addChild(shared);
  right->addChild(secondProbe.node());
  scene.root->addChild(restoredProbe.node());
  RecordingRenderAction action;
  for (int repeat = 0; repeat != 3; ++repeat) action.apply(scene.root);
  BOOST_CHECK_EQUAL(firstProbe.visits, 3);
  BOOST_CHECK_EQUAL(secondProbe.visits, 3);
  BOOST_CHECK_EQUAL(restoredProbe.visits, 6);
  BOOST_CHECK(shared->range.getValue() == SbVec2f(.34f, .56f));
}

BOOST_AUTO_TEST_CASE(depth_explicit_function_and_range_values) {
  for (int function = SoDepthBuffer::NEVER;
       function <= SoDepthBuffer::NOTEQUAL; ++function) {
    Scene scene;
    const Depth value(function & 1, (function & 2) != 0,
      static_cast<SoDepthBufferElement::DepthWriteFunction>(function),
      function & 1 ? 1.0f : 0.0f, function & 1 ? 0.0f : 1.0f);
    scene.root->addChild(depth(value));
    Probe probe(value);
    scene.root->addChild(probe.node());
    RecordingRenderAction action;
    action.apply(scene.root);
    BOOST_CHECK_EQUAL(probe.visits, 1);
  }
}

BOOST_AUTO_TEST_CASE(depth_raw_ranges_are_preserved) {
  const SbVec2f ranges[] = {SbVec2f(-2.0f, 3.0f), SbVec2f(4.0f, -1.0f),
                           SbVec2f(.45f, .45f)};
  for (unsigned i = 0; i != sizeof(ranges)/sizeof(ranges[0]); ++i) {
    Scene scene;
    const Depth parent(FALSE, TRUE, SoDepthBufferElement::GREATER,
                       ranges[i][0], ranges[i][1]);
    scene.root->addChild(depth(parent));
    Probe explicitProbe(parent);
    scene.root->addChild(explicitProbe.node());
    SoDepthBuffer * inherited = depth(Depth(TRUE, FALSE,
      SoDepthBufferElement::ALWAYS, .25f, .75f));
    inherited->range.setIgnored(TRUE);
    scene.root->addChild(inherited);
    Probe inheritedProbe(Depth(TRUE, FALSE, SoDepthBufferElement::ALWAYS,
                              ranges[i][0], ranges[i][1]));
    scene.root->addChild(inheritedProbe.node());
    RecordingRenderAction action;
    action.apply(scene.root);
    BOOST_CHECK_EQUAL(explicitProbe.visits, 1);
    BOOST_CHECK_EQUAL(inheritedProbe.visits, 1);
    BOOST_CHECK(inherited->range.getValue() == SbVec2f(.25f, .75f));
  }
}

BOOST_AUTO_TEST_CASE(depth_ignored_engines_are_lazy) {
  const Depth inherited(FALSE, FALSE, SoDepthBufferElement::GREATER, -.25f, 1.4f);
  for (int mask = 0; mask != 16; ++mask) {
    Scene scene;
    scene.root->addChild(depth(inherited));
    SoDepthBuffer * node = new SoDepthBuffer;
    scene.root->addChild(node);
    SoField * fields[] = { &node->test, &node->write, &node->function, &node->range };
    DepthCountingEngine * engines[4];
    SoEngineOutput * outputs[4];
    for (int i = 0; i != 4; ++i) {
      engines[i] = new DepthCountingEngine;
      engines[i]->ref();
      outputs[i] = i == 2 ? &engines[i]->function :
                   i == 3 ? &engines[i]->range : &engines[i]->boolean;
      BOOST_REQUIRE(fields[i]->connectFrom(outputs[i]));
    }
    // Settle the connections once. Every phase below dirties them again after
    // changing ignored, so cached connection values cannot hide eager reads.
    node->test.getValue(); node->write.getValue();
    node->function.getValue(); node->range.getValue();
    Probe probe(inherited);
    scene.root->addChild(probe.node());
    RecordingRenderAction action;
    for (int phase = 0; phase != 3; ++phase) {
      const int ignored = phase == 1 ? (mask ^ 15) : mask;
      const Depth explicitValue(phase != 1, phase != 1,
        phase == 1 ? SoDepthBufferElement::NEVER : SoDepthBufferElement::NOTEQUAL,
        phase == 1 ? -.7f : 1.7f, phase == 1 ? 1.3f : -.4f);
      ignore(node, ignored);
      int before[4];
      probe.expected = Depth(ignored & 1 ? inherited.test : explicitValue.test,
        ignored & 2 ? inherited.write : explicitValue.write,
        ignored & 4 ? inherited.function : explicitValue.function,
        ignored & 8 ? inherited.range[0] : explicitValue.range[0],
        ignored & 8 ? inherited.range[1] : explicitValue.range[1]);
      int nodeNotifications = 0, fieldNotifications = 0;
      SoNodeSensor nodeSensor(notified, &nodeNotifications);
      nodeSensor.setPriority(0);
      nodeSensor.attach(node);
      SoFieldSensor sensors[4];
      for (int i = 0; i != 4; ++i) {
        sensors[i].setFunction(notified);
        sensors[i].setData(&fieldNotifications);
        // An immediate field sensor evaluates its field in trigger(). Keep
        // this observer queued so it cannot consume the dirty engine value.
        sensors[i].setPriority(1);
        sensors[i].attach(fields[i]);
      }
      for (int i = 0; i != 4; ++i) {
        engines[i]->booleanValue = i == 0 ? explicitValue.test : explicitValue.write;
        engines[i]->functionValue = explicitValue.function;
        engines[i]->rangeValue = explicitValue.range;
        engines[i]->dirty();
        fields[i]->setDefault(phase & 1);
        before[i] = engines[i]->evaluations;
      }
      for (int i = 0; i != 4; ++i) sensors[i].unschedule();
      nodeNotifications = fieldNotifications = 0;
      action.apply(scene.root);
      action.apply(scene.root);
      for (int i = 0; i != 4; ++i) {
        BOOST_CHECK_EQUAL(engines[i]->evaluations,
                          before[i] + ((ignored & (1 << i)) ? 0 : 1));
        BOOST_CHECK_EQUAL(fields[i]->isIgnored(), (ignored & (1 << i)) != 0);
        // A required engine output normally clears the default flag when it
        // writes its value; an ignored field must preserve the prepared flag.
        BOOST_CHECK_EQUAL(fields[i]->isDefault(),
                          (ignored & (1 << i)) && (phase & 1));
        BOOST_CHECK(fields[i]->isConnected());
        BOOST_CHECK(fields[i]->isConnectionEnabled());
        SoEngineOutput * current = NULL;
        BOOST_CHECK(fields[i]->getConnectedEngine(current));
        BOOST_CHECK(current == outputs[i]);
        BOOST_CHECK(outputs[i]->isEnabled());
        BOOST_CHECK(!sensors[i].isScheduled());
        sensors[i].detach();
      }
      BOOST_CHECK_EQUAL(nodeNotifications, 0);
      BOOST_CHECK_EQUAL(fieldNotifications, 0);
      nodeSensor.detach();
    }
    BOOST_CHECK_EQUAL(probe.visits, 6);
    for (int i = 0; i != 4; ++i) engines[i]->unref();
  }
}


namespace {
struct InheritedCacheProbe {
  SoCache * cache;
  int component, visits;
  explicit InheritedCacheProbe(int c) : cache(NULL), component(c), visits(0) { }
  static void open(void * data, SoAction * action) {
    InheritedCacheProbe * p = static_cast<InheritedCacheProbe *>(data);
    SoState * state = action->getState();
    state->push();
    p->cache = new SoCache(state);
    p->cache->ref();
    SoCacheElement::set(state, p->cache);
  }
  static void closeAndCheck(void * data, SoAction * action) {
    InheritedCacheProbe * p = static_cast<InheritedCacheProbe *>(data);
    SoState * state = action->getState();
    SoCacheElement::set(state, NULL);
    state->pop();
    ++p->visits;
    BOOST_CHECK(p->cache->isValid(state));
    // Change only the inherited component, outside the completed cache.
    SoDepthBufferElement::set(state, p->component == 0 ? TRUE : FALSE,
      p->component == 1 ? TRUE : FALSE,
      p->component == 2 ? SoDepthBufferElement::NEVER : SoDepthBufferElement::GREATER,
      p->component == 3 ? SbVec2f(.7f, -.2f) : SbVec2f(-.25f, 1.4f));
    BOOST_CHECK(!p->cache->isValid(state));
    p->cache->unref(state);
    p->cache = NULL;
  }
  SoCallback * callback(SoCallbackCB * cb) {
    SoCallback * node = new SoCallback;
    node->setCallback(cb, this);
    return node;
  }
};
}

BOOST_AUTO_TEST_CASE(depth_ignored_components_capture_cache_dependencies) {
  for (int component = 0; component != 4; ++component) {
    Scene scene;
    scene.root->addChild(depth(Depth(FALSE, FALSE,
      SoDepthBufferElement::GREATER, -.25f, 1.4f)));
    InheritedCacheProbe probe(component);
    scene.root->addChild(probe.callback(InheritedCacheProbe::open));
    SoDepthBuffer * node = depth(Depth(TRUE, TRUE,
      SoDepthBufferElement::NOTEQUAL, 1.7f, -.4f));
    ignore(node, 1 << component);
    scene.root->addChild(node);
    scene.root->addChild(probe.callback(InheritedCacheProbe::closeAndCheck));
    RecordingRenderAction action;
    for (int repeat = 0; repeat != 3; ++repeat) action.apply(scene.root);
    BOOST_CHECK_EQUAL(probe.visits, 3);
  }
}

int main(int argc, char ** argv) {
  SoDB::init();
  RecordingDepthElement::initClass();
  DepthCountingEngine::initClass();
  const int result = CoinTest::run_all(argc, argv);
  SoDB::finish();
  return result;
}
