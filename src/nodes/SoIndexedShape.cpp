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
  \class SoIndexedShape SoIndexedShape.h Inventor/nodes/SoIndexedShape.h
  \brief The SoIndexedShape class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoIndexedShape.h>



#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/nodes/SoVertexProperty.h>

#if !defined(COIN_EXCLUDE_SOACTION)
#include <Inventor/actions/SoAction.h>
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCOORDINATEELEMENT)
#include <Inventor/elements/SoCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOCOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SOSHAPEHINTSELEMENT)
#include <Inventor/elements/SoShapeHintsElement.h>
#endif // !COIN_EXCLUDE_SOSHAPEHINTSELEMENT
#if !defined(COIN_EXCLUDE_SOCREASEANGLEELEMENT)
#include <Inventor/elements/SoCreaseAngleElement.h>
#endif // !COIN_EXCLUDE_SOCREASEANGLEELEMENT
#if !defined(COIN_EXCLUDE_SONORMALBINDINGELEMENT)
#include <Inventor/elements/SoNormalBindingElement.h>
#endif // !COIN_EXCLUDE_SONORMALBINDINGELEMENT
#if !defined(COIN_EXCLUDE_SOCOORDINATEELEMENT)
#include <Inventor/elements/SoCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOCOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SONORMALELEMENT)
#include <Inventor/elements/SoNormalElement.h>
#endif // !COIN_EXCLUDE_SONORMALELEMENT
#if !defined(COIN_EXCLUDE_SOLIGHTMODELELEMENT)
#include <Inventor/elements/SoLightModelElement.h>
#endif // !COIN_EXCLUDE_SOLIGHTMODELELEMENT
#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/nodes/SoVertexProperty.h>

/*!
  \var SoMFInt32 SoIndexedShape::coordIndex
  FIXME: write documentation for field
*/
/*!
  \var SoMFInt32 SoIndexedShape::materialIndex
  FIXME: write documentation for field
*/
/*!
  \var SoMFInt32 SoIndexedShape::normalIndex
  FIXME: write documentation for field
*/
/*!
  \var SoMFInt32 SoIndexedShape::textureCoordIndex
  FIXME: write documentation for field
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

/*!
  Does initialization common for all objects of the
  SoIndexedShape class. This includes setting up the
  type system, among other things.
*/
void
SoIndexedShape::initClass()
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoIndexedShape);
}

/*!
  FIXME: write function documentation
*/
void
SoIndexedShape::notify(SoNotList * /* list */)
{
  // FIXME: implement what's necessary here (cache
  // destruction?). 19990405 mortene.
}

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void
SoIndexedShape::computeBBox(SoAction * action, SbBox3f & box,
                            SbVec3f & center)
{
  SoState * state = action->getState();

  const SoCoordinateElement *coordelem = SoCoordinateElement::getInstance(state);
  SoVertexProperty * vp = (SoVertexProperty *) this->vertexProperty.getValue();
  assert(!vp ||
         vp->getTypeId().isDerivedFrom(SoVertexProperty::getClassTypeId()));
  SbBool vpvtx = vp && (vp->vertex.getNum() > 0);

  int numCoords = vpvtx ?
    vp->vertex.getNum() :
    coordelem->getNum();
  
  if (vpvtx || coordelem->is3D()) {
    const SbVec3f * coords = vpvtx ?
      vp->vertex.getValues(0) :
      coordelem->getArrayPtr3();
    assert(coords);
    box.makeEmpty();
    const int32_t * ptr = coordIndex.getValues(0);
    const int32_t * endptr = ptr + coordIndex.getNum();
    while (ptr < endptr) {
      int idx = *ptr++;
      assert(idx < numCoords);
      if (idx >= 0) box.extendBy(coords[idx]);
    }
  }
  else {
    const SbVec4f * coords =
      coordelem->getArrayPtr4();
    assert(coords);
    const int32_t * ptr = coordIndex.getValues(0);
    const int32_t * endptr = ptr + coordIndex.getNum();
    while (ptr < endptr) {
      int idx = *ptr++;
      assert(idx < numCoords);
      if (idx >= 0) {
        SbVec4f tmp = coords[idx];
        box.extendBy(SbVec3f(tmp[0], tmp[1], tmp[2]));
      }
    }
  }
  center = box.getCenter();
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write function documentation
*/
SbBool
SoIndexedShape::areTexCoordsIndexed(SoAction * /* action */)
{
  assert(0 && "FIXME: not implemented yet");
  return TRUE;
}
#endif // !COIN_EXCLUDE_SOACTION

/*!
  FIXME: write function documentation
*/
int
SoIndexedShape::getNumVerts(const int startCoord)
{
  const int32_t * ptr = coordIndex.getValues(0);
  const int32_t * endptr = ptr + coordIndex.getNum();
  ptr += startCoord;
  int cnt = 0;
  while (ptr < endptr && *ptr >= 0) cnt++, ptr++;
  return cnt;
}

/*!
  FIXME: write function documentation
*/
void
SoIndexedShape::setupIndices(const int /* numParts */,
                             const int /* numFaces */,
                             const SbBool /* needNormals */,
                             const SbBool /* needTexCoords */)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
const int32_t *
SoIndexedShape::getNormalIndices()
{
  assert(0 && "FIXME: not implemented yet");
  return NULL;
}

/*!
  FIXME: write function documentation
*/
const int32_t *
SoIndexedShape::getColorIndices()
{
  assert(0 && "FIXME: not implemented yet");
  return NULL;
}

/*!
  FIXME: write function documentation
*/
const int32_t *
SoIndexedShape::getTexCoordIndices()
{
  assert(0 && "FIXME: not implemented yet");
  return NULL;
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
SbBool
SoIndexedShape::generateDefaultNormals(SoState * state,
                                       SoNormalCache * nc)
{
  SbBool ccw = TRUE;
  if (SoShapeHintsElement::getVertexOrdering(state) ==
      SoShapeHintsElement::CLOCKWISE) ccw = FALSE;
  
  const SbVec3f * coords = SoCoordinateElement::getInstance(state)->getArrayPtr3();
  assert(coords);

#if !defined(COIN_EXCLUDE_SONORMALBINDINGELEMENT)
  SoNormalBindingElement::Binding normbind =
    SoNormalBindingElement::get(state);


  switch (normbind) {
  case SoNormalBindingElement::PER_VERTEX:
  case SoNormalBindingElement::PER_VERTEX_INDEXED:
    nc->generatePerVertex(coords,
                          coordIndex.getValues(0),
                          coordIndex.getNum(),
                          SoCreaseAngleElement::get(state),
                          NULL,
                          ccw);
    break;
  case SoNormalBindingElement::PER_FACE:
  case SoNormalBindingElement::PER_FACE_INDEXED:
  case SoNormalBindingElement::PER_PART: // FIXME: is this correct?
  case SoNormalBindingElement::PER_PART_INDEXED:
    nc->generatePerFace(coords,
                        coordIndex.getValues(0),
                        coordIndex.getNum(),
                        ccw);
    break;
  default:
    break;
  }
#else // COIN_EXCLUDE_SONORMALBINDINGELEMENT
  nc->generatePerVertex(coords,
                        coordIndex.getValues(0),
                        coordIndex.getNum(),
                        SoCreaseAngleElement::get(state),
                        ccw);
#endif // COIN_EXCLUDE_SONORMALBINDINGELEMENT

  return TRUE;
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

/*!
  FIXME: write function documentation
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

  if (needNormals) {
    nindices = this->normalIndex.getValues(0);
    if (this->normalIndex.getNum() <= 0 || nindices[0] < 0) nindices = NULL;

    normalCacheUsed = FALSE;
    if (normals == NULL) {
      if (this->getNormalCache() == NULL ||
          !this->getNormalCache()->isValid(state)) {
        generateNormals(state);
      }
      normals = this->getNormalCache()->getNormals();
      nindices = this->getNormalCache()->getIndices();
      normalCacheUsed = TRUE;
    }
  }
  return TRUE;
}
