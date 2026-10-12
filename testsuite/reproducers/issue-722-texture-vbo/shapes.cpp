#include "../CoinCleanup.h"
#include <Inventor/SoDB.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/elements/SoMultiTextureCoordinateElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoIndexedPointSet.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoTextureCoordinatePlane.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShaderProgram.h>
#include <Inventor/nodes/SoShaderParameter.h>
#include <Inventor/nodes/SoVertexShader.h>
#include <Inventor/nodes/SoFragmentShader.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTextureUnit.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/VRMLnodes/SoVRMLCoordinate.h>
#include <Inventor/VRMLnodes/SoVRMLNormal.h>
#include <Inventor/VRMLnodes/SoVRMLTextureCoordinate.h>
#include <Inventor/VRMLnodes/SoVRMLIndexedFaceSet.h>
#include <cstdio>
#include <cstring>
#include <vector>

static int missingarrays = 0;
static void diagnostic(const SoError * error, void *)
{
  const char * message = error->getDebugString().getString();
  if (std::strstr(message, "no coordinate array is available")) ++missingarrays;
  std::fprintf(stderr, "%s\n", message);
}

static const SbVec4f & procedural(void *, const SbVec3f & point, const SbVec3f & normal)
{
  static SbVec4f result;
  result.setValue(normal[2] > 0 ? point[1] : 1.0f-point[1], point[0], 0, 1);
  return result;
}

static void installFunction(void * node, SoAction * action)
{
  SoMultiTextureCoordinateElement::setFunction(action->getState(),
    static_cast<SoNode *>(node), 0, procedural, NULL);
}

int main(int argc, char ** argv)
{
  if (argc != 3) return 2;
  const char * kind = argv[1];
  const char * mode = argv[2];
  const bool faces = !std::strcmp(kind, "face");
  const bool point = !std::strcmp(kind, "point");
  const bool indexedpoint = !std::strcmp(kind, "indexed-point");
  const bool line = !std::strcmp(kind, "line");
  const bool vrml = !std::strcmp(kind, "vrml");
  const bool texgen = !std::strcmp(mode, "texgen");
  const bool textured = std::strcmp(mode, "no-texture") != 0;
  const bool constant = !std::strcmp(mode, "texture");
  const bool explicitcoords = !std::strcmp(mode, "explicit");
  SoDB::init();
  CoinReproducerCleanup cleanup;
  SoDebugError::setHandlerCallback(diagnostic, NULL);
  SoSeparator * root = new SoSeparator;
  root->ref();
  SoOrthographicCamera * camera = new SoOrthographicCamera;
  camera->position.setValue(0.5f, 0.5f, 3);
  camera->height = 1.2f;
  root->addChild(camera);
  if (!texgen) {
    SoShaderProgram * program = new SoShaderProgram;
    SoVertexShader * vs = new SoVertexShader;
    vs->sourceType = SoShaderObject::GLSL_PROGRAM;
    vs->sourceProgram = "void main() { gl_TexCoord[0] = gl_MultiTexCoord0; gl_Position = ftransform(); }";
    SoFragmentShader * fs = new SoFragmentShader;
    fs->sourceType = SoShaderObject::GLSL_PROGRAM;
    fs->sourceProgram = !textured ? "void main() { gl_FragColor = vec4(1.0); }" : constant
      ? "uniform sampler2D uT; void main() { gl_FragColor = texture2D(uT, vec2(0.5)); }"
      : "uniform sampler2D uT; void main() { gl_FragColor = texture2D(uT, gl_TexCoord[0].st); }";
    if (textured) {
      SoShaderParameter1i * sampler = new SoShaderParameter1i;
      sampler->name = "uT";
      sampler->value = 0;
      fs->parameter.set1Value(0, sampler);
    }
    program->shaderObject.set1Value(0, vs);
    program->shaderObject.set1Value(1, fs);
    root->addChild(program);
  }
  if (textured) {
    const unsigned char pixels[] = {255,0,0, 0,255,0, 255,0,0, 0,255,0};
    SoTexture2 * texture = new SoTexture2;
    texture->image.setValue(SbVec2s(2,2), 3, pixels);
    texture->wrapS = texture->wrapT = SoTexture2::CLAMP;
    root->addChild(texture);
    if (!std::strcmp(mode, "multi")) {
      SoTextureUnit * unit = new SoTextureUnit;
      unit->unit = 1;
      root->addChild(unit);
      SoTexture2 * other = new SoTexture2;
      other->image.setValue(SbVec2s(2,2), 3, pixels);
      root->addChild(other);
      unit = new SoTextureUnit;
      unit->unit = 0;
      root->addChild(unit);
    }
    if (!std::strcmp(mode, "function")) {
      SoCallback * callback = new SoCallback;
      callback->setCallback(installFunction, callback);
      root->addChild(callback);
    }
  }
  if (texgen) {
    SoTextureCoordinatePlane * plane = new SoTextureCoordinatePlane;
    plane->directionS.setValue(0,1,0);
    plane->directionT.setValue(1,0,0);
    root->addChild(plane);
    SoLightModel * light = new SoLightModel;
    light->model = SoLightModel::BASE_COLOR;
    root->addChild(light);
  }
  SoDrawStyle * style = new SoDrawStyle;
  style->pointSize = 6;
  style->lineWidth = 6;
  root->addChild(style);

  std::vector<SbVec3f> vertices;
  std::vector<int32_t> indices;
  const int side = 25;
  if (faces) {
    // Exercise an unused prefix and a nonzero startIndex.
    for (int i=0; i<3; ++i) vertices.push_back(SbVec3f(-3,-3,0));
    for (int y=0; y<side; ++y) for (int x=0; x<side; ++x) {
      vertices.push_back(SbVec3f(float(x)/side, float(y)/side, 0));
      vertices.push_back(SbVec3f(float(x+1)/side, float(y)/side, 0));
      vertices.push_back(SbVec3f(float(x+1)/side, float(y+1)/side, 0));
      vertices.push_back(SbVec3f(float(x)/side, float(y+1)/side, 0));
    }
  }
  else {
    for (int y=0; y<=side; ++y) for (int x=0; x<=side; ++x)
      vertices.push_back(SbVec3f(float(x)/side, float(y)/side, 0));
    if (indexedpoint) {
      for (size_t i=0; i<vertices.size(); ++i) indices.push_back(int32_t(i));
    }
    else if (line) {
      for (int y=0; y<=side; ++y) {
        for (int x=0; x<=side; ++x) indices.push_back(y*(side+1)+x);
        indices.push_back(-1);
      }
    }
    else {
      for (int y=0; y<side; ++y) for (int x=0; x<side; ++x) {
        const int a=y*(side+1)+x;
        indices.push_back(a); indices.push_back(a+1);
        indices.push_back(a+side+2); indices.push_back(a+side+1); indices.push_back(-1);
      }
    }
  }
  SoVertexProperty * vp = new SoVertexProperty;
  vp->vertex.setValues(0, int(vertices.size()), vertices.data());
  for (size_t i=0; i<vertices.size(); ++i) {
    if (std::strcmp(mode, "generated-normal")) vp->normal.set1Value(int(i), 0,0,1);
    if (explicitcoords) vp->texCoord.set1Value(int(i), vertices[i][1], vertices[i][0]);
  }
  vp->normalBinding = faces || point ? SoVertexProperty::PER_VERTEX : SoVertexProperty::PER_VERTEX_INDEXED;
  if (vrml) {
    SoVRMLIndexedFaceSet * shape = new SoVRMLIndexedFaceSet;
    SoVRMLCoordinate * coord = new SoVRMLCoordinate;
    coord->point = vp->vertex;
    SoVRMLNormal * normal = new SoVRMLNormal;
    normal->vector = vp->normal;
    shape->coord = coord; shape->normal = normal;
    if (explicitcoords) {
      SoVRMLTextureCoordinate * tex = new SoVRMLTextureCoordinate;
      tex->point = vp->texCoord;
      shape->texCoord = tex;
    }
    shape->coordIndex.setValues(0, int(indices.size()), indices.data());
    root->addChild(shape);
    vp->ref(); vp->unref();
  }
  else if (faces) {
    SoFaceSet * shape = new SoFaceSet;
    shape->startIndex = 3;
    for (int i=0; i<side*side; ++i) shape->numVertices.set1Value(i, 4);
    shape->vertexProperty = vp;
    root->addChild(shape);
  }
  else if (point) {
    SoPointSet * shape = new SoPointSet;
    shape->numPoints = int(vertices.size()); shape->vertexProperty = vp;
    root->addChild(shape);
  }
  else {
    SoIndexedShape * shape = indexedpoint ? static_cast<SoIndexedShape *>(new SoIndexedPointSet)
      : line ? static_cast<SoIndexedShape *>(new SoIndexedLineSet)
      : static_cast<SoIndexedShape *>(new SoIndexedFaceSet);
    shape->coordIndex.setValues(0, int(indices.size()), indices.data());
    shape->vertexProperty = vp;
    root->addChild(shape);
  }
  SoOffscreenRenderer renderer(SbViewportRegion(128,128));
  renderer.setComponents(SoOffscreenRenderer::RGB);
  bool ok = true;
  for (int frame=0; frame<2; ++frame) {
    if (!renderer.render(root)) { ok=false; break; }
    const unsigned char * image=renderer.getBuffer();
    const int bottom=(32*128+64)*3, top=(96*128+64)*3;
    if (!textured) ok = ok && image[bottom]>240 && image[top]>240;
    else if (constant) ok = ok && image[bottom]>50 && image[bottom+1]>50 &&
      image[bottom]==image[top] && image[bottom+1]==image[top+1];
    else ok = ok && image[bottom]>image[bottom+1]+100 && image[top+1]>image[top]+100;
    std::printf("%s %s frame=%d bottom=%u,%u,%u top=%u,%u,%u missing-arrays=%d ok=%d\n",
      kind, mode, frame, image[bottom],image[bottom+1],image[bottom+2],
      image[top],image[top+1],image[top+2],missingarrays,ok);
  }
  ok = ok && missingarrays==0;
  root->unref();
  SoDebugError::setHandlerCallback(NULL, NULL);
  return ok ? 0 : 1;
}
