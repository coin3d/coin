/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
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
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLIndexedShape);

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

  if (neednormals) {
    nindices = this->normalIndex.getValues(0);
    if (this->normalIndex.getNum() <= 0 || nindices[0] < 0) nindices = NULL;

    normalcacheused = FALSE;
    if (normals == NULL) {
      if (this->getNormalCache() == NULL ||
          !this->getNormalCache()->isValid(state)) {
          this->generateNormals(state);
      }
      normals = this->getNormalCache()->getNormals();
      nindices = this->getNormalCache()->getIndices();
      normalcacheused = TRUE;
    }
  }
  return TRUE;
}

// Doc in parent
void
SoVRMLIndexedShape::notify(SoNotList * list)
{
  SoField * f = list->getLastField();
  if (f == &this->coordIndex && this->getNormalCache()) {
    this->getNormalCache()->invalidate();
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
    assert(idx < numCoords);
    if (idx >= 0) box.extendBy(coords[idx]);
  }
  if (!box.isEmpty()) center = box.getCenter();
}
