// Coin #722: a shader sampling its own UVs still pays for implicit texture
// coordinates when a SoTexture2 is enabled. Run with run.sh.
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
#include <cstdio>
#include <cstdlib>
#include <cstring>

int main(int argc, char ** argv)
{
  const bool textured = argc > 1 && argv[1][0] == 't';
  const bool legacycoords = argc > 1 && std::strcmp(argv[1], "texcoord") == 0;
  SoDB::init();
  CoinReproducerCleanup cleanup;
  SoSeparator * root = new SoSeparator;
  root->ref();
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
  const int side = 25;
  int vi = 0, ii = 0;
  for (int y = 0; y <= side; ++y) {
    for (int x = 0; x <= side; ++x) {
      vp->vertex.set1Value(vi, float(x)/side, float(y)/side, 0.0f);
      vp->normal.set1Value(vi++, 0.0f, 0.0f, 1.0f);
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

  SoOffscreenRenderer renderer(SbViewportRegion(128, 128));
  renderer.setComponents(SoOffscreenRenderer::RGB);
  bool ok = renderer.render(root);
  const unsigned char * image = ok ? renderer.getBuffer() : NULL;
  int left = (64 * 128 + 32) * 3;
  int right = (64 * 128 + 96) * 3;
  int bottom = (32 * 128 + 64) * 3;
  int top = (96 * 128 + 64) * 3;
  std::printf("texture=%d legacycoords=%d rendered=%d left=%u,%u,%u right=%u,%u,%u bottom=%u,%u,%u top=%u,%u,%u\n",
              textured, legacycoords, ok,
              image ? image[left] : 0, image ? image[left+1] : 0, image ? image[left+2] : 0,
              image ? image[right] : 0, image ? image[right+1] : 0, image ? image[right+2] : 0,
              image ? image[bottom] : 0, image ? image[bottom+1] : 0, image ? image[bottom+2] : 0,
              image ? image[top] : 0, image ? image[top+1] : 0, image ? image[top+2] : 0);
  if (ok && textured) {
    if (legacycoords) {
      ok = image[bottom] > image[bottom+1] + 100 &&
           image[top+1] > image[top] + 100;
    }
    else {
      ok = image[bottom] == image[top] &&
           image[bottom+1] == image[top+1];
    }
  }
  root->unref();
  return ok ? 0 : 1;
}
