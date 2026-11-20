// Exercise cached default UVs across unchanged frames, scene edits and contexts.
#include "../CoinCleanup.h"
#include <Inventor/SoDB.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/elements/SoComplexityElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoCoordinate4.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoShaderProgram.h>
#include <Inventor/nodes/SoVertexShader.h>
#include <Inventor/nodes/SoFragmentShader.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <cstdio>
#include <cstring>
#include <vector>

static int missingarrays = 0;

// Verify that a mapping cache captures dependencies beyond coordinates.
class StateMappedFaceSet : public SoIndexedFaceSet {
public:
  void computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center) override
  {
    SoIndexedFaceSet::computeBBox(action, box, center);
    if (SoComplexityElement::get(action->getState()) > 0.75f && !box.isEmpty()) {
      const SbVec3f size = box.getMax() - box.getMin();
      box.extendBy(box.getMax() + size);
    }
  }
};

class EmptyBoundsFaceSet : public SoIndexedFaceSet {
public:
  void computeBBox(SoAction *, SbBox3f & box, SbVec3f &) override
  {
    box.makeEmpty();
  }
};

// #901 stores an overflowing extent's maximum in the size slot. These two
// boxes consequently have equal encoded origins/sizes but different flags.
// The unchanged phase changes only inherited state, exercising that key bit.
class WideBoundsFaceSet : public SoIndexedFaceSet {
public:
  void computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center) override
  {
    const bool wide = SoComplexityElement::get(action->getState()) > 0.75f;
    box.setBounds(SbVec3f(-2e38f, 0, 0), SbVec3f(wide ? 2e38f : 0.0f, 1, 0));
    center = box.getCenter();
  }
};
static void diagnostic(const SoError * error, void *)
{
  const char * message = error->getDebugString().getString();
  if (std::strstr(message, "no coordinate array is available")) ++missingarrays;
  std::fprintf(stderr, "%s\n", message);
}

static unsigned long long imageHash(SoOffscreenRenderer & renderer)
{
  const unsigned char * image = renderer.getBuffer();
  unsigned long long hash = 14695981039346656037ULL;
  for (int i = 0; i < 128 * 128 * 3; ++i) {
    hash ^= image[i];
    hash *= 1099511628211ULL;
  }
  return hash;
}

static void explicitMapping(const std::vector<SbVec3f> & vertices,
                            const std::vector<int32_t> & indices,
                            bool enlarged, bool emptybounds, bool widebounds,
                            SoTextureCoordinate2 * coordinates)
{
  SbBox3f box;
  for (size_t i = 0; i < indices.size(); ++i) {
    if (indices[i] >= 0) box.extendBy(vertices[indices[i]]);
  }
  if (emptybounds) box.setBounds(SbVec3f(0,0,0), SbVec3f(1,1,1));
  if (enlarged) box.extendBy(box.getMax() + (box.getMax() - box.getMin()));
  const SbVec3f size = box.getMax() - box.getMin();
  // These scenes lie in XY. The current default mapping chooses Y for S
  // on equal planar extents and normalizes the two axes independently.
  // Empty bounds use the unit-cube fallback, whose chosen axes are Z/Y.
  const int s = emptybounds ? 2 : (size[1] >= size[0] ? 1 : 0);
  const int t = emptybounds ? 1 : 1 - s;
  const float ssize = size[s] > 0 ? size[s] : 1;
  const float tsize = size[t] > 0 ? size[t] : 1;
  std::vector<SbVec2f> uv;
  for (size_t i = 0; i < vertices.size(); ++i) {
    if (widebounds) {
      const double minimum = double(-2e38f);
      const double maximum = enlarged ? double(2e38f) : 0.0;
      uv.push_back(SbVec2f(float((double(vertices[i][0])-minimum)/(maximum-minimum)),
                          vertices[i][1]));
      continue;
    }
    uv.push_back(SbVec2f((vertices[i][s] - box.getMin()[s]) / ssize,
                        (vertices[i][t] - box.getMin()[t]) / tsize));
  }
  coordinates->point.setValues(0, int(uv.size()), uv.data());
  coordinates->point.setNum(int(uv.size()));
}

int main(int argc, char ** argv)
{
  if (argc < 2 || argc > 4) return 2;
  bool reference = false, cached = false;
  for (int i = 2; i < argc; ++i) {
    if (!std::strcmp(argv[i], "reference")) reference = true;
    else if (!std::strcmp(argv[i], "cached")) cached = true;
    else if (std::strcmp(argv[i], "uncached")) return 2;
  }
  const bool property = !std::strcmp(argv[1], "property");
  const bool homogeneous = !std::strcmp(argv[1], "homogeneous");
  const bool shared = !std::strcmp(argv[1], "shared");
  const bool statemapping = !std::strcmp(argv[1], "state");
  const bool emptybounds = !std::strcmp(argv[1], "empty-bounds");
  const bool widebounds = !std::strcmp(argv[1], "wide-bounds");
  SoDB::init();
  CoinReproducerCleanup cleanup;
  SoDebugError::setHandlerCallback(diagnostic, NULL);
  SoSeparator * root = new SoSeparator;
  root->ref();
  root->renderCaching = SoSeparator::OFF;
  SoOrthographicCamera * camera = new SoOrthographicCamera;
  camera->position.setValue(0.5f, 0.5f, 3);
  camera->height = 2.5f;
  root->addChild(camera);
  SoShaderProgram * program = new SoShaderProgram;
  SoVertexShader * vs = new SoVertexShader;
  vs->sourceType = SoShaderObject::GLSL_PROGRAM;
  vs->sourceProgram = "void main() { gl_TexCoord[0] = gl_MultiTexCoord0; gl_Position = ftransform(); }";
  SoFragmentShader * fs = new SoFragmentShader;
  fs->sourceType = SoShaderObject::GLSL_PROGRAM;
  fs->sourceProgram = "void main() { gl_FragColor = vec4(gl_TexCoord[0].st, 0.0, 1.0); }";
  program->shaderObject.set1Value(0, vs);
  program->shaderObject.set1Value(1, fs);
  root->addChild(program);
  SoTexture2 * texture = new SoTexture2;
  const unsigned char pixel[] = {255,255,255};
  texture->image.setValue(SbVec2s(1,1), 3, pixel);
  root->addChild(texture);
  SoNormal * normal = new SoNormal;
  normal->vector.setValue(0,0,1);
  root->addChild(normal);
  SoNormalBinding * binding = new SoNormalBinding;
  binding->value = SoNormalBinding::OVERALL;
  root->addChild(binding);

  SoComplexity * complexity = new SoComplexity;
  root->addChild(complexity);
  SoIndexedFaceSet * shape = statemapping ? new StateMappedFaceSet
    : emptybounds ? new EmptyBoundsFaceSet : widebounds ? new WideBoundsFaceSet : new SoIndexedFaceSet;
  SoSeparator * drawscope = cached && !shared ? new SoSeparator : NULL;
  SoSeparator * parent = drawscope ? drawscope : root;
  SoVertexProperty * vp = NULL;
  SoCoordinate3 * coord = NULL;
  SoCoordinate4 * coord4 = NULL;
  SoCoordinate3 * other = NULL;
  SoTextureCoordinate2 * referencecoords = reference ? new SoTextureCoordinate2 : NULL;
  SoTextureCoordinate2 * otherreference = reference && shared ? new SoTextureCoordinate2 : NULL;
  if (reference && !shared) root->addChild(referencecoords);
  if (property) {
    vp = new SoVertexProperty;
    vp->normal.setValue(0,0,1);
    vp->normalBinding = SoVertexProperty::OVERALL;
    shape->vertexProperty = vp;
    parent->addChild(shape);
  }
  else if (homogeneous) {
    coord4 = new SoCoordinate4;
    root->addChild(coord4);
    parent->addChild(shape);
  }
  else if (shared) {
    coord = new SoCoordinate3;
    other = new SoCoordinate3;
    for (int i = 0; i < 2; ++i) {
      SoSeparator * branch = new SoSeparator;
      branch->renderCaching = cached ? SoSeparator::ON : SoSeparator::OFF;
      SoTranslation * translation = new SoTranslation;
      translation->translation.setValue(i ? 0.6f : -0.6f, 0, 0);
      branch->addChild(translation);
      branch->addChild(i ? other : coord);
      if (reference) branch->addChild(i ? otherreference : referencecoords);
      branch->addChild(shape);
      root->addChild(branch);
    }
  }
  else {
    coord = new SoCoordinate3;
    root->addChild(coord);
    parent->addChild(shape);
  }
  if (drawscope) root->addChild(drawscope);

  SoOffscreenRenderer first(SbViewportRegion(128,128));
  SoOffscreenRenderer second(SbViewportRegion(128,128));
  first.setComponents(SoOffscreenRenderer::RGB);
  second.setComponents(SoOffscreenRenderer::RGB);
  bool ok = true;
  std::vector<SbVec3f> vertices;
  std::vector<int32_t> indices;
  const int side = 25;
  for (int phase = 0; phase < 8 && ok; ++phase) {
    if ((statemapping || widebounds) && phase == 1) complexity->value = 1.0f;
    // Phase 1 is unchanged. Phase 3 edits only indices; phase 5 restores
    // them without editing coordinates. Other phases replace the source.
    if (phase != 1 && phase != 3 && phase != 5) {
      vertices.clear();
      const int rows = phase == 4 ? side + 2 : side;
      for (int y = 0; y <= rows; ++y) for (int x = 0; x <= side; ++x) {
        const float px = phase == 2 ? 0.05f + 0.7f * x / side : float(x) / side;
        const float py = phase == 6 ? 0.5f : (phase == 2 ? 1.3f : 1.0f) * y / rows;
        vertices.push_back(SbVec3f(px, py, 0));
      }
      if (vp) vp->vertex.setValues(0, int(vertices.size()), vertices.data());
      else if (coord4) {
        std::vector<SbVec4f> points;
        for (size_t i = 0; i < vertices.size(); ++i) {
          const float w = float(1 << (i % 3));
          points.push_back(SbVec4f(vertices[i][0]*w, vertices[i][1]*w, 0, w));
        }
        coord4->point.setValues(0, int(points.size()), points.data());
        coord4->point.setNum(int(points.size()));
      }
      else {
        coord->point.setValues(0, int(vertices.size()), vertices.data());
        coord->point.setNum(int(vertices.size()));
        if (other) {
          std::vector<SbVec3f> mirrored = vertices;
          for (size_t i = 0; i < mirrored.size(); ++i) mirrored[i][0] = 1.0f - mirrored[i][0];
          other->point.setValues(0, int(mirrored.size()), mirrored.data());
          other->point.setNum(int(mirrored.size()));
        }
      }
      if (vp) vp->vertex.setNum(int(vertices.size()));
    }
    if (phase != 1) {
      indices.clear();
      const int rows = phase == 4 ? side + 2 : side;
      const int columns = phase == 3 ? side / 2 : side;
      for (int y = 0; y < rows; ++y) for (int x = 0; x < columns; ++x) {
        const int a = y * (side + 1) + x;
        indices.push_back(a); indices.push_back(a+1);
        indices.push_back(a+side+2); indices.push_back(-1);
        indices.push_back(a); indices.push_back(a+side+2);
        indices.push_back(a+side+1); indices.push_back(-1);
      }
      shape->coordIndex.setValues(0, int(indices.size()), indices.data());
      shape->coordIndex.setNum(int(indices.size()));
    }
    if (reference) {
      const bool enlarged = (statemapping || widebounds) && phase >= 1;
      explicitMapping(vertices, indices, enlarged, emptybounds, widebounds, referencecoords);
      if (otherreference) {
        std::vector<SbVec3f> mirrored = vertices;
        for (size_t i = 0; i < mirrored.size(); ++i) mirrored[i][0] = 1.0f - mirrored[i][0];
        explicitMapping(mirrored, indices, enlarged, emptybounds, widebounds, otherreference);
      }
    }
    for (int context = 0; context < 2; ++context) {
      // Build a parent display list using an already populated mapping
      // cache, then change inherited state outside that parent.
      if (drawscope) drawscope->renderCaching = phase == 0 && context == 0
        ? SoSeparator::OFF : SoSeparator::ON;
      SoOffscreenRenderer & renderer = context ? second : first;
      if (!renderer.render(root)) { ok = false; break; }
      std::printf("%s phase=%d context=%d hash=%016llx\n", argv[1], phase, context, imageHash(renderer));
    }
  }
  ok = ok && !missingarrays;
  root->unref();
  SoDebugError::setHandlerCallback(NULL, NULL);
  return ok ? 0 : 1;
}
