// Reproducer for a NULL-pointer dereference in
// SoVRMLIndexedFaceSet::generatePrimitives() (src/vrml97/IndexedFaceSet.cpp),
// the same "check the binding enum, not the index pointer" pattern fixed
// for SoIndexedFaceSet in the indexedfaceset-perface-indexed-null
// reproducer.
//
// SoVRMLIndexedFaceSet::findMaterialBinding() normally derives its
// internal PER_FACE_INDEXED binding only when this->colorIndex actually
// has values (see the file for the exact condition), which would make
// mindices non-NULL by construction in that path. But when
// SoOverrideElement::getMaterialBindingOverride() is set (a classic
// Inventor SoMaterialBinding node with setOverride(TRUE) placed ahead of
// this shape), the binding is read directly from
// SoMaterialBindingElement -- entirely independent of this node's own
// colorIndex field -- reaching the exact same NULL mindices/PER_FACE_INDEXED
// combination as the plain SoIndexedFaceSet bug.
//
// See run.sh in this directory for how to build and run this against a
// given libCoin build.

#include <cstdio>
#include <cstdlib>
#include <Inventor/SoDB.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/VRMLnodes/SoVRMLCoordinate.h>
#include <Inventor/VRMLnodes/SoVRMLIndexedFaceSet.h>

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

  SoSeparator * root = new SoSeparator;
  root->ref();

  // Classic-Inventor SoMaterialBinding with override forces
  // SoVRMLIndexedFaceSet::findMaterialBinding() down the state-only path,
  // regardless of this node's own color/colorIndex fields (which are left
  // entirely unset below).
  SoMaterialBinding * matbinding = new SoMaterialBinding;
  matbinding->value = SoMaterialBinding::PER_FACE_INDEXED;
  matbinding->setOverride(TRUE);
  root->addChild(matbinding);

  SoVRMLCoordinate * coord = new SoVRMLCoordinate;
  const SbVec3f pts[4] = {
    SbVec3f(0, 0, 0), SbVec3f(1, 0, 0), SbVec3f(1, 1, 0), SbVec3f(0, 1, 0)
  };
  coord->point.setValues(0, 4, pts);

  SoVRMLIndexedFaceSet * ifs = new SoVRMLIndexedFaceSet;
  ifs->coord = coord;
  const int32_t indices[5] = {0, 1, 2, 3, -1};
  ifs->coordIndex.setValues(0, 5, indices);
  // colorIndex intentionally left empty (default).
  root->addChild(ifs);

  SoCallbackAction action;
  action.addTriangleCallback(SoVRMLIndexedFaceSet::getClassTypeId(), triangle_cb, NULL);

  fprintf(stderr, "[repro] applying SoCallbackAction to an "
                  "SoVRMLIndexedFaceSet with an overriding materialBinding "
                  "PER_FACE_INDEXED and an empty colorIndex field\n");
  action.apply(root);

  root->unref();

  fprintf(stderr, "[repro] PASS: survived without crashing\n");
  return 0;
}
