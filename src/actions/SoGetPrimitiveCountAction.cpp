/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoGetPrimitiveCountAction Inventor/actions/SoGetPrimitiveCountAction.h
  \brief The SoGetPrimitiveCountAction class is for getting the number of
  primitives a subgraph consists of.

  FIXME: code MiA. 19990315 mortene.
*/

#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/SbName.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/lists/SoEnabledElementsList.h>

// *************************************************************************

//$ BEGIN TEMPLATE ActionSource( SoGetPrimitiveCountAction )
//$ BEGIN TEMPLATE ActionClassTypeSource( SoGetPrimitiveCountAction )

SoType SoGetPrimitiveCountAction::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the classname class.
*/
SoType
SoGetPrimitiveCountAction::getClassTypeId(void)
{
  return classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoGetPrimitiveCountAction::getTypeId(void) const
{
  return classTypeId;
}
//$ END TEMPLATE ActionClassTypeSource

#include <assert.h>

// static variables
SoEnabledElementsList * SoGetPrimitiveCountAction::enabledElements;
SoActionMethodList * SoGetPrimitiveCountAction::methods;

/*!
  \fn SoGetPrimitiveCountAction::enabledElements
  FIXME: write doc.
*/

/*!
  \fn SoGetPrimitiveCountAction::methods
  FIXME: write doc.
*/

/*!
  This method returns the list of enabled elements for the given action class.
*/
const SoEnabledElementsList &
SoGetPrimitiveCountAction::getEnabledElements(void) const
{
  assert(enabledElements);
  return *enabledElements;
}

/*!
  This method adds a method to be perfomed by the action class on the given
  node type.
*/
void 
SoGetPrimitiveCountAction::addMethod(const SoType type, SoActionMethod method)
{
  assert(methods);
  methods->addMethod(type, method);
}

/*!
  This method enables an element in the state stack for the action class.
*/
void 
SoGetPrimitiveCountAction::enableElement(const SoType type, const int stackIndex)
{
  assert(enabledElements);
  enabledElements->enable(type, stackIndex);
}
//$ END TEMPLATE ActionSource

// *************************************************************************

/*!
  This static method initializes the static data for the
  SoGetPrimitiveCountAction class.
*/

void
SoGetPrimitiveCountAction::initClass(void)
{
//$ BEGIN TEMPLATE InitActionSource( SoGetPrimitiveCountAction )
  assert(SoGetPrimitiveCountAction::getClassTypeId() == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGetPrimitiveCountAction::classTypeId = 
      SoType::createType(inherited::getClassTypeId(),
                         "SoGetPrimitiveCountAction");
  enabledElements = new SoEnabledElementsList(inherited::enabledElements);
  methods = new SoActionMethodList(inherited::methods);
//$ END TEMPLATE InitActionSource

  methods->setDefault((void *)SoNode::getPrimitiveCountS);
}

// *************************************************************************

/*!
  A constructor.
*/

SoGetPrimitiveCountAction::SoGetPrimitiveCountAction(void)
{
  SO_ACTION_CONSTRUCTOR(SoGetPrimitiveCountAction);  

  static int first = 1;
  if (first) {
    first = 0;
    SO_ACTION_ADD_METHOD(SoNode, SoNode::getPrimitiveCountS);
  }
  SoGetPrimitiveCountAction::methods->setUp();

  this->textAsTris = TRUE;
  this->approx = FALSE;
  this->nonVertexAsTris = TRUE;
}

/*!
  The destructor.
*/
SoGetPrimitiveCountAction::~SoGetPrimitiveCountAction()
{
}

/*!
  Returns number of triangles in graph.
*/
int 
SoGetPrimitiveCountAction::getTriangleCount() const
{
  return this->numTris;
}

/*!
  Returns number of lines in graph.
*/
int 
SoGetPrimitiveCountAction::getLineCount() const
{
  return this->numLines;
}

/*!
  Returns number of points in graph.
*/
int 
SoGetPrimitiveCountAction::getPointCount() const
{
  return this->numPoints;
}

/*!
  Returns number of texts in the graph.
*/
int 
SoGetPrimitiveCountAction::getTextCount() const
{
  return this->numTexts;
}

/*!
  Returns the number of images in the graph.
*/
int 
SoGetPrimitiveCountAction::getImageCount() const
{
  return this->numImages;
}

/*!
  Returns whether there are primitives in graph.
*/
SbBool 
SoGetPrimitiveCountAction::containsNoPrimitives()
{
  return 
    this->numTris == 0 &&
    this->numLines == 0 &&
    this->numPoints == 0 &&
    this->numTexts == 0 &&
    this->numImages == 0 &&
    this->numCubes == 0 &&
    this->numCones == 0 &&
    this->numCylinders == 0 &&
    this->numSpheres == 0;
}

/*!
  Returns whether there are non-triangular primitives in graph.
*/
SbBool 
SoGetPrimitiveCountAction::containsNonTriangleShapes()
{
  return 
    this->numLines != 0 ||
    this->numPoints != 0 ||
    this->numTexts != 0 ||
    this->numImages != 0 ||
    this->numCubes != 0 ||
    this->numCones != 0 ||
    this->numCylinders != 0 ||
    this->numSpheres != 0;
}

/*!
  Returns whether Text3 nodes text is counted as triangles or texts. The
  default is to count as triangles.
  \sa SoGetPrimitiveCountAction::is3DTextCountedAsTriangles()
*/
SbBool 
SoGetPrimitiveCountAction::is3DTextCountedAsTriangles()
{
  return this->textAsTris;
}

/*!
  Sets whether Text3 nodes text is counted as triangles or texts.
  \sa SoGetPrimitiveCountAction::is3DTextCountedAsTriangles()
*/
void 
SoGetPrimitiveCountAction::setCount3DTextAsTriangles(const SbBool onOff)
{
  this->textAsTris = onOff;
}

/*!
  Returns whether shapes can use an approximate value when counting
  primitives. This is much faster than doing an accurate count.
  The default is not to approximate.

  \sa SoGetPrimitiveCountAction::setCanApproximate()
*/
SbBool 
SoGetPrimitiveCountAction::canApproximateCount()
{
  return this->approx;
}

/*!
  Sets whether shapes can do an approximate count.
  \sa SoGetPrimitiveCountAction::canApproximateCount()
*/
void 
SoGetPrimitiveCountAction::setCanApproximate(const SbBool onOff)
{
  this->approx = onOff;
}

/*!
  Adds number of triangles.
*/
void 
SoGetPrimitiveCountAction::addNumTriangles(const int num)
{
  this->numTris += num;
}

/*!
  Adds number of lines.
*/
void 
SoGetPrimitiveCountAction::addNumLines(const int num)
{
  this->numLines += num;
}

/*!
  Adds number of points.
*/
void 
SoGetPrimitiveCountAction::addNumPoints(const int num)
{
  this->numPoints = num;
}

/*!
  Adds number of texts.
*/
void 
SoGetPrimitiveCountAction::addNumText(const int num)
{
  this->numTexts += num;
}

/*!
  Adds number of images.
*/
void 
SoGetPrimitiveCountAction::addNumImage(const int num)
{
  this->numImages += num;
}

/*!
  Increments number of triangles.
*/
void 
SoGetPrimitiveCountAction::incNumTriangles()
{
  this->numTris++;
}

/*!
  Increments number of lines.
*/
void 
SoGetPrimitiveCountAction::incNumLines()
{
  this->numLines++;
}

/*!
  Increments number of points.
*/
void 
SoGetPrimitiveCountAction::incNumPoints()
{
  this->numPoints++;
}

/*!
  Increments number of texts.
*/
void 
SoGetPrimitiveCountAction::incNumText()
{
  this->numTexts++;
}

/*!
  Increments number of images.
*/
void 
SoGetPrimitiveCountAction::incNumImage()
{
  this->numImages++;
}

/*!
  Returns whether non vertex shapes should be counted as triangles.
  
  This method is not part of the OIV API.

  \sa SoGetPrimitiveCountAction::isNonVertexShapesCountedAsTriangles() 
*/
SbBool 
SoGetPrimitiveCountAction::isNonVertexShapesCountedAsTriangles() const
{
  return this->nonVertexAsTris;
}

/*!
  Sets whether non vertex shapes should be counted as triangles.
  The default is to count non-vertex shapes as triangles.
  
  This method is not part of the OIV API.

  \sa SoGetPrimitiveCountAction::isNonVertexShapesCountedAsTriangles() 
*/  
void 
SoGetPrimitiveCountAction::setCountNonVertexShapesAsTriangles(const SbBool onOff)
{
  this->nonVertexAsTris = onOff;
}

/*!
  Returns number of cubes in graph.

  This method is not part of the OIV API.  
*/ 
int 
SoGetPrimitiveCountAction::getCubeCount() const
{
  return this->numCubes;
}

/*!
  Returns number of cylinders in graph.

  This method is not part of the OIV API.
*/
int 
SoGetPrimitiveCountAction::getCylinderCount() const
{
  return this->numCylinders;
}

/*!
  Returns number of cones in graph.

  This method is not part of the OIV API.
*/
int 
SoGetPrimitiveCountAction::getConeCount() const
{
  return this->numCones;
}

/*!
  Returns number of spheres in graph.

  This method is not part of the OIV API.
*/ 
int 
SoGetPrimitiveCountAction::getSphereCount() const
{
  return this->numSpheres;
}

/*!
  Increments number of cubes.

  This method is not part of the OIV API.
*/
void 
SoGetPrimitiveCountAction::incNumCubes()
{
  this->numCubes++;
}

/*!
  Increments number of cylinders.

  This method is not part of the OIV API.
*/
void 
SoGetPrimitiveCountAction::incNumCylinders()
{
  this->numCylinders++;
}

/*!
  Increments number of cones.

  This method is not part of the OIV API.
*/
void 
SoGetPrimitiveCountAction::incNumCones()
{
  this->numCones++;
}

/*!
  Increments number of spheres.

  This method is not part of the OIV API.
*/
void 
SoGetPrimitiveCountAction::incNumSpheres()
{
  this->numSpheres++;
}

/*!
  This method is called at the beginning of the traversal.
*/
void
SoGetPrimitiveCountAction::beginTraversal(SoNode * node)
{
  this->numTris = 0;
  this->numLines = 0;
  this->numPoints = 0;
  this->numTexts = 0;
  this->numImages = 0;
  this->numCubes = 0;
  this->numCylinders = 0;
  this->numCones = 0;
  this->numSpheres = 0;

  this->traverse(node);
}

