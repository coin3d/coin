/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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
  \brief The SoNonIndexedShape class is the superclass for all non-indexed vertex based shapes.
  \ingroup nodes
  
  It contains the (now obsoleted) startIndex field and a convenience method for 
  calculating the bounding box.
*/

#include <Inventor/nodes/SoNonIndexedShape.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/elements/SoCoordinateElement.h>

/*!  
  \var SoSFInt32 SoNonIndexedShape::startIndex 
  Coordinates are fetched from this index on. This field is now obsoleted, and 
  is provided only for backward compatibility.  
*/


SO_NODE_ABSTRACT_SOURCE(SoNonIndexedShape);

/*!
  Constructor.
*/
SoNonIndexedShape::SoNonIndexedShape()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoNonIndexedShape);

  SO_NODE_ADD_FIELD(startIndex, (0));
}

/*!
  Destructor.
*/
SoNonIndexedShape::~SoNonIndexedShape()
{
}

// doc from parent
void
SoNonIndexedShape::initClass()
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoNonIndexedShape);
}

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
  const SoCoordinateElement *coordelem =
    SoCoordinateElement::getInstance(action->getState());

  SoVertexProperty * vp = (SoVertexProperty *) this->vertexProperty.getValue();
  assert(!vp ||
         vp->getTypeId().isDerivedFrom(SoVertexProperty::getClassTypeId()));
  SbBool vpvtx = vp && (vp->vertex.getNum() > 0);

  const int numCoords = vpvtx ?
    vp->vertex.getNum() :
    coordelem->getNum();

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
    coordelem->getArrayPtr3();

  center.setValue(0.0f, 0.0f, 0.0f);
  for (int i = startidx; i <= lastidx; i++) {
    box.extendBy(coords[i]);
    center += coords[i];
  }

  center /= float(lastidx + 1 - startidx);
}

/*!  
  Convenience method that might adjust \a start and \a end
  pointers, which should point at the start and end of the numVertices
  array when calling this method. This takes care of the case where
  numVertices contains a single -1, and all coordinates in the state
  (or in the vertexProperty field) should be rendered as one
  primitive.

  \a dummyarray should be a temporary array, with room for one integer.

  Not part of the OIV API.  
*/
void
SoNonIndexedShape::fixNumVerticesPointers(SoState *state, const int32_t *&start, const int32_t *&end,
                                          int32_t *dummyarray) const
{
  if ((start + 1 == end) && (*start == -1)) {
    const SoCoordinateElement *coordelem =
      SoCoordinateElement::getInstance(state);
    SoVertexProperty * vp = (SoVertexProperty *) this->vertexProperty.getValue();
    SbBool vpvtx = vp && (vp->vertex.getNum() > 0);

    const int numCoords = vpvtx ?
      vp->vertex.getNum() :
      coordelem->getNum();

    dummyarray[0] = numCoords - startIndex.getValue();
    start = dummyarray;
    end = numCoords > 1 ? start + 1 : start;
  }
}
