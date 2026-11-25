// Verify buffer contents, binding preservation and context invalidation.
#include "../CoinCleanup.h"
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoSeparator.h>
#include "rendering/SoVBO.h"
#include <cstdio>
#include <cstring>

struct Fixture {
  explicit Fixture(GLenum target) : buffer(target), sentinel(target), target(target),
    phase(0), ok(true), firstcontext(0), firstbuffer(0) {}
  SoVBO buffer, sentinel;
  GLenum target;
  int phase;
  bool ok;
  uint32_t firstcontext;
  GLint firstbuffer;
  float values[8];
};

static void check(void * userdata, SoAction * action)
{
  if (!action->isOfType(SoGLRenderAction::getClassTypeId())) return;
  Fixture & f = *static_cast<Fixture *>(userdata);
  const uint32_t context = static_cast<SoGLRenderAction *>(action)->getCacheContext();
  const cc_glglue * glue = cc_glglue_instance(context);
  typedef void (APIENTRY * ReadBuffer)(GLenum, intptr_t, intptr_t, void *);
  ReadBuffer read = reinterpret_cast<ReadBuffer>(cc_glglue_getprocaddress(glue, "glGetBufferSubData"));
  if (!read) { f.ok = false; return; }
  const GLenum binding = f.target == GL_ARRAY_BUFFER ? GL_ARRAY_BUFFER_BINDING :
                                                       GL_ELEMENT_ARRAY_BUFFER_BINDING;
  const int count = f.phase < 4 ? 4 : f.phase < 6 ? 8 : 2;
  const SbUniqueId id = f.phase < 2 ? 1 : f.phase < 4 ? 2 : f.phase < 6 ? 3 : 4;
  if (f.phase == 0 || f.phase == 2 || f.phase == 4 || f.phase == 6) {
    for (int i = 0; i < count; ++i) f.values[i] = float(id * 10 + i);
    f.sentinel.setBufferData(f.values, count * sizeof(float));
    f.sentinel.bindBuffer(context);
    GLint before, after;
    glGetIntegerv(binding, &before);
    f.buffer.updateBufferData(f.values, count * sizeof(float), id, context);
    glGetIntegerv(binding, &after);
    f.ok = f.ok && before != 0 && after == before;
  }
  f.buffer.bindBuffer(context);
  GLint current;
  glGetIntegerv(binding, &current);
  if (f.phase == 0) { f.firstcontext = context; f.firstbuffer = current; }
  if (f.phase == 1) f.ok = f.ok && context != f.firstcontext;
  if (f.phase == 2) f.ok = f.ok && context == f.firstcontext && current == f.firstbuffer;
  float actual[8] = {};
  read(f.target, 0, count * sizeof(float), actual);
  const GLvoid * data;
  intptr_t bytes;
  f.buffer.getBufferData(data, bytes);
  f.ok = f.ok && bytes == intptr_t(count * sizeof(float)) && data == f.values &&
    f.buffer.getBufferDataId() == id &&
    !std::memcmp(actual, f.values, count * sizeof(float)) && glGetError() == GL_NO_ERROR;
  cc_glglue_glBindBuffer(glue, f.target, 0);
}

static bool run(GLenum target)
{
  Fixture fixture(target);
  SoSeparator * root = new SoSeparator;
  root->ref();
  root->renderCaching = SoSeparator::OFF;
  SoCallback * callback = new SoCallback;
  callback->setCallback(check, &fixture);
  root->addChild(callback);
  SoOffscreenRenderer first(SbViewportRegion(16,16)), second(SbViewportRegion(16,16));
  for (fixture.phase = 0; fixture.phase < 8 && fixture.ok; ++fixture.phase) {
    SoOffscreenRenderer & renderer = fixture.phase % 2 ? second : first;
    if (!renderer.render(root)) fixture.ok = false;
  }
  root->unref();
  std::printf("target=%x phases=%d valid=%d\n", target, fixture.phase, fixture.ok);
  return fixture.ok;
}

int main()
{
  SoDB::init();
  CoinReproducerCleanup cleanup;
  return run(GL_ARRAY_BUFFER) && run(GL_ELEMENT_ARRAY_BUFFER) ? 0 : 1;
}
