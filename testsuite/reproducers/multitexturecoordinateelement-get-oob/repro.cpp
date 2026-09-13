// Reproducer for an out-of-bounds array read in
// SoMultiTextureCoordinateElement::get2()/get3()/get4()
// (src/elements/SoMultiTextureCoordinateElement.cpp) when a shape's
// textureCoordIndex field contains an index that is out of range for the
// actual number of texture coordinates supplied by the bound
// SoTextureCoordinate2/3 node.
//
// Each accessor bounds-checks the index with nothing but a Debug-only
// assert:
//
//   const SbVec4f &
//   SoMultiTextureCoordinateElement::get4(const int unit, const int index) const
//   {
//     assert(unit < PRIVATE(this)->unitdata.getLength());
//     const UnitData & ud = PRIVATE(this)->unitdata[unit];
//     assert(index >= 0 && index < ud.numCoords);   // compiled away under NDEBUG
//     assert(ud.whatKind == EXPLICIT);
//     if (ud.coordsDimension==4) {
//       return ud.coords4[index];                    // OOB read in Release
//     ...
//
// and nothing in the call chain that reaches it from ordinary shape
// traversal validates the index first:
// SoTextureCoordinateBundle::get(index) (src/bundles/SoTextureCoordinateBundle.cpp)
// forwards straight through to coordElt->get4(index), and generatePrimitives()
// in every indexed shape (e.g. SoIndexedFaceSet) forwards the shape's own
// textureCoordIndex field values -- entirely file/application controlled,
// with no relation enforced to the number of coordinates actually present in
// the currently bound SoTextureCoordinate2 node -- straight into that call.
//
// So a scene graph with a 2-entry SoTextureCoordinate2 and an
// SoIndexedFaceSet whose textureCoordIndex references index 5 is enough to
// hit this: an assert() abort in a Debug build, and a silent out-of-bounds
// heap read in Release (UBSan/ASan catch it either way).
//
// This reaches generatePrimitives() via SoCallbackAction with a triangle
// callback registered for SoIndexedFaceSet, the same technique used by the
// sibling indexedfaceset-perface-indexed-null reproducer.
//
// See run.sh in this directory for how to build and run this against a
// given libCoin build.

#include "../CoinCleanup.h"
#include <cstdio>
#include <cstdlib>
#include <Inventor/SoDB.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>

static void
triangle_cb(void *, SoCallbackAction *,
            const SoPrimitiveVertex *,
            const SoPrimitiveVertex *,
            const SoPrimitiveVertex *)
{
  // Reaching a triangle at all means generatePrimitives() survived --
  // nothing to do here, the crash (if any) happens before this runs.
}

int main()
{
  SoDB::init();
  CoinReproducerCleanup cleanup;

  SoSeparator * root = new SoSeparator;
  root->ref();

  // A texture must be enabled for the state for SoTextureCoordinateBundle
  // to bother fetching texture coordinates at all -- an in-memory 1x1
  // image is enough, no file/GL context needed for SoCallbackAction.
  SoTexture2 * tex = new SoTexture2;
  static const unsigned char pixel[3] = {255, 255, 255};
  tex->image.setValue(SbVec2s(1, 1), 3, pixel);
  root->addChild(tex);

  // Only 2 texture coordinates supplied ...
  SoTextureCoordinate2 * tc = new SoTextureCoordinate2;
  const SbVec2f tcpts[2] = { SbVec2f(0, 0), SbVec2f(1, 1) };
  tc->point.setValues(0, 2, tcpts);
  root->addChild(tc);

  SoCoordinate3 * coord = new SoCoordinate3;
  const SbVec3f pts[4] = {
    SbVec3f(0, 0, 0), SbVec3f(1, 0, 0), SbVec3f(1, 1, 0), SbVec3f(0, 1, 0)
  };
  coord->point.setValues(0, 4, pts);
  root->addChild(coord);

  SoIndexedFaceSet * ifs = new SoIndexedFaceSet;
  const int32_t coordindex[5] = {0, 1, 2, 3, -1};
  ifs->coordIndex.setValues(0, 5, coordindex);
  // ... but textureCoordIndex references index 5, which is out of range.
  const int32_t texcoordindex[5] = {0, 1, 5, 0, -1};
  ifs->textureCoordIndex.setValues(0, 5, texcoordindex);
  root->addChild(ifs);

  SoCallbackAction action;
  action.addTriangleCallback(SoIndexedFaceSet::getClassTypeId(), triangle_cb, NULL);

  fprintf(stderr, "[repro] applying SoCallbackAction to an SoIndexedFaceSet "
                  "with textureCoordIndex referencing index 5 into a "
                  "2-entry SoTextureCoordinate2\n");
  action.apply(root);

  root->unref();

  fprintf(stderr, "[repro] PASS: survived without an out-of-bounds access\n");
  return 0;
}
