/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoVRMLIndexedShape SoVRMLIndexedShape.h Inventor/VRMLnodes/SoVRMLIndexedShape.h
  \brief The SoVRMLIndexedShape class is a superclass for geometry that use indexes.
*/

/*!
  \var SoMFInt32 SoVRMLIndexedShape::coordIndex
  The coordinate index array.
*/

/*!
  \var SoMFInt32 SoVRMLIndexedShape::colorIndex
  The color index array.
*/

/*!
  \var SoMFInt32 SoVRMLIndexedShape::normalIndex
  The normal index array.
*/

/*!
  \var SoMFInt32 SoVRMLIndexedShape::texCoordIndex
  The texture coordinate index array.
*/

#include <Inventor/VRMLnodes/SoVRMLIndexedShape.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/VRMLnodes/SoVRMLCoordinate.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/errors/SoDebugError.h>

SO_NODE_ABSTRACT_SOURCE(SoVRMLIndexedShape);

// Doc in parent
void 
SoVRMLIndexedShape::initClass(void)
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoVRMLIndexedShape, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLIndexedShape::SoVRMLIndexedShape(void)
{
  SO_VRMLNODE_INTERNAL_CONSTRUCTOR(SoVRMLIndexedShape);

  SO_VRMLNODE_ADD_EMPTY_MFIELD(coordIndex);
  SO_VRMLNODE_ADD_EMPTY_MFIELD(colorIndex);
  SO_VRMLNODE_ADD_EMPTY_MFIELD(normalIndex);
  SO_VRMLNODE_ADD_EMPTY_MFIELD(texCoordIndex);
}

/*!
  Destructor.
*/
SoVRMLIndexedShape::~SoVRMLIndexedShape()
{
}

/*!
  Convenience method that will fetch data needed for rendering or
  generating primitives. Takes care of normal cache.
*/
SbBool
SoVRMLIndexedShape::getVertexData(SoState * state,
                                  const SoCoordinateElement *& coords,
                                  const SbVec3f *& normals,
                                  const int32_t *& cindices,
                                  const int32_t *& nindices,
                                  const int32_t *& tindices,
                                  const int32_t *& mindices,
                                  int & numcindices,
                                  const SbBool neednormals,
                                  SbBool & normalcacheused)
{
  SoVRMLVertexShape::getVertexData(state, coords, normals, neednormals);

  cindices = this->coordIndex.getValues(0);
  numcindices = this->coordIndex.getNum();

  mindices = this->colorIndex.getValues(0);
  if (this->colorIndex.getNum() <= 0 || mindices[0] < 0) mindices = NULL;

  tindices = this->texCoordIndex.getValues(0);
  if (this->texCoordIndex.getNum() <= 0 || tindices[0] < 0) tindices = NULL;

  normalcacheused = FALSE;

  if (neednormals) {
    nindices = this->normalIndex.getValues(0);
    if (this->normalIndex.getNum() <= 0 || nindices[0] < 0) nindices = NULL;
    
    if (normals == NULL) {
      SoNormalCache * nc = this->generateAndReadLockNormalCache(state);
      normals = nc->getNormals();
      nindices = nc->getIndices();
      normalcacheused = TRUE;
      // if no normals were generated, unlock normal cache before
      // returning
      if (normals == NULL) {
        this->readUnlockNormalCache();
        normalcacheused = FALSE;
      }
    }
  }
  return TRUE;
}

// Doc in parent
void
SoVRMLIndexedShape::notify(SoNotList * list)
{
  SoField * f = list->getLastField();
  if (f == &this->coordIndex) {
    SoNormalCache * nc = this->getNormalCache();
    if (nc) nc->invalidate();
  }
  inherited::notify(list);
}

// Doc in parent
void
SoVRMLIndexedShape::computeBBox(SoAction * action, SbBox3f & box,
                                SbVec3f & center)
{
  SoVRMLCoordinate * node = (SoVRMLCoordinate*) this->coord.getValue();
  if (node == NULL) return;

  int numCoords = node->point.getNum();
  const SbVec3f * coords = node->point.getValues(0);

  box.makeEmpty();
  const int32_t * ptr = coordIndex.getValues(0);
  const int32_t * endptr = ptr + coordIndex.getNum();
  while (ptr < endptr) {
    int idx = *ptr++;

    if (idx >= numCoords) {
      SoDebugError::post("SoVRMLIndexedShape::computeBBox",
                         "index @ %d: %d is out of bounds [%d, %d]",
                         (ptr - coordIndex.getValues(0)) / sizeof(*ptr),
                         idx, numCoords ? 0 : -1, numCoords - 1);
      continue;
    }

    if (idx >= 0) box.extendBy(coords[idx]);
  }
  if (!box.isEmpty()) center = box.getCenter();
}
