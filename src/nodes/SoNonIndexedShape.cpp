/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
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
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoNonIndexedShape, SO_FROM_INVENTOR_2_1);
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

  int32_t startidx = this->startIndex.getValue();
  int32_t lastidx;

  if (numVertices < 0) lastidx = numCoords - 1;
  else lastidx = startidx + numVertices-1;

  if (numCoords <= 0 || lastidx >= numCoords) {
    // no need to call box.makeEmpty(). Box will be empty when
    // this method is called.
    return;
  }

  center.setValue(0.0f, 0.0f, 0.0f);
  
  if (vpvtx || coordelem->is3D()) {
    const SbVec3f * coords = vpvtx ?
      vp->vertex.getValues(0) :
      coordelem->getArrayPtr3();
    
    for (int i = startidx; i <= lastidx; i++) {
      box.extendBy(coords[i]);
      center += coords[i];
    }
  }
  else { // 4D
    const SbVec4f * coords = coordelem->getArrayPtr4();
    for (int i = startidx; i <= lastidx; i++) {
      SbVec4f h = coords[i];
      float mul = 1.0f / h[3];
      SbVec3f c(h[0]*mul, h[1]*mul, h[2]*mul);
      box.extendBy(c);
      center += c;
    }
  }
  if (lastidx+1 - startidx) {
    center /= float(lastidx + 1 - startidx);
  }
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
