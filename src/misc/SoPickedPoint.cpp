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
  \class SoPickedPoint SoPickedPoint.h Inventor/SoPickedPoint.h
  \brief The SoPickedPoint class is used for specifying picked points.
  \ingroup general

  It holds miscellaneous information about the picked point, such
  as position, normal, texture coordinate and material index in the
  current material. It might also hold detail information (an SoDetail
  subclass) for every node in the picked path.

  \sa SoRayPickAction
*/

#include <Inventor/SoPickedPoint.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoTextureMatrixElement.h>
#include <Inventor/details/SoDetail.h>
#include <Inventor/C/tidbits.h> // coin_atexit()
#include <assert.h>


//
// this is not thread-safe, but creating a new matrix action for
// each picked point is not very efficient.
//
static SoGetMatrixAction *matrixAction = NULL;

//
//  Will be called at the end of program to free static memory
//  used by this class.
//
static
void clean_class()
{
  delete matrixAction;
  matrixAction = NULL;
}

/*!
  Copy constructor.
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

  int n = pp.detailList.getLength();
  for (int i = 0; i < n; i++) {
    if (pp.detailList[i])
      this->detailList.append(pp.detailList[i]->copy());
    else
      this->detailList.append(NULL);
  }
}

/*!
  Constructor. Uses the state to convert between world and object
  space for the data.
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

  int pathlen = ((SoFullPath*)this->path)->getLength();
  for (int i = 0; i < pathlen; i++) {
    this->detailList.append(NULL);
  }
}

/*!
  Destructor.
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
  Returns a copy of this picked point.

  \DANGEROUS_ALLOC_RETURN
*/
SoPickedPoint *
SoPickedPoint::copy() const
{
  return new SoPickedPoint(*this);
}

/*!
  Returns the world space point.
*/
const SbVec3f &
SoPickedPoint::getPoint() const
{
  return this->point;
}

/*!
  Returns the world space normal.
*/
const SbVec3f &
SoPickedPoint::getNormal() const
{
  return this->normal;
}

/*!
  Returns the image space texture coordinates.
*/
const SbVec4f &
SoPickedPoint::getTextureCoords() const
{
  return this->texCoords;
}

/*!
  Returns the material index.
*/
int
SoPickedPoint::getMaterialIndex() const
{
  return this->materialIndex;
}

/*!
  Returns the path to the picked object.
*/
SoPath *
SoPickedPoint::getPath() const
{
  return (SoPath*)this->path;
}

/*!
  Returns TRUE if this picked point is on the actual geometry of the
  picked object, or FALSE if not (it might for instance be on the
  bounding box if picking was done on bounding boxes).
*/
SbBool
SoPickedPoint::isOnGeometry() const
{
  return this->onGeometry;
}

/*!
  Returns detail for \a node. If \a node equals NULL, the detail
  for the picked object is returned.
*/
const SoDetail *
SoPickedPoint::getDetail(const SoNode * const node) const
{
  int idx = node ? this->path->findNode(node) :
    ((SoFullPath*)this->path)->getLength() - 1;
  return idx >= 0 ? this->detailList[idx] : NULL;
}

/*!
  Returns the matrix which converts from object (specified by \a node)
  to world space. If \a node equals NULL, the object space of the
  picked object will used.
*/
const SbMatrix &
SoPickedPoint::getObjectToWorld(const SoNode * const node) const
{
  this->applyMatrixAction(node);
  return getMatrixAction()->getMatrix();
}

/*!
  Returns the matrix which converts from world to object (specified
  by \a node) space. If \a node equals NULL, the object space of the
  picked object will used.
*/
const SbMatrix &
SoPickedPoint::getWorldToObject(const SoNode * const node) const
{
  this->applyMatrixAction(node);
  return getMatrixAction()->getInverse();
}

/*!
  Returns the matrix which converts from object (specified by \a node)
  to image space. If \a node equals NULL, the object space of the
  picked object will used.
*/
const SbMatrix &
SoPickedPoint::getObjectToImage(const SoNode * const node) const
{
  this->applyMatrixAction(node);
  return getMatrixAction()->getTextureMatrix();
}

/*!
  Returns the matrix which converts from image to object (specified
  by \a node) space. If \a node equals NULL, the object space of the
  picked object will used.
*/
const SbMatrix &
SoPickedPoint::getImageToObject(const SoNode * const node) const
{
  this->applyMatrixAction(node);
  return getMatrixAction()->getTextureInverse();
}

/*!
  Returns the object space point, in the object space specified by \a
  node. If \a node equals \c NULL, the object space of the node where
  the point was actually picked will be used (this is what one would
  usually be interested in).

  \a node can be any node in the scenegraph.
*/
SbVec3f
SoPickedPoint::getObjectPoint(const SoNode * const node) const
{
  if (node && node != ((SoFullPath*)this->path)->getTail()) {
    SbVec3f ret;
    this->getWorldToObject(node).multVecMatrix(this->point, ret);
    return ret;
  }
  return this->objPoint;
}

/*!
  Returns the object space (specified by \a node) normal. If
  \a node equals NULL, the picked point object space will
  be used.
*/
SbVec3f
SoPickedPoint::getObjectNormal(const SoNode * const node) const
{
  if (node && node != ((SoFullPath*)this->path)->getTail()) {
    SbVec3f ret;
    this->getWorldToObject(node).multDirMatrix(this->normal, ret);
    return ret;
  }
  return this->objNormal;
}

/*!
  Returns the object space (specified by \a node) texture coordinates.
  If \a node equals NULL, the picked point object space will be used.
*/
SbVec4f
SoPickedPoint::getObjectTextureCoords(const SoNode * const node) const
{
  if (node && node != ((SoFullPath*)this->path)->getTail()) {
    SbVec4f ret;
    this->getImageToObject(node).multVecMatrix(this->texCoords, ret);
    return ret;
  }
  return this->objTexCoords;
}

/*!
  Sets the picked point objects space normal vector.
*/
void
SoPickedPoint::setObjectNormal(const SbVec3f &normal)
{
  this->objNormal = normal;
  SoModelMatrixElement::get(this->state).multDirMatrix(normal, this->normal);
}

/*!
  Sets the picked point object space texture coordinates.
*/
void
SoPickedPoint::setObjectTextureCoords(const SbVec4f &texCoords)
{
  this->objTexCoords = texCoords;
  SoTextureMatrixElement::get(this->state).multVecMatrix(texCoords, this->texCoords);
}

/*!
  Sets the material index.
*/
void
SoPickedPoint::setMaterialIndex(const int index)
{
  this->materialIndex = index;
}

/*!
  Sets the detail for \a node. \a node must be in the picked
  path, of course. Set to NULL if you want to remove a detail
  for a node.
*/
void
SoPickedPoint::setDetail(SoDetail *detail, SoNode *node)
{
  int idx = this->path->findNode(node);
  if (idx >= 0) {
    delete this->detailList[idx];
    this->detailList[idx] = detail;
  }
}

//
// applies a matrix action to the path. Stops at node if != NULL
//
void
SoPickedPoint::applyMatrixAction(const SoNode * const node) const
{
  if (node) {
    SoFullPath *fullpath = (SoFullPath*) this->path;
    int idx = fullpath->findNode(node);
    assert(idx >= 0);
    SoTempPath subpath(idx+1);
    subpath.ref(); // Avoid an internal Coin warning for SoAction::apply().
    for (int i = 0; i <= idx; i++) {
      subpath.append(fullpath->getNode(i));
    }
    this->getMatrixAction()->apply(&subpath);
  }
  else {
    this->getMatrixAction()->apply(this->path);
  }
}

//
// creates or returns a matrix action.
//
SoGetMatrixAction *
SoPickedPoint::getMatrixAction() const
{
  if (matrixAction == NULL) {
    matrixAction = new SoGetMatrixAction(this->viewport);
    coin_atexit((coin_atexit_f *)clean_class);
  }
  else {
    matrixAction->setViewportRegion(this->viewport);
  }
  return matrixAction;
}
