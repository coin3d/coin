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
  \class SoPickedPoint SoPickedPoint.h Inventor/SoPickedPoint.h
  \brief The SoPickedPoint class ... blablabla ...

  FIXME: write doc
 */


#include <Inventor/SoPickedPoint.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoTextureMatrixElement.h>
#include <Inventor/details/SoDetail.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

//
// this is not thread-safe, but creating a new matrix action for
// each picked point is not very efficient.
//
static SoGetMatrixAction *matrixAction = NULL;

#if 0 // FIXME: re-code to be run automatically upon exit. 19991106 mortene.
/*!
  Will be called at the end of your program to free static memory
  used by this class.
*/
void 
SoPickedPoint::cleanClass()
{
  delete matrixAction;
  matrixAction = NULL;
}
#endif // re-code

/*!
  FIXME: write doc
 */
SoPickedPoint::SoPickedPoint(const SoPickedPoint &pp)
{
  this->path = pp.path;
  this->path->ref();
  this->state = pp.state;
  this->point = pp.point;
  this->objPoint = pp.objPoint;
  this->normal = pp.normal;
  this->objNormal = pp.objNormal;
  this->texCoords = pp.texCoords;
  this->objTexCoords = pp.objTexCoords;
  this->materialIndex = pp.materialIndex;
  this->onGeometry = pp.onGeometry;
  this->viewport = pp.viewport;
  this->detailList = pp.detailList;
}

/*!
  FIXME: write doc
*/
SoPickedPoint::SoPickedPoint(const SoPath * const path, SoState * const state,
			     const SbVec3f &objSpacePoint)
{
  this->path = path->copy();
  this->path->ref();
  this->state = state;
  this->objPoint = objSpacePoint;
  SoModelMatrixElement::get(state).multVecMatrix(objSpacePoint, this->point);
  this->objNormal = this->normal = SbVec3f(0,0,1);
  this->objTexCoords = this->texCoords = SbVec4f(0,0,0,1);
  this->materialIndex = 0;
  this->onGeometry = TRUE;
  this->viewport = SoViewportRegionElement::get(state);
  
  int pathlen = this->path->getLength();
  for (int i = 0; i < pathlen; i++) {
    this->detailList.append(NULL);
  }
}

/*!
  FIXME: write doc
 */
SoPickedPoint::~SoPickedPoint()
{
  assert(this->path);
  this->path->unref();
  
  int n = this->detailList.getLength();
  for (int i = 0; i < n; i++) {
    delete this->detailList[i];
  }
}

/*!
  FIXME: write doc
 */
SoPickedPoint *
SoPickedPoint::copy() const
{
  return new SoPickedPoint(*this);
}

/*!
  FIXME: write doc
 */
const SbVec3f &
SoPickedPoint::getPoint() const
{
  return this->point;
}

/*!
  FIXME: write doc
 */
const SbVec3f &
SoPickedPoint::getNormal() const
{
  return this->normal;
}

/*!
  FIXME: write doc
 */
const SbVec4f &
SoPickedPoint::getTextureCoords() const
{
  return this->texCoords;
}

/*!
  FIXME: write doc
 */
int
SoPickedPoint::getMaterialIndex() const
{
  return this->materialIndex;
}

/*!
  FIXME: write doc
 */
SoPath *
SoPickedPoint::getPath() const
{
  // FIXME: return copy?
  return (SoPath*)this->path;
}

/*!
  FIXME: write doc
 */
SbBool 
SoPickedPoint::isOnGeometry() const
{
  return this->onGeometry;
}

/*!
  FIXME: write doc
 */
const SoDetail *
SoPickedPoint::getDetail(const SoNode * const node) const
{
  int idx = node ? this->path->findNode(node) : this->path->getLength() - 1;
  return idx >= 0 ? this->detailList[idx] : NULL;
}

const SbMatrix &
SoPickedPoint::getObjectToWorld(const SoNode * const node) const
{
  this->applyMatrixAction(node);
  return getMatrixAction()->getMatrix();
}

const SbMatrix &
SoPickedPoint::getWorldToObject(const SoNode * const node) const
{
  this->applyMatrixAction(node);
  return getMatrixAction()->getInverse();
}

/*!
  FIXME: write doc
 */
const SbMatrix &
SoPickedPoint::getObjectToImage(const SoNode * const node) const
{
  this->applyMatrixAction(node);
  return getMatrixAction()->getTextureMatrix();
}

/*!
  FIXME: write doc
 */
const SbMatrix &
SoPickedPoint::getImageToObject(const SoNode * const node) const
{
  this->applyMatrixAction(node);
  return getMatrixAction()->getTextureInverse();
}

/*!
  FIXME: write doc
 */
SbVec3f
SoPickedPoint::getObjectPoint(const SoNode * const /*node*/) const
{
#if 0 // OBSOLETED 19991117, pederb (incorrect for SoArray, SoMultipleCopy scenes)
  SbVec3f ret;
  this->getWorldToObject(node).multVecMatrix(this->point, ret);
  return ret;
#else // new code
  return this->objPoint;
#endif
}

/*!
  FIXME: write doc
 */
SbVec3f
SoPickedPoint::getObjectNormal(const SoNode * const /*node*/) const
{
#if 0 // OBSOLETED 19991117, pederb (incorrect for SoArray, SoMultipleCopy scenes)
  SbVec3f ret;
  this->getWorldToObject(node).multDirMatrix(this->normal, ret);
  return ret;
#else // new code
  return this->objNormal;
#endif // new code
}

/*!
  FIXME: write doc
 */
SbVec4f
SoPickedPoint::getObjectTextureCoords(const SoNode * const /*node*/) const
{
#if 0 // OBSOLETED 19991117, pederb (incorrect for SoArray, SoMultipleCopy scenes)
  SbVec4f ret;
  this->getImageToObject(node).multVecMatrix(this->texCoords, ret);
  return ret;
#else // new code
  return this->objTexCoords;
#endif // new code
}

/*!
  FIXME: write doc
 */
void
SoPickedPoint::setObjectNormal(const SbVec3f &normal)
{
  this->objNormal = normal;
  SoModelMatrixElement::get(this->state).multDirMatrix(normal, this->normal);
}

/*!
  FIXME: write doc
 */
void
SoPickedPoint::setObjectTextureCoords(const SbVec4f &texCoords)
{
  this->objTexCoords = texCoords;
  SoTextureMatrixElement::get(this->state).multVecMatrix(texCoords, this->texCoords);
}

/*!
  FIXME: write doc
 */
void
SoPickedPoint::setMaterialIndex(const int index)
{
  this->materialIndex = index;
}

/*!
  FIXME: write doc
 */
void
SoPickedPoint::setDetail(SoDetail *detail, SoNode *node)
{
  int idx = this->path->findNode(node);
  if (idx >= 0) {
    delete this->detailList[idx];
    this->detailList.set(idx, detail);
  }
}

void 
SoPickedPoint::applyMatrixAction(const SoNode * const node) const
{
  if (node) {
    // FIXME: it should be possible to optimize this by 
    // avoiding to create 
    int idx = this->path->findNode(node);
    assert(idx >= 0);
    SoPath *subpath = this->path->copy(idx+1);
    assert(subpath);
    subpath->ref();
    getMatrixAction()->apply(subpath);
    subpath->unref();
  }
  else {
    getMatrixAction()->apply(this->path);
  }
}

/*!
  Used to return a matrix action to be used to find scene matrices.
*/
SoGetMatrixAction *
SoPickedPoint::getMatrixAction() const
{
  if (matrixAction == NULL) {
    matrixAction = new SoGetMatrixAction(this->viewport);
  }
  else {
    matrixAction->setViewportRegion(this->viewport);
  }
  return matrixAction;
}

