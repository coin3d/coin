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
  \class SoNonIndexedShape SoNonIndexedShape.h Inventor/nodes/SoNonIndexedShape.h
  \brief The SoNonIndexedShape class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoNonIndexedShape.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/SbName.h>

#if !defined(COIN_EXCLUDE_SOACTION)
#include <Inventor/actions/SoAction.h>
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCOORDINATEELEMENT)
#include <Inventor/elements/SoCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOCOORDINATEELEMENT

/*!
  \var SoSFInt32 SoNonIndexedShape::startIndex
  FIXME: write documentation for field
*/


//$ BEGIN TEMPLATE NodeAbstractSource(SoNonIndexedShape)
SoType SoNonIndexedShape::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the SoNonIndexedShape class.
*/
SoType
SoNonIndexedShape::getClassTypeId(void)
{
  return SoNonIndexedShape::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoNonIndexedShape::getTypeId(void) const
{
  return SoNonIndexedShape::classTypeId;
}
//$ END TEMPLATE NodeAbstractSource

/*!
  Constructor.
*/
SoNonIndexedShape::SoNonIndexedShape()
{
//$ BEGIN TEMPLATE NodeConstructor(SoNonIndexedShape)
  // Make sure the class has been initialized.
  assert(SoNonIndexedShape::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(startIndex, (0));
}

/*!
  Destructor.
*/
SoNonIndexedShape::~SoNonIndexedShape()
{
}

/*!
  Does initialization common for all objects of the
  SoNonIndexedShape class. This includes setting up the
  type system, among other things.
*/
void 
SoNonIndexedShape::initClass()
{
//$ BEGIN TEMPLATE InitNodeAbstractSource(NonIndexedShape)
  // Make sure we only initialize once.
  assert(SoNonIndexedShape::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoNonIndexedShape::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "NonIndexedShape",
                       NULL,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeAbstractSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void 
SoNonIndexedShape::cleanClass()
{
}

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  This method is provided as a convenient means for the subclasses of this
  class to find their bounding box and center value.

  The returned bounding box will enclose all vertices from \a startIndex
  up to \a startIndex + \a numVertices. If \a numVertices is less than
  zero, \e all vertices in the current coordinate element or vertex property
  node will be used in the calculation.

  The \a center point will be calculated as the average of all the vertices
  in the bounding box.
 */
void
SoNonIndexedShape::computeCoordBBox(SoAction * action, int numVertices,
				    SbBox3f & box, SbVec3f & center)
{
  SoVertexProperty * vp = (SoVertexProperty *) this->vertexProperty.getValue();
  assert(!vp ||
	 vp->getTypeId().isDerivedFrom(SoVertexProperty::getClassTypeId()));
  SbBool vpvtx = vp && (vp->vertex.getNum() > 0);

  const int numCoords = vpvtx ?
    vp->vertex.getNum() :
    SoCoordinateElement::getInstance(action->getState())->getNum();

  // FIXME: handle assertion more gracefully. 19990327 mortene.
  assert(numCoords > 0);

  int32_t startidx = this->startIndex.getValue();
  int32_t lastidx;

  if (numVertices < 0) lastidx = numCoords - 1;
  else lastidx = startidx + numVertices-1;

  // FIXME: handle assertion more gracefully. 19990327 mortene.
  assert(lastidx < numCoords);

  const SbVec3f * coords = vpvtx ?
    vp->vertex.getValues(0) :
    SoCoordinateElement::getArrayPtr3(action->getState());

  center.setValue(0.0f, 0.0f, 0.0f);
  for (int i = startidx; i <= lastidx; i++) {
    box.extendBy(coords[i]);
    center += coords[i];
  }

  center /= lastidx + 1 - startidx;
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

