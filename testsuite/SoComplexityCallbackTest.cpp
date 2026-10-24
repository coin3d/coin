#include "CoinTest.h"

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoTextureOverrideElement.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoInfo.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/sensors/SoNodeSensor.h>

#include <vector>

namespace {
struct ComplexitySample {
  float quality;
  float value;
  SoComplexity::Type type;
  SbBool qualityoverride;
  SbBool valueoverride;
  SbBool typeoverride;
};

struct ComplexityScene {
  SoGroup * root;
  SoCallbackAction action;
  std::vector<ComplexitySample> samples;

  ComplexityScene() : root(new SoGroup) {
    root->ref();
    action.addPreCallback(SoInfo::getClassTypeId(), record, this);
  }
  ~ComplexityScene() { root->unref(); }

  static SoCallbackAction::Response record(void * data,
                                          SoCallbackAction * action,
                                          const SoNode *) {
    ComplexityScene * scene = static_cast<ComplexityScene *>(data);
    SoState * state = action->getState();
    ComplexitySample sample = {
      SoTextureQualityElement::get(state), action->getComplexity(),
      action->getComplexityType(),
      SoTextureOverrideElement::getQualityOverride(state),
      SoOverrideElement::getComplexityOverride(state),
      SoOverrideElement::getComplexityTypeOverride(state)
    };
    scene->samples.push_back(sample);
    return SoCallbackAction::CONTINUE;
  }
  void run() { samples.clear(); action.apply(root); }
};

SoComplexity * addcomplexity(SoGroup * group, float quality,
                             SbBool override = FALSE) {
  SoComplexity * node = new SoComplexity;
  node->textureQuality = quality;
  node->setOverride(override);
  group->addChild(node);
  return node;
}

void mark(SoGroup * group) { group->addChild(new SoInfo); }

// Exercise normal virtual dispatch through an application subclass, too.
class CallbackComplexity : public SoComplexity {
public:
  int calls = 0;
  void callback(SoCallbackAction * action) override {
    ++calls;
    SoComplexity::callback(action);
  }
protected:
  ~CallbackComplexity() override { }
};

void notified(void * data, SoSensor *) {
  ++*static_cast<int *>(data);
}
}

BOOST_AUTO_TEST_SUITE(SoComplexityCallback_TestSuite)

BOOST_AUTO_TEST_CASE(normal_quality_follows_order_including_boundaries_and_default)
{
  ComplexityScene scene;
  mark(scene.root);
  const float qualities[] = { 0.0f, 1.0f, 0.25f };
  for (unsigned int i = 0; i < 3; ++i) {
    addcomplexity(scene.root, qualities[i]);
    mark(scene.root);
  }
  SoComplexity * defaults = new SoComplexity;
  BOOST_CHECK(defaults->textureQuality.isDefault());
  scene.root->addChild(defaults);
  mark(scene.root);
  scene.run();
  BOOST_REQUIRE_EQUAL(scene.samples.size(), 5);
  BOOST_CHECK_EQUAL(scene.samples[0].quality, 0.5f);
  for (unsigned int i = 0; i < 3; ++i) {
    BOOST_CHECK_EQUAL(scene.samples[i + 1].quality, qualities[i]);
    BOOST_CHECK(!scene.samples[i + 1].qualityoverride);
  }
  BOOST_CHECK_EQUAL(scene.samples[4].quality, 0.5f);
  BOOST_CHECK(!scene.samples[4].qualityoverride);
}

BOOST_AUTO_TEST_CASE(first_quality_override_blocks_normal_and_later_override)
{
  ComplexityScene scene;
  addcomplexity(scene.root, 0.0f, TRUE);
  mark(scene.root);
  addcomplexity(scene.root, 1.0f);
  mark(scene.root);
  addcomplexity(scene.root, 0.75f, TRUE);
  mark(scene.root);
  scene.run();
  BOOST_REQUIRE_EQUAL(scene.samples.size(), 3);
  for (unsigned int i = 0; i < 3; ++i) {
    BOOST_CHECK_EQUAL(scene.samples[i].quality, 0.0f);
    BOOST_CHECK(scene.samples[i].qualityoverride);
  }
}

BOOST_AUTO_TEST_CASE(default_quality_can_establish_override)
{
  ComplexityScene scene;
  SoComplexity * defaults = new SoComplexity;
  defaults->setOverride(TRUE);
  scene.root->addChild(defaults);
  addcomplexity(scene.root, 1.0f);
  mark(scene.root);
  scene.run();
  BOOST_REQUIRE_EQUAL(scene.samples.size(), 1);
  BOOST_CHECK_EQUAL(scene.samples[0].quality, 0.5f);
  BOOST_CHECK(scene.samples[0].qualityoverride);
}

BOOST_AUTO_TEST_CASE(ignored_quality_does_not_write_or_establish_override)
{
  ComplexityScene scene;
  addcomplexity(scene.root, 0.25f);
  SoComplexity * ignored = addcomplexity(scene.root, 0.75f, TRUE);
  ignored->textureQuality.setIgnored(TRUE);
  mark(scene.root);
  addcomplexity(scene.root, 1.0f);
  mark(scene.root);
  scene.run();
  BOOST_REQUIRE_EQUAL(scene.samples.size(), 2);
  BOOST_CHECK_EQUAL(scene.samples[0].quality, 0.25f);
  BOOST_CHECK(!scene.samples[0].qualityoverride);
  BOOST_CHECK_EQUAL(scene.samples[1].quality, 1.0f);
  BOOST_CHECK(!scene.samples[1].qualityoverride);
  // The ignored texture field must not disable the independent overrides.
  BOOST_CHECK(scene.samples[0].valueoverride);
  BOOST_CHECK(scene.samples[0].typeoverride);
}

BOOST_AUTO_TEST_CASE(quality_override_is_independent_of_value_and_type_overrides)
{
  // Establish just one override at a time, then let a second overriding node
  // set the other two. Each field must consult its own flag.
  for (int selected = 0; selected < 3; ++selected) {
    ComplexityScene scene;
    SoComplexity * first = addcomplexity(scene.root, 0.25f, TRUE);
    first->value = 0.125f;
    first->type = SoComplexity::SCREEN_SPACE;
    first->textureQuality.setIgnored(selected != 0);
    first->value.setIgnored(selected != 1);
    first->type.setIgnored(selected != 2);
    SoComplexity * second = addcomplexity(scene.root, 0.75f, TRUE);
    second->value = 0.875f;
    second->type = SoComplexity::BOUNDING_BOX;
    mark(scene.root);
    SoComplexity * third = addcomplexity(scene.root, 1.0f);
    third->value = 0.5f;
    third->type = SoComplexity::OBJECT_SPACE;
    mark(scene.root);
    scene.run();
    BOOST_REQUIRE_EQUAL(scene.samples.size(), 2);
    for (unsigned int i = 0; i < 2; ++i) {
      BOOST_CHECK_EQUAL(scene.samples[i].quality, selected == 0 ? 0.25f : 0.75f);
      BOOST_CHECK_EQUAL(scene.samples[i].value, selected == 1 ? 0.125f : 0.875f);
      BOOST_CHECK_EQUAL(scene.samples[i].type,
                        selected == 2 ? SoComplexity::SCREEN_SPACE : SoComplexity::BOUNDING_BOX);
      BOOST_CHECK(scene.samples[i].qualityoverride);
      BOOST_CHECK(scene.samples[i].valueoverride);
      BOOST_CHECK(scene.samples[i].typeoverride);
    }
  }
}

BOOST_AUTO_TEST_CASE(separator_restores_quality_and_override_state)
{
  ComplexityScene scene;
  addcomplexity(scene.root, 0.25f);
  SoSeparator * local = new SoSeparator;
  scene.root->addChild(local);
  addcomplexity(local, 1.0f, TRUE);
  addcomplexity(local, 0.0f);
  mark(local);
  mark(scene.root);
  addcomplexity(scene.root, 0.75f);
  mark(scene.root);
  scene.run();
  BOOST_REQUIRE_EQUAL(scene.samples.size(), 3);
  BOOST_CHECK_EQUAL(scene.samples[0].quality, 1.0f);
  BOOST_CHECK(scene.samples[0].qualityoverride);
  BOOST_CHECK_EQUAL(scene.samples[1].quality, 0.25f);
  BOOST_CHECK(!scene.samples[1].qualityoverride);
  BOOST_CHECK_EQUAL(scene.samples[2].quality, 0.75f);
  BOOST_CHECK(!scene.samples[2].qualityoverride);
}

BOOST_AUTO_TEST_CASE(inherited_quality_override_survives_separator)
{
  ComplexityScene scene;
  addcomplexity(scene.root, 0.25f, TRUE);
  SoSeparator * local = new SoSeparator;
  scene.root->addChild(local);
  addcomplexity(local, 1.0f, TRUE);
  mark(local);
  mark(scene.root);
  scene.run();
  BOOST_REQUIRE_EQUAL(scene.samples.size(), 2);
  for (unsigned int i = 0; i < 2; ++i) {
    BOOST_CHECK_EQUAL(scene.samples[i].quality, 0.25f);
    BOOST_CHECK(scene.samples[i].qualityoverride);
  }
}

BOOST_AUTO_TEST_CASE(reused_action_observes_notifications_and_resets_overrides)
{
  ComplexityScene scene;
  CallbackComplexity * node = new CallbackComplexity;
  node->textureQuality = 0.25f;
  node->setOverride(TRUE);
  scene.root->addChild(node);
  mark(scene.root);
  scene.run();
  BOOST_REQUIRE_EQUAL(scene.samples.size(), 1);
  BOOST_CHECK_EQUAL(scene.samples[0].quality, 0.25f);
  BOOST_CHECK(scene.samples[0].qualityoverride);

  int notifications = 0;
  SoNodeSensor sensor(notified, &notifications);
  sensor.setPriority(0);
  sensor.attach(node);
  node->textureQuality = 0.75f;
  BOOST_CHECK(notifications > 0);
  node->setOverride(FALSE);
  scene.run();
  BOOST_REQUIRE_EQUAL(scene.samples.size(), 1);
  BOOST_CHECK_EQUAL(scene.samples[0].quality, 0.75f);
  BOOST_CHECK(!scene.samples[0].qualityoverride);

  node->textureQuality.setIgnored(TRUE);
  scene.run();
  BOOST_REQUIRE_EQUAL(scene.samples.size(), 1);
  BOOST_CHECK_EQUAL(scene.samples[0].quality, 0.5f);
  BOOST_CHECK(!scene.samples[0].qualityoverride);
  BOOST_CHECK_EQUAL(node->calls, 3);
  sensor.detach();
}

BOOST_AUTO_TEST_SUITE_END()
