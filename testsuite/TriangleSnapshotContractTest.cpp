#include "TriangleSnapshotTestDatabase.h"
#include <Inventor/SoTriangleSnapshot.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoSeparator.h>
#include <cstdio>
#include <stdexcept>
#include <string>

namespace {
class ChangingShape : public SoFaceSet {
public:
  bool changeMatrix = false;
  bool invalidMaterial = false;
  bool completed = false;
protected:
  void generatePrimitives(SoAction * action) override
  {
    SoPrimitiveVertex vertices[3];
    for (int i = 0; i < 3; ++i) {
      vertices[i].setPoint(SbVec3f(float(i == 1),float(i == 2),0));
      vertices[i].setNormal(SbVec3f(0,0,1));
      vertices[i].setMaterialIndex(invalidMaterial ? 100 : 0);
    }
    this->invokeTriangleCallbacks(action, &vertices[0], &vertices[1], &vertices[2]);
    if (changeMatrix) {
      SoModelMatrixElement::translateBy(action->getState(), this, SbVec3f(5,0,0));
      this->invokeTriangleCallbacks(action, &vertices[0], &vertices[1], &vertices[2]);
    }
    completed = true;
  }
};
SoCallbackAction::Response stop(void *, SoCallbackAction *, const SoNode *)
{
  return SoCallbackAction::ABORT;
}
void abortAction(void *, SoAction * action)
{
  static_cast<SoCallbackAction *>(action)->addPreCallback(SoFaceSet::getClassTypeId(), stop, nullptr);
}
}

int main(int argc, char ** argv)
{
  if (argc != 2) return 10;
  SnapshotTestDatabase database;
  const std::string mode = argv[1];
  auto * root = new SoSeparator;
  root->ref();
  auto * shape = new ChangingShape;
  shape->changeMatrix = mode == "transform";
  shape->invalidMaterial = mode == "material";
  root->addChild(shape);
  if (mode == "abort") {
    auto * stopper = new SoCallback;
    stopper->setCallback(abortAction, nullptr);
    root->addChild(stopper);
    root->addChild(new SoFaceSet);
  }
  bool rejected = false;
  try {
    SoTriangleSnapshot snapshot(*root,SbViewportRegion(64,64));
    if (mode == "bounds") {
      try { snapshot.getTriangle(snapshot.getNumTriangles()); }
      catch (const std::out_of_range &) { rejected = true; }
    }
  }
  catch (const std::out_of_range &) { rejected = mode == "material"; }
  catch (const std::runtime_error &) { rejected = mode == "transform" || mode == "abort"; }
  const bool completed = shape->completed;
  // Failure must not poison a later capture of the repaired graph.
  while (root->getNumChildren() > 1) root->removeChild(1);
  shape->changeMatrix = false;
  shape->invalidMaterial = false;
  SoTriangleSnapshot recovered(*root, SbViewportRegion(64,64));
  const bool recovery = recovered.getNumTriangles() == 1 &&
                        recovered.getNumOccurrences() == 1;
  bool matrixBounds = false;
  try { recovered.getModelMatrix(recovered.getNumOccurrences()); }
  catch (const std::out_of_range &) { matrixBounds = true; }
  root->removeChild(0);
  SoTriangleSnapshot empty(*root, SbViewportRegion(64,64));
  const bool emptyCounts = empty.getNumTriangles() == 0 &&
                           empty.getNumOccurrences() == 0;
  root->unref();
  if (!rejected || !completed || !recovery || !matrixBounds || !emptyCounts) {
    std::fprintf(stderr,"%s rejected=%d completed=%d\n",mode.c_str(),int(rejected),int(completed));
    return 1;
  }
  std::printf("%s rejected after producer completed\n",mode.c_str());
  return 0;
}
