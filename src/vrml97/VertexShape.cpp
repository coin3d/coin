/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoVRMLVertexShape SoVRMLVertexShape.h Inventor/VRMLnodes/SoVRMLVertexShape.h
  \brief The SoVRMLVertexShape class is a superclass for vertex based shapes.
*/

/*!
  \var SoSFNode SoVRMLVertexShape::coord
  Should contain an SoVRMLCoordinate node.
*/

/*!
  \var SoSFNode SoVRMLVertexShape::texCoord
  Can contain an SoVRMLTextureCoordinate node.
*/

/*!
  \var SoSFNode SoVRMLVertexShape::normal
  Can contain an SoVRMLNormal node.
*/

/*!
  \var SoSFNode SoVRMLVertexShape::color
  Can contain an SoVRMLColor node.
*/

/*!
  \var SoSFBool SoVRMLVertexShape::colorPerVertex
  When TRUE, colors are applied per vertex. Default value is TRUE.
*/

/*!
  \var SoSFBool SoVRMLVertexShape::normalPerVertex
  When TRUE, normals are applied per vertex. Default value is TRUE.
*/

#include <Inventor/VRMLnodes/SoVRMLVertexShape.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/VRMLnodes/SoVRMLNormal.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/misc/SoState.h>
#include <stddef.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef COIN_THREADSAFE
#include <Inventor/threads/SbRWMutex.h>
#endif // COIN_THREADSAFE

#ifndef DOXYGEN_SKIP_THIS
class SoVRMLVertexShapeP {
public:
  SoVRMLVertexShapeP(void) 
#ifdef COIN_THREADSAFE
    : normalcachemutex(SbRWMutex::READ_PRECEDENCE)
#endif // COIN_THREADSAFE
  { }

  SoNormalCache * normalcache;
#ifdef COIN_THREADSAFE
  SbRWMutex normalcachemutex;
#endif // COIN_THREADSAFE
};
#endif // DOXYGEN_SKIP_THIS

#undef THIS
#define THIS this->pimpl

SO_NODE_ABSTRACT_SOURCE(SoVRMLVertexShape);

// Doc in parent
void
SoVRMLVertexShape::initClass(void)
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoVRMLVertexShape, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLVertexShape::SoVRMLVertexShape(void)
{
  THIS = new SoVRMLVertexShapeP;
  THIS->normalcache = NULL;

  SO_VRMLNODE_INTERNAL_CONSTRUCTOR(SoVRMLVertexShape);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(coord, (NULL));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(texCoord, (NULL));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(normal, (NULL));
  SO_VRMLNODE_ADD_FIELD(color, (NULL));
  SO_VRMLNODE_ADD_FIELD(colorPerVertex, (TRUE));
  SO_VRMLNODE_ADD_FIELD(normalPerVertex, (TRUE));
}

/*!
  Destructor.
*/
SoVRMLVertexShape::~SoVRMLVertexShape()
{
  if (THIS->normalcache) THIS->normalcache->unref();
  delete THIS;
}

// Doc in parent
SbBool
SoVRMLVertexShape::generateDefaultNormals(SoState * ,
                                          SoNormalBundle *)
{
  return FALSE;
}


// Doc in parent
SbBool
SoVRMLVertexShape::generateDefaultNormals(SoState * /* state */,
                                          SoNormalCache * /* nc */)
{
  return FALSE;
}

void
SoVRMLVertexShape::doAction(SoAction * action)
{
  SoNode * node;

  node = this->coord.getValue();
  if (node) node->doAction(action);

  node = this->texCoord.getValue();
  if (node) node->doAction(action);

  node = this->normal.getValue();
  if (node) node->doAction(action);

  node = this->color.getValue();
  if (node) node->doAction(action);
}

void
SoVRMLVertexShape::GLRender(SoGLRenderAction * action)
{
  SoNode * node;

  node = this->coord.getValue();
  if (node) node->GLRender(action);

  node = this->texCoord.getValue();
  if (node) node->GLRender(action);

  node = this->normal.getValue();
  if (node) node->GLRender(action);

  node = this->color.getValue();
  if (node) node->GLRender(action);
}

void
SoVRMLVertexShape::getBoundingBox(SoGetBoundingBoxAction * action)
{
  inherited::getBoundingBox(action);
}

void
SoVRMLVertexShape::callback(SoCallbackAction * action)
{
  SoVRMLVertexShape::doAction((SoAction*) action);
  inherited::callback(action);
}

void
SoVRMLVertexShape::pick(SoPickAction * action)
{
  SoVRMLVertexShape::doAction((SoAction*) action);
}

void
SoVRMLVertexShape::notify(SoNotList * list)
{
  SoField * f = list->getLastField();
  if (f == &this->coord && THIS->normalcache) {
    THIS->normalcache->invalidate();
  }
  inherited::notify(list);
}

SbBool
SoVRMLVertexShape::shouldGLRender(SoGLRenderAction * action)
{
  return SoShape::shouldGLRender(action);
}

void
SoVRMLVertexShape::setNormalCache(SoState * state,
                                  int num,
                                  SbVec3f * normals)
{
  this->writeLockNormalCache();
  if (THIS->normalcache) THIS->normalcache->unref();
  // create new normal cache with no dependencies
  state->push();
  THIS->normalcache = new SoNormalCache(state);
  THIS->normalcache->ref();
  THIS->normalcache->set(num, normals);
  // force element dependencies
  (void) SoCoordinateElement::getInstance(state);
  state->pop();
  this->writeUnlockNormalCache();
}

/*!  

  Convenience method that can be used by subclasses to return or
  create a normal cache. If the current cache is not valid, it takes
  care of unrefing the old cache and pushing and popping the state to
  create element dependencies when creating the new cache.

  When returning from this method, the normal cache will be
  read locked, and the caller should call readUnlockNormalCache()
  when the normals in the cache is no longer needed.

  This method is specific to Coin and is not part of the OIV API.  

  \since 2002-07-11 

*/
SoNormalCache *
SoVRMLVertexShape::generateAndReadLockNormalCache(SoState * const state)
{
  this->readLockNormalCache();
  if (THIS->normalcache && THIS->normalcache->isValid(state)) {
    return THIS->normalcache;
  }
  this->readUnlockNormalCache();
  this->writeLockNormalCache();
  
  SbBool storeinvalid = SoCacheElement::setInvalid(FALSE);
  
  if (THIS->normalcache) THIS->normalcache->unref();
  state->push(); // need to push for cache dependencies
  THIS->normalcache = new SoNormalCache(state);
  THIS->normalcache->ref();
  SoCacheElement::set(state, THIS->normalcache);
  //
  // See if the node supports the Coin-way of generating normals
  //
  if (!generateDefaultNormals(state, THIS->normalcache)) {
    // FIXME: implement SoNormalBundle
    if (generateDefaultNormals(state, (SoNormalBundle *)NULL)) {
      // FIXME: set generator in normal cache
    }
  }
  state->pop(); // don't forget this pop
  
  SoCacheElement::setInvalid(storeinvalid);
  this->writeUnlockNormalCache();
  this->readLockNormalCache();
  return THIS->normalcache;
}

SoNormalCache *
SoVRMLVertexShape::getNormalCache(void) const
{
  return THIS->normalcache;
}

/*!
  Convenience method that returns the current coordinate and normal
  element. This method is not part of the OIV API.
*/
void
SoVRMLVertexShape::getVertexData(SoState * state,
                                 const SoCoordinateElement *& coords,
                                 const SbVec3f *& normals,
                                 const SbBool neednormals)
{
  coords = SoCoordinateElement::getInstance(state);
  assert(coords);

  normals = NULL;
  if (neednormals) {
    SoVRMLNormal * node = (SoVRMLNormal*) this->normal.getValue();
    normals = (node && node->vector.getNum()) ? node->vector.getValues(0) : NULL;
  }
}

/*!

  Read lock the normal cache. This method should be called before
  fetching the normal cache (using getNormalCache()). When the cached
  normals are no longer needed, readUnlockNormalCache() must be called.
  
  It is also possible to use generateAndReadLockNormalCache().

  \since 2002-07-11

  \sa readUnlockNormalCache()
*/
void 
SoVRMLVertexShape::readLockNormalCache(void)
{
#ifdef COIN_THREADSAFE
  THIS->normalcachemutex.readLock();
#endif // COIN_THREADSAFE
}

/*!

  Read unlock the normal cache. Should be called when the read-locked
  cached normals are no longer needed.

  \since 2002-07-11
  
  \sa readLockNormalCache()
*/
void 
SoVRMLVertexShape::readUnlockNormalCache(void)
{
#ifdef COIN_THREADSAFE
  THIS->normalcachemutex.readUnlock();
#endif // COIN_THREADSAFE
}

// write lock normal cache
void 
SoVRMLVertexShape::writeLockNormalCache(void)
{
#ifdef COIN_THREADSAFE
  THIS->normalcachemutex.writeLock();
#endif // COIN_THREADSAFE
}

// write unlock normal cache
void 
SoVRMLVertexShape::writeUnlockNormalCache(void)
{
#ifdef COIN_THREADSAFE
  THIS->normalcachemutex.writeUnlock();
#endif // COIN_THREADSAFE
}

#undef THIS

