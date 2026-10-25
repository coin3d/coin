#ifndef COIN_TEXTURE2_OVERRIDE_TEST_SUPPORT_H
#define COIN_TEXTURE2_OVERRIDE_TEST_SUPPORT_H

#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/elements/SoTextureOverrideElement.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/elements/SoMultiTextureImageElement.h>
#include <Inventor/elements/SoGLMultiTextureImageElement.h>
#include <Inventor/elements/SoMultiTextureEnabledElement.h>
#include <Inventor/misc/SoGLImage.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/SbImage.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTextureUnit.h>
#include <stdexcept>
#include <string>

namespace Texture2OverrideTest {
inline void require(bool ok, const char * message) {
  if (!ok) throw std::runtime_error(message);
}
struct Snapshot {
  Snapshot(int u = 0) : unit(u), visits(0), enabled(false), imageoverride(false),
    qualityoverride(false), hasquality(false), quality(0), size(0,0), components(0), first(-1), wrapS(0), wrapT(0) {}
  int unit, visits;
  bool enabled, imageoverride, qualityoverride, hasquality;
  float quality;
  SbVec2s size;
  int components, first, wrapS, wrapT;
};
inline void capture(void * data, SoAction * action) {
  Snapshot & s = *static_cast<Snapshot *>(data);
  SoState * state = action->getState();
  ++s.visits;
  s.enabled = SoMultiTextureEnabledElement::get(state, s.unit) != FALSE;
  s.imageoverride = SoTextureOverrideElement::getImageOverride(state) != FALSE;
  s.qualityoverride = SoTextureOverrideElement::getQualityOverride(state) != FALSE;
  s.hasquality = state->isElementEnabled(SoTextureQualityElement::getClassStackIndex()) != FALSE;
  if (s.hasquality) s.quality = SoTextureQualityElement::get(state);
  s.size = SbVec2s(0,0); s.components = 0; s.wrapS = 0; s.wrapT = 0;
  const unsigned char * bytes = NULL;
  if (action->isOfType(SoGLRenderAction::getClassTypeId())) {
    SoMultiTextureImageElement::Model model;
    SbColor color;
    SoGLImage * image = SoGLMultiTextureImageElement::get(state, s.unit, model, color);
    if (image && image->getImage()) {
      bytes = image->getImage()->getValue(s.size, s.components);
      // GLImage uses its own enum values for wrap modes.
      s.wrapS = image->getWrapS() == SoGLImage::CLAMP ? SoTexture2::CLAMP : SoTexture2::REPEAT;
      s.wrapT = image->getWrapT() == SoGLImage::CLAMP ? SoTexture2::CLAMP : SoTexture2::REPEAT;
    }
  }
  else {
    bytes = SoMultiTextureImageElement::getImage(state, s.unit, s.size, s.components);
    s.wrapS = SoMultiTextureImageElement::getWrapS(state, s.unit);
    s.wrapT = SoMultiTextureImageElement::getWrapT(state, s.unit);
  }
  s.first = bytes && s.size != SbVec2s(0,0) ? bytes[0] : -1;
}
inline void probe(SoGroup * group, Snapshot & s) {
  SoCallback * cb = new SoCallback;
  cb->setCallback(capture, &s); group->addChild(cb);
}
inline void unit(SoGroup * group, int value) {
  SoTextureUnit * u = new SoTextureUnit; u->unit = value; group->addChild(u);
}
inline SoTexture2 * texture(SoGroup * group, int value, bool overrideimage = false) {
  SoTexture2 * t = new SoTexture2;
  if (value >= 0) {
    const unsigned char bytes[] = {static_cast<unsigned char>(value), 20, 30};
    t->image.setValue(SbVec2s(1,1), 3, bytes);
  }
  t->setOverride(overrideimage); group->addChild(t); return t;
}
struct Scene {
  Scene() : root(new SoSeparator) { root->ref(); root->renderCaching = SoSeparator::OFF; }
  ~Scene() { root->unref(); }
  SoSeparator * root;
};
inline void expect(const Snapshot & s, int value, bool protect) {
  require(s.visits > 0, "state probe was not traversed");
  require(s.imageoverride == protect, "unexpected unit-zero image protection");
  require(s.first == value, "later texture changed the selected image");
  require(s.enabled == (value >= 0), "unexpected texture enabled state");
  if (value >= 0) require(s.size == SbVec2s(1,1) && s.components == 3, "wrong image dimensions/components");
}
inline void qualityProtection(void *, SoAction * action) {
  SoTextureOverrideElement::setQualityOverride(action->getState(), TRUE);
  // Pick actions carry the override flags without the quality-value element.
  if (action->getState()->isElementEnabled(SoTextureQualityElement::getClassStackIndex()))
    SoTextureQualityElement::set(action->getState(), 0.75f);
}
inline void seedQuality(SoGroup * root) {
  SoCallback * cb = new SoCallback; cb->setCallback(qualityProtection); root->addChild(cb);
}

// All cases use separators so reapplying an action cannot inherit the prior
// root's override state. A root SoGroup intentionally retains that state.
inline void ordering(SoAction & action) {
  for (int kind = 0; kind < 5; ++kind) {
    Scene scene;
    const bool firstoverride = kind != 0;
    const int first = kind == 3 ? -1 : 80;
    Snapshot before, after;
    texture(scene.root, first, firstoverride); probe(scene.root, before);
    texture(scene.root, kind == 4 ? -1 : 160, kind == 2);
    probe(scene.root, after); action.apply(scene.root);
    expect(before, first, firstoverride);
    expect(after, firstoverride ? first : 160, firstoverride);
    require(!after.qualityoverride, "image override also changed quality protection");
  }
  Scene scene; Snapshot s;
  texture(scene.root, 40); texture(scene.root, 80, true);
  texture(scene.root, 160, true); probe(scene.root, s);
  action.apply(scene.root); expect(s, 80, true);
}
inline void units(SoAction & action) {
  {
    Scene scene; Snapshot zero, one(1);
    texture(scene.root, 80, true);
    unit(scene.root, 1); texture(scene.root, 160); probe(scene.root, one);
    unit(scene.root, 0); texture(scene.root, 240); probe(scene.root, zero);
    action.apply(scene.root); expect(zero, 80, true); expect(one, 160, true);
  }
  {
    Scene scene; Snapshot zero, one(1);
    unit(scene.root, 1); texture(scene.root, 80, true);
    // A nonzero override establishes no global protection, including against
    // another texture on that same nonzero unit.
    texture(scene.root, 160); probe(scene.root, one);
    unit(scene.root, 0); texture(scene.root, 240); probe(scene.root, zero);
    action.apply(scene.root); expect(zero, 240, false); expect(one, 160, false);
  }
  {
    Scene scene; Snapshot zero, one(1);
    texture(scene.root, -1, true); unit(scene.root, 1);
    texture(scene.root, 160); probe(scene.root, one);
    unit(scene.root, 0); texture(scene.root, 240); probe(scene.root, zero);
    action.apply(scene.root); expect(zero, -1, true); expect(one, 160, true);
  }
}
inline void scope(SoAction & action) {
  Scene scene; Snapshot inside, popped, after;
  texture(scene.root, 80);
  SoSeparator * child = new SoSeparator; child->renderCaching = SoSeparator::OFF;
  scene.root->addChild(child);
  texture(child, 160, true); texture(child, 240); probe(child, inside);
  probe(scene.root, popped); texture(scene.root, 240); probe(scene.root, after);
  action.apply(scene.root);
  expect(inside, 160, true); expect(popped, 80, false); expect(after, 240, false);
  action.apply(scene.root);
  require(after.visits == 2, "reused action did not traverse scene");
  expect(inside, 160, true); expect(popped, 80, false); expect(after, 240, false);
}
inline void quality(SoAction & action) {
  Scene scene; Snapshot s;
  seedQuality(scene.root); texture(scene.root, 80, true); texture(scene.root, 160);
  probe(scene.root, s); action.apply(scene.root); expect(s, 80, true);
  require(s.qualityoverride, "image handling overwrote quality protection");
  if (s.hasquality) require(s.quality == 0.75f, "image handling overwrote quality value");
}
inline void edits(SoAction & action) {
  Scene scene; Snapshot s;
  SoTexture2 * t = texture(scene.root, 80, true);
  texture(scene.root, 160); probe(scene.root, s);
  action.apply(scene.root); expect(s, 80, true);
  const unsigned char bytes[] = {240,20,30};
  t->image.setValue(SbVec2s(1,1), 3, bytes);
  t->wrapS = SoTexture2::CLAMP; t->wrapT = SoTexture2::CLAMP;
  action.apply(scene.root); expect(s, 240, true);
  require(s.wrapS == SoTexture2::CLAMP && s.wrapT == SoTexture2::CLAMP, "wrap notification did not update selected image");
  require(t->filename.isDefault() && !t->image.isDefault(), "image notification changed serialization contract");
  t->image.setValue(SbVec2s(0,0), 0, NULL);
  action.apply(scene.root);
  expect(s, -1, true);
  t->image.setValue(SbVec2s(1,1), 3, bytes);
  action.apply(scene.root); expect(s, 240, true);
  // No file read is needed to exercise clearing a filename. The filename
  // sensor resets image and serialization defaults synchronously at priority 0.
  const SbBool notify = t->filename.enableNotify(FALSE);
  t->filename = "not-loaded.rgb"; t->filename.enableNotify(notify);
  t->filename = "";
  action.apply(scene.root); expect(s, -1, true);
  require(t->image.isDefault() && t->filename.isDefault(), "filename clear notification did not restore defaults");
}
inline void ignored(SoAction & action) {
  Scene scene; Snapshot s;
  SoTexture2 * t = texture(scene.root, -1, true);
  // Setting filename ignored notifies its priority-zero sensor. With the
  // empty filename this resets image, so set up the inline image afterwards.
  t->filename.setIgnored(TRUE);
  const unsigned char bytes[] = {80,20,30};
  t->image.setValue(SbVec2s(1,1), 3, bytes);
  t->image.setIgnored(TRUE); t->wrapS = SoTexture2::CLAMP;
  t->wrapS.setIgnored(TRUE);
  texture(scene.root, 160); probe(scene.root, s); action.apply(scene.root);
  // SoTexture2 historically consumes these fields even when ignored.
  expect(s, 80, true); require(s.wrapS == SoTexture2::CLAMP, "ignored wrap semantics changed");
}
inline void placeholder(SoAction & action) {
  Scene scene; Snapshot s;
  SoTexture2 * t = texture(scene.root, -1, true);
  // Model a filename which has not been loaded without filesystem/simage
  // dependencies or triggering the filename field's immediate sensor.
  const SbBool notify = t->filename.enableNotify(FALSE);
  t->filename = "texture2-override-not-yet-loaded.rgb";
  t->filename.enableNotify(notify); t->image.setDefault(TRUE);
  texture(scene.root, 160); probe(scene.root, s); action.apply(scene.root);
  require(s.visits && s.imageoverride && s.enabled, "pending filename lost placeholder/protection");
  require(s.size == SbVec2s(2,2) && s.components == 1 && s.first == 255, "pending filename placeholder changed");
}
class DerivedTexture : public SoTexture2 {
public:
  DerivedTexture() : callbacks(0), picks(0), renders(0) {}
  int callbacks, picks, renders;
  void callback(SoCallbackAction * a) override { ++callbacks; SoTexture2::callback(a); }
  void rayPick(SoRayPickAction * a) override { ++picks; SoTexture2::rayPick(a); }
  void GLRender(SoGLRenderAction * a) override { ++renders; SoTexture2::GLRender(a); }
protected:
  ~DerivedTexture() override {}
};
inline void dispatch(SoAction & action) {
  Scene scene; Snapshot s;
  DerivedTexture * t = new DerivedTexture;
  const unsigned char bytes[] = {80,20,30}; t->image.setValue(SbVec2s(1,1), 3, bytes);
  t->setOverride(TRUE); scene.root->addChild(t);
  texture(scene.root, 160); probe(scene.root, s); action.apply(scene.root);
  expect(s, 80, true);
  if (action.isOfType(SoGLRenderAction::getClassTypeId())) require(t->renders == 1, "GLRender virtual dispatch changed");
  else if (action.isOfType(SoRayPickAction::getClassTypeId())) require(t->picks == 1, "rayPick virtual dispatch changed");
  else require(t->callbacks == 1, "callback virtual dispatch changed");
}
inline void run(SoAction & action, bool multitexture, bool cpu) {
  ordering(action); scope(action); quality(action); edits(action); ignored(action); dispatch(action);
  if (multitexture) units(action);
  if (cpu) placeholder(action);
}
}
#endif
