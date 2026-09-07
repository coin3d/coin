// Reproducer for SoTextureCoordinate2::GLRender() (and SoVertexProperty's
// equivalent) hardcoding unit 0 in its SoGLVBOElement::setTexCoordVBO()
// call, instead of using the texture unit it just read from
// SoTextureUnitElement a few lines earlier (src/nodes/SoTextureCoordinate2.cpp,
// src/nodes/SoVertexProperty.cpp).
//
// In a scene with two TextureCoordinate2 nodes under two different
// TextureUnit contexts, both register their VBO under unit 0's slot in
// SoGLVBOElement -- so by the time the shape renders, unit 0's slot holds
// whichever of the two was traversed *last* (semantically unit 1's data),
// and unit 1's slot was never written at all (stays NULL, silently falling
// back to a plain client-memory pointer -- which happens to still be
// correct, since that fallback reads directly from
// SoMultiTextureCoordinateElement, which *is* properly per-unit).
// Net effect: unit 0 renders sampled with unit 1's texture coordinates.
//
// This only manifests through SoShape::startVertexArray()'s VBO/vertex-array
// fast path, which requires >= 20 vertices (SoVBO's default
// vbo_vertex_count_min_limit) and OVERALL normal binding (i.e. unlit, or no
// per-vertex normals needed) -- the immediate-mode fallback path used
// otherwise queries SoMultiTextureCoordinateElement directly per-unit and
// isn't affected.
//
// This builds a small unlit strip of quads (11x2 vertices = 22, above the
// VBO threshold) with two independent TextureCoordinate2 blocks: unit 0's
// texture coordinate varies along X (columns), unit 1's varies along Y
// (rows). Unit 0's texture is a solid red/green split (left half red,
// right half green); unit 1's is solid white/mid-gray (top white, bottom
// gray, MODULATEd on top of unit 0's color so it only affects brightness,
// not hue). If unit 0 is being sampled with unit 1's (row/Y-based)
// coordinates instead of its own (column/X-based) ones, the left and right
// columns of a single row stop differing in hue -- exactly the symptom
// reported in issue #526 (a texture's own coordinate stops varying the way
// its own geometry says it should, once another unit's coordinates are in
// the picture).
//
// See run.sh in this directory for how to build and run this against a
// given libCoin build.

#include <cstdio>
#include <vector>
#include <Inventor/SoDB.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SbColor.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTextureUnit.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoShapeHints.h>

static void setSplitTexture(SoTexture2 * tex, unsigned char r0, unsigned char g0, unsigned char b0,
                            unsigned char r1, unsigned char g1, unsigned char b1, SbBool horizontal)
{
  // 2x2 RGB texture, split either left/right (horizontal) or top/bottom.
  unsigned char buf[2*2*3];
  for (int y = 0; y < 2; y++) {
    for (int x = 0; x < 2; x++) {
      int idx = (y*2 + x) * 3;
      SbBool firsthalf = horizontal ? (x == 0) : (y == 0);
      buf[idx+0] = firsthalf ? r0 : r1;
      buf[idx+1] = firsthalf ? g0 : g1;
      buf[idx+2] = firsthalf ? b0 : b1;
    }
  }
  tex->image.setValue(SbVec2s(2,2), 3, buf);
  tex->wrapS = SoTexture2::CLAMP;
  tex->wrapT = SoTexture2::CLAMP;
}

int main()
{
  SoDB::init();

  SoSeparator * root = new SoSeparator;
  root->ref();

  SoOrthographicCamera * cam = new SoOrthographicCamera;
  cam->position.setValue(0.5f, 0.5f, 5.0f);
  cam->nearDistance = 1.0f;
  cam->farDistance = 10.0f;
  cam->height = 1.2f;
  root->addChild(cam);

  // Unlit: this is what forces OVERALL normal binding in the shape,
  // which is what enables the buggy VBO/vertex-array fast path.
  SoLightModel * lm = new SoLightModel;
  lm->model = SoLightModel::BASE_COLOR;
  root->addChild(lm);

  SoMaterial * mat = new SoMaterial;
  mat->diffuseColor.setValue(1,1,1);
  root->addChild(mat);

  SoShapeHints * hints = new SoShapeHints;
  hints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
  root->addChild(hints);

  // 11 columns x 2 rows = 22 vertices, at or above SoVBO's default
  // vbo_vertex_count_min_limit (20), needed to trigger the VBO path at all.
  const int COLS = 11, ROWS = 2;
  SoCoordinate3 * coords = new SoCoordinate3;
  std::vector<SbVec3f> pts;
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      pts.push_back(SbVec3f(col / float(COLS-1), row / float(ROWS-1), 0.0f));
    }
  }
  coords->point.setValues(0, (int)pts.size(), pts.data());
  root->addChild(coords);

  // Unit 0: red (left, col 0..4) / green (right, col 5..10) via X.
  SoTextureUnit * unit0 = new SoTextureUnit;
  unit0->unit = 0;
  root->addChild(unit0);
  SoTexture2 * tex0 = new SoTexture2;
  setSplitTexture(tex0, 255,0,0, 0,255,0, TRUE);
  root->addChild(tex0);
  SoTextureCoordinate2 * tc0 = new SoTextureCoordinate2;
  std::vector<SbVec2f> tc0pts;
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      tc0pts.push_back(SbVec2f(col / float(COLS-1), 0.5f));
    }
  }
  tc0->point.setValues(0, (int)tc0pts.size(), tc0pts.data());
  root->addChild(tc0);

  // Unit 1: white (top, row 1) / gray (bottom, row 0) via Y -- MODULATEs
  // unit 0's color, so it only ever dims it, never changes hue.
  SoTextureUnit * unit1 = new SoTextureUnit;
  unit1->unit = 1;
  root->addChild(unit1);
  SoTexture2 * tex1 = new SoTexture2;
  setSplitTexture(tex1, 128,128,128, 255,255,255, FALSE);
  root->addChild(tex1);
  SoTextureCoordinate2 * tc1 = new SoTextureCoordinate2;
  std::vector<SbVec2f> tc1pts;
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      tc1pts.push_back(SbVec2f(0.5f, row / float(ROWS-1)));
    }
  }
  tc1->point.setValues(0, (int)tc1pts.size(), tc1pts.data());
  root->addChild(tc1);

  SoIndexedFaceSet * ifs = new SoIndexedFaceSet;
  std::vector<int32_t> idx;
  for (int row = 0; row < ROWS-1; row++) {
    for (int col = 0; col < COLS-1; col++) {
      int a = row*COLS+col, b = row*COLS+col+1, c = (row+1)*COLS+col+1, d = (row+1)*COLS+col;
      idx.push_back(a); idx.push_back(b); idx.push_back(c); idx.push_back(d); idx.push_back(-1);
    }
  }
  ifs->coordIndex.setValues(0, (int)idx.size(), idx.data());
  root->addChild(ifs);

  SbViewportRegion vp(220, 40);
  SoOffscreenRenderer renderer(vp);
  renderer.setBackgroundColor(SbColor(0,0,0));
  if (!renderer.render(root)) {
    fprintf(stderr, "[repro] render() failed\n");
    return 2;
  }

  unsigned char * buf = renderer.getBuffer();
  int w = 220, h = 40;

  // Scan the whole image for clearly-red and clearly-green pixels, rather
  // than trusting fixed pixel offsets to land on the (small, off-center)
  // rendered quad -- the orthographic camera's height (1.2) vs. the 220x40
  // viewport's aspect ratio means the quad only occupies a small portion of
  // the frame, so any fixed-coordinate sample is fragile. Existence of both
  // hues anywhere in the image is exactly what distinguishes correct
  // per-unit texture coordinates (unit 0 shows its own red/green X split)
  // from the bug (unit 0 gets unit 1's coordinates, whose U is constant at
  // 0.5 for every vertex -- landing exactly on the red/green seam of unit
  // 0's texture and producing one uniform interpolated hue everywhere,
  // never a crisp red pixel or a crisp green pixel).
  SbBool foundRed = FALSE, foundGreen = FALSE;
  for (int i = 0; i < w*h; i++) {
    unsigned char * p = buf + i*3;
    if (p[0] > 150 && p[1] < 100 && p[2] < 100) foundRed = TRUE;
    if (p[1] > 150 && p[0] < 100 && p[2] < 100) foundGreen = TRUE;
  }
  fprintf(stderr, "[repro] foundRed=%d foundGreen=%d\n", foundRed, foundGreen);

  root->unref();

  if (!foundRed || !foundGreen) {
    fprintf(stderr, "[repro] FAIL: unit 0's red/green split by column was not reproduced "
                    "-- unit 0 is being sampled with the wrong (unit 1's) texture coordinates\n");
    return 1;
  }
  fprintf(stderr, "[repro] PASS\n");
  return 0;
}
