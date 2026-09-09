// Reproducer for a NULL-pointer dereference in
// SoIndexedTriangleStripSet::generatePrimitives()
// (src/shapenodes/SoIndexedTriangleStripSet.cpp), the same "check the
// binding enum, not the index pointer" pattern fixed for SoIndexedFaceSet
// in the indexedfaceset-perface-indexed-null reproducer, replicated here
// for PER_STRIP_INDEXED/PER_TRIANGLE_INDEXED instead of
// PER_FACE_INDEXED/PER_PART_INDEXED.
//
// materialBinding PER_PART_INDEXED (mapped to the internal
// PER_STRIP_INDEXED binding by SoIndexedTriangleStripSet::
// findMaterialBinding()) with no materialIndex field set on the shape
// itself is enough to SIGSEGV: the binding comes purely from the state (a
// sibling SoMaterialBinding node), independent of whether materialIndex
// has any values, and the NULL-index fallback in generatePrimitives()
// only covers PER_VERTEX_INDEXED.
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
#include <Inventor/nodes/SoIndexedTriangleStripSet.h>
#include <Inventor/nodes/SoMaterialBinding.h>

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

  // PER_PART_INDEXED -> internal PER_STRIP_INDEXED binding, with no
  // materialIndex field set on the shape below.
  SoMaterialBinding * matbinding = new SoMaterialBinding;
  matbinding->value = SoMaterialBinding::PER_PART_INDEXED;
  root->addChild(matbinding);

  SoCoordinate3 * coord = new SoCoordinate3;
  const SbVec3f pts[4] = {
    SbVec3f(0, 0, 0), SbVec3f(1, 0, 0), SbVec3f(1, 1, 0), SbVec3f(0, 1, 0)
  };
  coord->point.setValues(0, 4, pts);
  root->addChild(coord);

  SoIndexedTriangleStripSet * its = new SoIndexedTriangleStripSet;
  const int32_t indices[5] = {0, 1, 2, 3, -1};
  its->coordIndex.setValues(0, 5, indices);
  // materialIndex intentionally left empty (default).
  root->addChild(its);

  SoCallbackAction action;
  action.addTriangleCallback(SoIndexedTriangleStripSet::getClassTypeId(), triangle_cb, NULL);

  fprintf(stderr, "[repro] applying SoCallbackAction to an "
                  "SoIndexedTriangleStripSet with materialBinding "
                  "PER_PART_INDEXED and an empty materialIndex field\n");
  action.apply(root);

  root->unref();

  fprintf(stderr, "[repro] PASS: survived without crashing\n");
  return 0;
}
