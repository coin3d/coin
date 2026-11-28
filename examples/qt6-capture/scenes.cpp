#include "scenes.h"

#include "Alternate.h"
#include "Glow.h"
#include "Pyramid.h"

#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTranslation.h>
#include <QDir>
#include <QFile>

SoSeparator *makeStellatedDodecahedron();

SoSeparator *readScene(const QString &path)
{
  SoInput input;
  return input.openFile(QFile::encodeName(path).constData()) ? SoDB::readAll(&input) : nullptr;
}

namespace {

SoSeparator *helloCone(const QString &)
{
  auto *root = new SoSeparator;
  auto *red = new SoMaterial;
  red->diffuseColor.setValue(1, 0, 0);
  root->addChild(red);
  root->addChild(new SoCone);
  return root;
}

SoSeparator *transformOrdering(const QString &directory)
{
  auto *temple = readScene(QDir(directory).filePath("data/temple.iv"));
  if (!temple) return nullptr;

  auto *root = new SoSeparator;
  auto *rotation = new SoRotationXYZ;
  rotation->axis = SoRotationXYZ::X;
  rotation->angle = 1.57079632679f;
  auto *scale = new SoScale;
  scale->scaleFactor.setValue(2, 1, 3);

  auto *left = new SoSeparator;
  auto *leftTranslation = new SoTranslation;
  leftTranslation->translation.setValue(-1, 0, 0);
  left->addChild(leftTranslation);
  left->addChild(rotation);
  left->addChild(scale);
  left->addChild(temple);
  root->addChild(left);

  auto *right = new SoSeparator;
  auto *rightTranslation = new SoTranslation;
  rightTranslation->translation.setValue(1, 0, 0);
  right->addChild(rightTranslation);
  right->addChild(scale);
  right->addChild(rotation);
  right->addChild(temple);
  root->addChild(right);
  return root;
}

SoSeparator *indexedFaceSet(const QString &)
{
  return makeStellatedDodecahedron();
}

SoSeparator *pickAction(const QString &directory)
{
  auto *star = readScene(QDir(directory).filePath("data/star.iv"));
  if (!star) return nullptr;

  auto *root = new SoSeparator;
  auto *rotation = new SoRotationXYZ;
  rotation->axis = SoRotationXYZ::X;
  rotation->angle = 1.42799666f;
  root->addChild(rotation);
  root->addChild(star);

  auto *red = new SoMaterial;
  red->diffuseColor.setValue(1, 0, 0);
  auto *translation = new SoTranslation;
  translation->translation.setValue(1, 0, 1);
  root->addChild(red);
  root->addChild(translation);
  root->addChild(star);
  return root;
}

SoSeparator *toolmakerNodes(const QString &directory)
{
  Glow::initClass();
  Pyramid::initClass();
  Alternate::initClass();
  return readScene(QDir(directory).filePath("Toolmaker/02.Nodes/newNodes.iv"));
}

const ExampleScene examples[] = {
  {"hello-cone", "Hello Cone", "A material and a cone in a scene graph", helloCone},
  {"transform-ordering", "Transform ordering", "Scale then rotate on the left, reverse order on the right", transformOrdering},
  {"indexed-face-set", "Indexed face set", "Stellated dodecahedron", indexedFaceSet},
  {"pick-action", "Pick action", "Click either star to inspect the picked node", pickAction},
  {"toolmaker-nodes", "Toolmaker nodes", "A scene with custom Glow, Pyramid and Alternate nodes", toolmakerNodes},
};

}

const ExampleScene *findExample(const QString &name)
{
  for (const auto &example : examples) {
    if (name == QLatin1String(example.name)) return &example;
  }
  return nullptr;
}
