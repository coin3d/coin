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
  \class SoIndexedShape SoIndexedShape.h Inventor/nodes/SoIndexedShape.h
  \brief The SoIndexedShape class is the superclass for all indexed vertex shapes.
  \ingroup nodes

  This is an abstract class which contains storage for four fields for
  indices to coordinates, normals, materials and texture coordinates
  for it's subclasses.
*/

#include <Inventor/nodes/SoIndexedShape.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <coindefs.h> // COIN_OBSOLETED()
#include <Inventor/actions/SoAction.h>
#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/elements/SoShapeHintsElement.h>
#include <Inventor/elements/SoNormalBindingElement.h>
#include <Inventor/elements/SoTextureCoordinateBindingElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/nodes/SoVertexProperty.h>

/*!
  \var SoMFInt32 SoIndexedShape::coordIndex
  Coordinate indices.
*/
/*!
  \var SoMFInt32 SoIndexedShape::materialIndex
  Material indices.
*/
/*!
  \var SoMFInt32 SoIndexedShape::normalIndex
  Normal indices.
*/
/*!
  \var SoMFInt32 SoIndexedShape::textureCoordIndex
  Texture coordinate indices.
*/


SO_NODE_ABSTRACT_SOURCE(SoIndexedShape);

/*!
  Constructor.
*/
SoIndexedShape::SoIndexedShape()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoIndexedShape);

  SO_NODE_ADD_FIELD(coordIndex,(0));
  SO_NODE_ADD_FIELD(materialIndex,(-1));
  SO_NODE_ADD_FIELD(normalIndex,(-1));
  SO_NODE_ADD_FIELD(textureCoordIndex,(-1));
}

/*!
  Destructor.
*/
SoIndexedShape::~SoIndexedShape()
{
}

// Documented in superclass.
void
SoIndexedShape::initClass(void)
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoIndexedShape, SO_FROM_INVENTOR_1);
}

// Collects common error msg code for computeBBox() for both
// Coordinate3 and Coordinate4 loops.
static void
error_idx_out_of_bounds(const SoIndexedShape * node, int idxidx, int upper)
{
  SbName n = node->getName();
  SbString ns(" ");
  ns += n;

  SbName t = node->getTypeId().getName();

  SbString bounds;
  if (upper > 0) {
    // (Note: the if-expression above should have been "upper >= 0",
    // if it weren't for the default SoCoordinateElement containing
    // one default coordinate.)
    bounds.sprintf("should be within [0, %d]", upper);
  }
  else {
    bounds = "but no coordinates are available from the state!";
  }

  SoDebugError::post("SoIndexedShape::computeBBox",
                     "coordinate index nr %d for %snode%s of type %s is "
                     "out of bounds: is %d, %s",
                     idxidx,
                     (n == "") ? "<unnamed> " : "",
                     (n != "") ? ns.getString() : "",
                     t.getString(),
                     node->coordIndex[idxidx],
                     bounds.getString());
}

// Documented in superclass. Overridden to calculate bounding box of
// all indexed coordinates, using the coordIndex field.
void
SoIndexedShape::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  assert(box.isEmpty());
  SoState * state = action->getState();

  const SoCoordinateElement * coordelem = NULL;
  SoVertexProperty * vp = (SoVertexProperty *) this->vertexProperty.getValue();
  assert(!vp ||
         vp->getTypeId().isDerivedFrom(SoVertexProperty::getClassTypeId()));
  SbBool vpvtx = vp && (vp->vertex.getNum() > 0);
  if (!vpvtx) {
    coordelem = SoCoordinateElement::getInstance(state);
  }

  const int numcoords = vpvtx ? vp->vertex.getNum() : coordelem->getNum();

  if (vpvtx || coordelem->is3D()) {
    const SbVec3f * coords = vpvtx ?
      vp->vertex.getValues(0) :
      coordelem->getArrayPtr3();

    const int32_t * ptr = this->coordIndex.getValues(0);
    const int32_t * endptr = ptr + this->coordIndex.getNum();
    while (ptr < endptr) {
      const int idx = *ptr++;
      if (idx < numcoords) {
        if (idx >= 0) box.extendBy(coords[idx]);
      }
#if COIN_DEBUG
      else {
        const int faultyidxpos = (ptr - this->coordIndex.getValues(0)) - 1;
        error_idx_out_of_bounds(this, faultyidxpos, numcoords - 1);
        if (numcoords <= 1) break; // give only one error msg on missing coords
        // (the default state is that there's a default
        // SoCoordinateElement element with a single default
        // coordinate point setup)
      }
#endif // COIN_DEBUG
    }
  }
  else {
    const SbVec4f * coords = coordelem->getArrayPtr4();
    const int32_t * ptr = this->coordIndex.getValues(0);
    const int32_t * endptr = ptr + this->coordIndex.getNum();
    while (ptr < endptr) {
      int idx = *ptr++;
      if (idx < numcoords) {
        if (idx >= 0) {
          SbVec4f tmp = coords[idx];
          box.extendBy(SbVec3f(tmp[0], tmp[1], tmp[2]));
        }
      }
#if COIN_DEBUG
      else {
        const int faultyidxpos = (ptr - this->coordIndex.getValues(0)) - 1;
        error_idx_out_of_bounds(this, faultyidxpos, numcoords - 1);
        if (numcoords <= 1) break; // give only one error msg on missing coords
        // (the default state is that there's a default
        // SoCoordinateElement element with a single default
        // coordinate point setup)
      }
#endif // COIN_DEBUG
    }
  }

  if (!box.isEmpty()) center = box.getCenter();
}

/*!
  Returns whether texture coordinates should be indexed or not.

  \sa SoTextureCoordinateBinding
*/
SbBool
SoIndexedShape::areTexCoordsIndexed(SoAction * action)
{
  return SoTextureCoordinateBindingElement::get(action->getState()) ==
    SoTextureCoordinateBindingElement::PER_VERTEX_INDEXED;
}

/*!  
  Starting at index position \a startCoord, returns the number of
  indices until either the end of index array or a separator index (-1).  
*/
int
SoIndexedShape::getNumVerts(const int startCoord)
{
  const int32_t * ptr = this->coordIndex.getValues(0);
  const int32_t * endptr = ptr + this->coordIndex.getNum();
  ptr += startCoord;
  int cnt = 0;
  while (ptr < endptr && *ptr >= 0) cnt++, ptr++;
  return cnt;
}

/*!
  Not implemented. Probably only used for internal purposes in SGI's
  original Open Inventor, which means it should have been private.

  Let us know if you need this method for any code you are porting and
  we'll look into implement it properly.
*/
void
SoIndexedShape::setupIndices(const int /* numParts */,
                             const int /* numFaces */,
                             const SbBool /* needNormals */,
                             const SbBool /* needTexCoords */)
{
  COIN_OBSOLETED();
}

/*!
  Not implemented. Probably only used for internal purposes in SGI's
  original Open Inventor, which means it should have been private.

  Let us know if you need this method for any code you are porting and
  we'll look into implement it properly.
*/
const int32_t *
SoIndexedShape::getNormalIndices()
{
  COIN_OBSOLETED();
  return NULL;
}

/*!
  Not implemented. Probably only used for internal purposes in SGI's
  original Open Inventor, which means it should have been private.

  Let us know if you need this method for any code you are porting and
  we'll look into implement it properly.
*/
const int32_t *
SoIndexedShape::getColorIndices()
{
  COIN_OBSOLETED();
  return NULL;
}

/*!
  Not implemented. Probably only used for internal purposes in SGI's
  original Open Inventor, which means it should have been private.

  Let us know if you need this method for any code you are porting and
  we'll look into implement it properly.
*/
const int32_t *
SoIndexedShape::getTexCoordIndices()
{
  COIN_OBSOLETED();
  return NULL;
}

/*!
  Convenience method that will fetch data needed for rendering or
  generating primitives. Takes care of normal cache.

  This method was not part of the original SGI Open Inventor API, and
  is an extension specific for Coin.
*/
SbBool
SoIndexedShape::getVertexData(SoState * state,
                              const SoCoordinateElement *& coords,
                              const SbVec3f *& normals,
                              const int32_t *& cindices,
                              const int32_t *& nindices,
                              const int32_t *& tindices,
                              const int32_t *& mindices,
                              int & numcindices,
                              const SbBool needNormals,
                              SbBool & normalCacheUsed)
{
  SoVertexShape::getVertexData(state, coords, normals, needNormals);
  
  cindices = this->coordIndex.getValues(0);
  numcindices = this->coordIndex.getNum();

  mindices = this->materialIndex.getValues(0);
  if (this->materialIndex.getNum() <= 0 || mindices[0] < 0) mindices = NULL;

  tindices = this->textureCoordIndex.getValues(0);
  if (this->textureCoordIndex.getNum() <= 0 || tindices[0] < 0) tindices = NULL;

  normalCacheUsed = FALSE;
  if (needNormals) {
    nindices = this->normalIndex.getValues(0);
    if (this->normalIndex.getNum() <= 0 || nindices[0] < 0) nindices = NULL;

    normalCacheUsed = FALSE;
    if (normals == NULL) {
      SoNormalCache * nc = this->generateAndReadLockNormalCache(state);
      normals = nc->getNormals();
      nindices = nc->getIndices();
      normalCacheUsed = TRUE;
    }
  }
  return TRUE;
}
