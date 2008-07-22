/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoVertexShape SoVertexShape.h Inventor/nodes/SoVertexShape.h
  \brief The SoVertexShape class is the superclass for all vertex based shapes.
  \ingroup nodes

  Basically, every polygon-, line- or point-based shape will inherit
  this class.  It contains methods for organizing the normal cache,
  and also holds the SoVertexShape::vertexProperty field which can be
  used to set vertex data inside the node.
*/

#include <Inventor/nodes/SoVertexShape.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoNormalElement.h>
#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/elements/SoGLShapeHintsElement.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoCreaseAngleElement.h>

#ifdef COIN_THREADSAFE
#include <Inventor/threads/SbRWMutex.h>
#endif // COIN_THREADSAFE

#include "nodes/SoSubNodeP.h"

/*!
  \var SoSFNode SoVertexShape::vertexProperty

  If you set the vertexProperty field, it should be with an
  SoVertexProperty node. Otherwise it will simply be
  ignored. Nodetypes inheriting SoVertexShape will then get their
  coordinate data from the vertexProperty node instead of from the
  global traversal state.

  The vertexProperty field of SoVertexShape-derived nodes breaks
  somewhat with the basic design of Open Inventor, as its contents are
  not passed to the global state. This is done to provide a simple
  path to highly optimized rendering of vertexbased shapes.

  \sa SoVertexProperty

  \since Coin 1.0
  \since SGI Inventor v2.1
*/


#ifndef DOXYGEN_SKIP_THIS
class SoVertexShapeP {
public:
  SoVertexShapeP(void) 
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

SO_NODE_ABSTRACT_SOURCE(SoVertexShape);

#define PRIVATE(obj) ((obj)->pimpl)

/*!
  Constructor.
*/
SoVertexShape::SoVertexShape(void)
{
  PRIVATE(this) = new SoVertexShapeP;
  PRIVATE(this)->normalcache = NULL;

  SO_NODE_INTERNAL_CONSTRUCTOR(SoVertexShape);

  SO_NODE_ADD_FIELD(vertexProperty, (NULL));
}

/*!
  Destructor.
*/
SoVertexShape::~SoVertexShape()
{
  if (PRIVATE(this)->normalcache) PRIVATE(this)->normalcache->unref();
  delete PRIVATE(this);
}

// doc from superclass
void
SoVertexShape::initClass(void)
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoVertexShape, SO_FROM_INVENTOR_1);
}

// Documented in superclass.
void
SoVertexShape::notify(SoNotList * nl)
{
  this->readLockNormalCache();
  if (PRIVATE(this)->normalcache) PRIVATE(this)->normalcache->invalidate();
  this->readUnlockNormalCache();
  inherited::notify(nl);
}

// This documentation block has a copy in vrml97/VertexShape.cpp.
/*!
  \COININTERNAL

  Subclasses should override this method to generate default normals
  using the SoNormalBundle class. \c TRUE should be returned if
  normals were generated, \c FALSE otherwise.

  Default method returns \c FALSE.

  \COIN_FUNCTION_EXTENSION
*/
SbBool
SoVertexShape::generateDefaultNormals(SoState *, SoNormalBundle *)
{
  return FALSE;
}

// This documentation block has a copy in vrml97/VertexShape.cpp.
/*!
  \COININTERNAL

  Subclasses should override this method to generate default normals
  using the SoNormalCache class. This is more effective than using
  SoNormalGenerator. Return \c TRUE if normals were generated, \c
  FALSE otherwise.

  Default method just returns \c FALSE.

  \COIN_FUNCTION_EXTENSION
*/
SbBool
SoVertexShape::generateDefaultNormals(SoState * /* state */,
                                      SoNormalCache * /* nc */)
{
  return FALSE;
}

// doc from superclass
SbBool
SoVertexShape::shouldGLRender(SoGLRenderAction * action)
{
  return SoShape::shouldGLRender(action);
}

/*!
  Sets normal cache to contain the normals specified by \a normals and \a num,
  and forces cache dependencies on coordinates, shape hints and crease angle.
*/
void
SoVertexShape::setNormalCache(SoState * const state,
                              const int num,
                              const SbVec3f * normals)
{
  this->writeLockNormalCache();
  if (PRIVATE(this)->normalcache) PRIVATE(this)->normalcache->unref();
  // create new normal cache with no dependencies
  state->push();
  PRIVATE(this)->normalcache = new SoNormalCache(state);
  PRIVATE(this)->normalcache->ref();
  PRIVATE(this)->normalcache->set(num, normals);
  // force element dependencies
  (void) SoCoordinateElement::getInstance(state);
  (void) SoShapeHintsElement::getVertexOrdering(state);
  (void) SoCreaseAngleElement::get(state);
  state->pop();
  this->writeUnlockNormalCache();
}

/*!
  Returns the current normal cache, or NULL if there is none.
*/
SoNormalCache *
SoVertexShape::getNormalCache(void) const
{
  return PRIVATE(this)->normalcache;
}

/*!  

  Convenience method that can be used by subclasses to return or
  create a normal cache. If the current cache is not valid, it takes
  care of unrefing the old cache and pushing and popping the state to
  create element dependencies when creating the new cache.

  When returning from this method, the normal cache will be
  read locked, and the caller should call readUnlockNormalCache()
  when the normals in the cache is no longer needed.

  \COIN_FUNCTION_EXTENSION

  \since Coin 2.0
*/
SoNormalCache *
SoVertexShape::generateAndReadLockNormalCache(SoState * const state)
{
  this->readLockNormalCache();
  if (PRIVATE(this)->normalcache && PRIVATE(this)->normalcache->isValid(state)) {
    return PRIVATE(this)->normalcache;
  }
  this->readUnlockNormalCache();
  this->writeLockNormalCache();
  
  SbBool storeinvalid = SoCacheElement::setInvalid(FALSE);
  
  if (PRIVATE(this)->normalcache) PRIVATE(this)->normalcache->unref();
  state->push(); // need to push for cache dependencies
  PRIVATE(this)->normalcache = new SoNormalCache(state);
  PRIVATE(this)->normalcache->ref();
  SoCacheElement::set(state, PRIVATE(this)->normalcache);
  //
  // See if the node supports the Coin-way of generating normals
  //
  if (!generateDefaultNormals(state, PRIVATE(this)->normalcache)) {
    // FIXME: implement SoNormalBundle
    if (generateDefaultNormals(state, (SoNormalBundle *)NULL)) {
      // FIXME: set generator in normal cache
    }
  }
  state->pop(); // don't forget this pop
  
  SoCacheElement::setInvalid(storeinvalid);
  this->writeUnlockNormalCache();
  this->readLockNormalCache();
  return PRIVATE(this)->normalcache;
}

/*!
  Convenience method that returns the current coordinate and normal
  element. This method is not part of the OIV API.
*/
void
SoVertexShape::getVertexData(SoState * state,
                             const SoCoordinateElement *& coords,
                             const SbVec3f *& normals,
                             const SbBool neednormals)
{
  coords = SoCoordinateElement::getInstance(state);
  assert(coords);

  normals = NULL;
  if (neednormals) {
    normals = SoNormalElement::getInstance(state)->getArrayPtr();
  }
}

// doc from superclass
void
SoVertexShape::write(SoWriteAction * action)
{
  inherited::write(action);
}

/*!

  Read lock the normal cache. This method should be called before
  fetching the normal cache (using getNormalCache()). When the cached
  normals are no longer needed, readUnlockNormalCache() must be called.
  
  It is also possible to use generateAndReadLockNormalCache().

  \COIN_FUNCTION_EXTENSION

  \sa readUnlockNormalCache()
  \since Coin 2.0
*/
void 
SoVertexShape::readLockNormalCache(void)
{
#ifdef COIN_THREADSAFE
  PRIVATE(this)->normalcachemutex.readLock();
#endif // COIN_THREADSAFE
}

/*!

  Read unlock the normal cache. Should be called when the read-locked
  cached normals are no longer needed.

  \sa readLockNormalCache()
  \since Coin 2.0
*/
void 
SoVertexShape::readUnlockNormalCache(void)
{
#ifdef COIN_THREADSAFE
  PRIVATE(this)->normalcachemutex.readUnlock();
#endif // COIN_THREADSAFE
}

// write lock normal cache
void 
SoVertexShape::writeLockNormalCache(void)
{
#ifdef COIN_THREADSAFE
  PRIVATE(this)->normalcachemutex.writeLock();
#endif // COIN_THREADSAFE
}

// write unlock normal cache
void 
SoVertexShape::writeUnlockNormalCache(void)
{
#ifdef COIN_THREADSAFE
  PRIVATE(this)->normalcachemutex.writeUnlock();
#endif // COIN_THREADSAFE
}

#undef PRIVATE
