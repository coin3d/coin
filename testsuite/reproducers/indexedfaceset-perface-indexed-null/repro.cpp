// Reproducer for a NULL-pointer dereference in
// SoIndexedFaceSet::generatePrimitives() (src/shapenodes/SoIndexedFaceSet.cpp)
// when materialBinding (or normalBinding) is set to PER_FACE_INDEXED (or
// PER_PART_INDEXED) but the corresponding materialIndex/normalIndex field
// on the SoIndexedFaceSet itself is left at its default (empty) value.
//
// SoIndexedShape::getVertexData() (src/shapenodes/SoIndexedShape.cpp) sets
// mindices/nindices to NULL whenever the materialIndex/normalIndex field is
// empty -- entirely independent of the current material/normal binding,
// which comes from a sibling SoMaterialBinding/SoNormalBinding node via
// SoMaterialBindingElement/SoNormalBindingElement. generatePrimitives()
// only re-derives a fallback index array (pointing it at coordIndex) when
// the binding is PER_VERTEX_INDEXED:
//
//   if (mbind == PER_VERTEX_INDEXED && mindices == NULL) {
//     mindices = cindices;
//   }
//
// but then unconditionally dereferences mindices/nindices for BOTH
// PER_VERTEX_INDEXED and PER_FACE_INDEXED:
//
//   else if (mbind == PER_VERTEX_INDEXED || mbind == PER_FACE_INDEXED) {
//     pointDetail.setMaterialIndex(*mindices);   // line 851 -- NULL deref
//     vertex.setMaterialIndex(*mindices++);
//   }
//
// So "materialBinding PER_FACE_INDEXED" with no materialIndex field set is
// enough to SIGSEGV in a normal (non-debug-assert) build; a Debug build hits
// this before any assert would have fired, since nothing here is
// assert()-guarded at all -- this is a plain unchecked pointer dereference.
//
// This reaches generatePrimitives() via SoCallbackAction with a triangle
// callback registered for SoIndexedFaceSet, which is the standard public
// way to force primitive generation without depending on GL context or
// picking geometry.
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
#include <Inventor/nodes/SoIndexedFaceSet.h>
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

  // PER_FACE_INDEXED with no materialIndex field set on the shape below --
  // findMaterialBinding() will report PER_FACE_INDEXED purely from this
  // node's state, regardless of whether materialIndex has any values.
  SoMaterialBinding * matbinding = new SoMaterialBinding;
  matbinding->value = SoMaterialBinding::PER_FACE_INDEXED;
  root->addChild(matbinding);

  SoCoordinate3 * coord = new SoCoordinate3;
  const SbVec3f pts[4] = {
    SbVec3f(0, 0, 0), SbVec3f(1, 0, 0), SbVec3f(1, 1, 0), SbVec3f(0, 1, 0)
  };
  coord->point.setValues(0, 4, pts);
  root->addChild(coord);

  SoIndexedFaceSet * ifs = new SoIndexedFaceSet;
  const int32_t indices[5] = {0, 1, 2, 3, -1};
  ifs->coordIndex.setValues(0, 5, indices);
  // materialIndex intentionally left empty (default): this is the
  // malformed-but-not-otherwise-invalid state that reaches the NULL deref.
  root->addChild(ifs);

  SoCallbackAction action;
  action.addTriangleCallback(SoIndexedFaceSet::getClassTypeId(), triangle_cb, NULL);

  fprintf(stderr, "[repro] applying SoCallbackAction to an SoIndexedFaceSet "
                  "with materialBinding PER_FACE_INDEXED and an empty "
                  "materialIndex field\n");
  action.apply(root);

  root->unref();

  fprintf(stderr, "[repro] PASS: survived without crashing\n");
  return 0;
}
