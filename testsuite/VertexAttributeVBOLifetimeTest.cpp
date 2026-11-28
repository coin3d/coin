#include <Inventor/SoDB.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/elements/SoGLVertexAttributeElement.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/misc/SoGLDriverDatabase.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShaderObject.h>
#include <Inventor/nodes/SoShaderProgram.h>
#include <Inventor/nodes/SoVertexAttribute.h>
#include <Inventor/nodes/SoVertexShader.h>
#include <Inventor/system/gl.h>

#include <cstdio>

#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER 0x8892
#endif
#ifndef GL_ARRAY_BUFFER_BINDING
#define GL_ARRAY_BUFFER_BINDING 0x8894
#endif
#ifndef COIN_TEST_SKIP_RETURN_CODE
#define COIN_TEST_SKIP_RETURN_CODE 77
#endif

struct VertexAttributeVBOProbe {
  enum Phase { CAPTURE, VERIFY };

  VertexAttributeVBOProbe()
    : phase(CAPTURE), buffer(0), context(0), capable(false), captured(false),
      samecontext(false), alive(false), calls(0) { }

  Phase phase;
  GLuint buffer;
  uint32_t context;
  bool capable;
  bool captured;
  bool samecontext;
  bool alive;
  int calls;
};

static void
probeVBO(void * closure, SoAction * action)
{
  VertexAttributeVBOProbe & probe =
    *static_cast<VertexAttributeVBOProbe *>(closure);
  ++probe.calls;

  SoGLRenderAction * glaction = static_cast<SoGLRenderAction *>(action);
  const uint32_t context = glaction->getCacheContext();
  const cc_glglue * glue = cc_glglue_instance(static_cast<int>(context));

  if (probe.phase == VertexAttributeVBOProbe::VERIFY) {
    probe.samecontext = context == probe.context;
    if (probe.samecontext) {
      probe.alive = cc_glglue_glIsBuffer(glue, probe.buffer) == GL_TRUE;
    }
    return;
  }

  probe.context = context;
  probe.capable =
    cc_glglue_glversion_matches_at_least(glue, 2, 0, 0) &&
    SoGLDriverDatabase::isSupported(glue, SO_GL_VERTEX_BUFFER_OBJECT) &&
    SoGLDriverDatabase::isSupported(glue, SO_GL_ARB_SHADER_OBJECT) &&
    SoGLDriverDatabase::isSupported(glue, SO_GL_ARB_VERTEX_SHADER);
  if (!probe.capable) return;

  cc_glglue_glBindBuffer(glue, GL_ARRAY_BUFFER, 0);
  const SoGLVertexAttributeElement * element =
    SoGLVertexAttributeElement::getInstance(glaction->getState());
  element->enableVBO(glaction);

  GLint binding = 0;
  glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &binding);

  element->disableVBO(glaction);
  cc_glglue_glBindBuffer(glue, GL_ARRAY_BUFFER, 0);
  probe.buffer = static_cast<GLuint>(binding);
  probe.captured = binding > 0 &&
    cc_glglue_glIsBuffer(glue, probe.buffer) == GL_TRUE;
}

static SoSeparator *
createAttributeScene(VertexAttributeVBOProbe & probe)
{
  SoSeparator * root = new SoSeparator;
  root->ref();
  root->renderCaching = SoSeparator::OFF;

  SoVertexShader * shader = new SoVertexShader;
  shader->sourceType = SoShaderObject::GLSL_PROGRAM;
  shader->sourceProgram =
    "#version 120\n"
    "attribute float coin_test_scalar;\n"
    "void main(void) {\n"
    "  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "  gl_FrontColor = vec4(coin_test_scalar, 0.0, 0.0, 1.0);\n"
    "}\n";

  SoShaderProgram * program = new SoShaderProgram;
  program->shaderObject.set1Value(0, shader);
  root->addChild(program);

  SbVec3f points[24];
  float scalars[24];
  for (int i = 0; i < 24; ++i) {
    points[i].setValue(static_cast<float>(i % 6),
                       static_cast<float>(i / 6), 0.0f);
    scalars[i] = static_cast<float>(i) / 23.0f;
  }

  SoCoordinate3 * coordinates = new SoCoordinate3;
  coordinates->point.setValues(0, 24, points);
  root->addChild(coordinates);

  SoVertexAttribute * attribute = new SoVertexAttribute;
  attribute->name = "coin_test_scalar";
  attribute->typeName = "SoMFFloat";
  static_cast<SoMFFloat *>(attribute->getValuesField())
    ->setValues(0, 24, scalars);
  root->addChild(attribute);

  SoCallback * callback = new SoCallback;
  callback->setCallback(probeVBO, &probe);
  root->addChild(callback);
  return root;
}

static SoSeparator *
createVerificationScene(VertexAttributeVBOProbe & probe)
{
  SoSeparator * root = new SoSeparator;
  root->ref();
  root->renderCaching = SoSeparator::OFF;
  SoCallback * callback = new SoCallback;
  callback->setCallback(probeVBO, &probe);
  root->addChild(callback);
  return root;
}

int
main()
{
  SoDB::init();
  int result = 0;
  {
    VertexAttributeVBOProbe probe;
    SoOffscreenRenderer renderer(SbViewportRegion(32, 32));
    SoSeparator * first = createAttributeScene(probe);

    if (!renderer.render(first)) {
      result = COIN_TEST_SKIP_RETURN_CODE;
    }
    else if (probe.calls != 1) {
      std::fprintf(stderr,
                   "VBO fixture callback count was %d instead of one\n",
                   probe.calls);
      result = 1;
    }
    else if (!probe.capable) {
      result = COIN_TEST_SKIP_RETURN_CODE;
    }
    else if (!probe.captured) {
      std::fprintf(stderr,
                   "VBO fixture did not capture a live buffer "
                   "(calls=%d id=%u)\n",
                   probe.calls, static_cast<unsigned int>(probe.buffer));
      result = 1;
    }

    first->unref();

    if (result == 0) {
      probe.phase = VertexAttributeVBOProbe::VERIFY;
      probe.calls = 0;
      SoSeparator * second = createVerificationScene(probe);
      if (!renderer.render(second)) {
        std::fprintf(stderr, "second render failed\n");
        result = 1;
      }
      else if (probe.calls != 1 || !probe.samecontext) {
        std::fprintf(stderr, "retained offscreen context was not reused\n");
        result = 1;
      }
      else if (probe.alive) {
        std::fprintf(stderr,
                     "vertex-attribute VBO remained live after owner "
                     "destruction\n");
        result = 1;
      }
      second->unref();
    }
  }
  SoDB::finish();
  return result;
}
