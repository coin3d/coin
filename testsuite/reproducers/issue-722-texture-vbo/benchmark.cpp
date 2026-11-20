// Compare immediate, CPU-array and VBO paths with the same scene.
// Timing includes glFinish(), excludes scene construction and warmup.
#include "../CoinCleanup.h"
#include <Inventor/SoDB.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoShaderProgram.h>
#include <Inventor/nodes/SoVertexShader.h>
#include <Inventor/nodes/SoFragmentShader.h>
#include <Inventor/nodes/SoShaderParameter.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/system/gl.h>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>

static void finishFrame(void *, SoAction * action)
{
  if (!action->isOfType(SoGLRenderAction::getClassTypeId())) return;
  static bool reported = false;
  if (!reported) {
    std::fprintf(stderr, "GL_RENDERER=%s\nGL_VERSION=%s\n", glGetString(GL_RENDERER), glGetString(GL_VERSION));
    reported = true;
  }
  glFinish();
}

int main(int argc, char ** argv)
{
  if (argc != 4) return 2;
  const bool textured = std::strcmp(argv[1], "no-texture") != 0;
  const bool legacycoords = false;
  const bool explicitcoords = std::strcmp(argv[1], "explicit") == 0;
  const int side = std::atoi(argv[2]);
  const int frames = std::atoi(argv[3]);
  if (side < 1 || side > 1000 || frames < 1) return 2;
  SoDB::init();
  CoinReproducerCleanup cleanup;
  SoSeparator * root = new SoSeparator;
  root->ref();
  root->renderCaching = SoSeparator::OFF;
  root->boundingBoxCaching = SoSeparator::ON;
  SoOrthographicCamera * camera = new SoOrthographicCamera;
  camera->position.setValue(0.5f, 0.5f, 3.0f);
  camera->height = 1.2f;
  root->addChild(camera);

  SoShaderProgram * program = new SoShaderProgram;
  SoVertexShader * vs = new SoVertexShader;
  vs->sourceType = SoShaderObject::GLSL_PROGRAM;
  vs->sourceProgram = legacycoords
    ? "void main() { gl_TexCoord[0] = gl_MultiTexCoord0; gl_Position = ftransform(); }"
    : "void main() { gl_Position = ftransform(); }";
  SoFragmentShader * fs = new SoFragmentShader;
  fs->sourceType = SoShaderObject::GLSL_PROGRAM;
  fs->sourceProgram = legacycoords
    ? "uniform sampler2D uT; void main() { gl_FragColor = texture2D(uT, gl_TexCoord[0].st); }"
    : textured
    ? "uniform sampler2D uT; void main() { gl_FragColor = texture2D(uT, vec2(0.5)); }"
    : "void main() { gl_FragColor = vec4(1.0); }";
  SoShaderParameter1i * sampler = new SoShaderParameter1i;
  sampler->name = "uT";
  sampler->value = 0;
  if (textured) fs->parameter.set1Value(0, sampler);
  program->shaderObject.set1Value(0, vs);
  program->shaderObject.set1Value(1, fs);
  root->addChild(program);

  if (textured) {
    SoTexture2 * texture = new SoTexture2;
    unsigned char pixels[] = {255, 0, 0, 0, 255, 0,
                              255, 0, 0, 0, 255, 0};
    texture->image.setValue(SbVec2s(2, 2), 3, pixels);
    texture->wrapS = SoTexture2::CLAMP;
    texture->wrapT = SoTexture2::CLAMP;
    root->addChild(texture);
  }

  SoIndexedFaceSet * shape = new SoIndexedFaceSet;
  SoVertexProperty * vp = new SoVertexProperty;
  int vi = 0, ii = 0;
  vp->vertex.setNum((side+1)*(side+1));
  vp->normal.setNum((side+1)*(side+1));
  shape->coordIndex.setNum(side*side*5);
  for (int y = 0; y <= side; ++y) {
    for (int x = 0; x <= side; ++x) {
      vp->vertex.set1Value(vi, float(x)/side, float(y)/side, 0.0f);
      vp->normal.set1Value(vi, 0.0f, 0.0f, 1.0f);
      if (explicitcoords) vp->texCoord.set1Value(vi, float(y)/side, float(x)/side);
      ++vi;
    }
  }
  vp->normalBinding = SoVertexProperty::PER_VERTEX_INDEXED;
  for (int y = 0; y < side; ++y) {
    for (int x = 0; x < side; ++x) {
      int a = y * (side + 1) + x;
      shape->coordIndex.set1Value(ii++, a);
      shape->coordIndex.set1Value(ii++, a + 1);
      shape->coordIndex.set1Value(ii++, a + side + 2);
      shape->coordIndex.set1Value(ii++, a + side + 1);
      shape->coordIndex.set1Value(ii++, -1);
    }
  }
  shape->vertexProperty = vp;
  root->addChild(shape);
  SoCallback * finish = new SoCallback;
  finish->setCallback(finishFrame, NULL);
  root->addChild(finish);

  SoOffscreenRenderer renderer(SbViewportRegion(128, 128));
  renderer.setComponents(SoOffscreenRenderer::RGB);
  for (int i=0; i<5; ++i) {
    if (!renderer.render(root)) { root->unref(); return 1; }
  }
  std::vector<double> samples;
  for (int i=0; i<frames; ++i) {
    const auto begin=std::chrono::steady_clock::now();
    const bool rendered=renderer.render(root);
    const auto end=std::chrono::steady_clock::now();
    if (!rendered) { root->unref(); return 1; }
    samples.push_back(std::chrono::duration<double,std::milli>(end-begin).count());
  }
  const unsigned char * pixels=renderer.getBuffer();
  const int pixel=(64*128+64)*3;
  const bool valid = textured ? pixels[pixel]>50 && pixels[pixel+1]>50 : pixels[pixel]>240;
  std::sort(samples.begin(), samples.end());
  const double median=samples[samples.size()/2];
  std::printf("{\"texture\":%d,\"side\":%d,\"vertices\":%d,\"faces\":%d,\"frames\":%d,\"median_ms\":%.6f,\"p10_ms\":%.6f,\"p90_ms\":%.6f,\"valid\":%d}\n",
    textured,side,vi,side*side,frames,median,samples[samples.size()/10],samples[(samples.size()*9)/10],valid);
  root->unref();
  return valid ? 0 : 1;
}
