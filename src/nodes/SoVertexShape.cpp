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
  \class SoVertexShape SoVertexShape.h Inventor/nodes/SoVertexShape.h
  \brief The SoVertexShape class is the superclass for all vertex based shapes.
  \ingroup nodes

  Basically, every polygon, line or point based shape will inherit
  this class.  It contains methods for organizing the normal cache,
  and also holds the vertexProperty field which can be used to set
  vertex data inside the node.
*/

#include <Inventor/nodes/SoVertexShape.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoNormalElement.h>
#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/elements/SoGLShapeHintsElement.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoCreaseAngleElement.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef COIN_THREADSAFE
#include <Inventor/threads/SbRWMutex.h>
#endif // COIN_THREADSAFE

/*!
  \var SoSFNode SoVertexShape::vertexProperty

  If you set the vertexProperty field, it should be with an SoVertexProperty
  node. Otherwise it will simply be ignored. Nodetypes inheriting
  SoVertexShape will then get their coordinate data from the
  vertexProperty node instead of from the global traversal state.

  The vertexProperty field of SoVertexShape-derived nodes breaks
  somewhat with the basic design of Open Inventor, as its contents are
  not passed to the global state. This is done to provide a simple
  path to highly optimized rendering of vertexbased shapes.

  \sa SoVertexProperty
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

#undef THIS
#define THIS this->pimpl

/*!
  Constructor.
*/
SoVertexShape::SoVertexShape(void)
{
  THIS = new SoVertexShapeP;
  THIS->normalcache = NULL;

  SO_NODE_INTERNAL_CONSTRUCTOR(SoVertexShape);

  SO_NODE_ADD_FIELD(vertexProperty, (NULL));
}

/*!
  Destructor.
*/
SoVertexShape::~SoVertexShape()
{
  if (THIS->normalcache) THIS->normalcache->unref();
  delete THIS;
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
  if (THIS->normalcache) THIS->normalcache->invalidate();
  inherited::notify(nl);
}

/*!
  Subclasses should override this method to generate default normals
  using the SoNormalBundle class. \c TRUE should be returned if
  normals were generated, \c FALSE otherwise.  Default method returns
  \c FALSE.
*/
SbBool
SoVertexShape::generateDefaultNormals(SoState *, SoNormalBundle *)
{
  return FALSE;
}


/*!
  Subclasses should override this method to generate default normals
  using the SoNormalCache class. This is more effective than using
  SoNormalGenerator. Return \c TRUE if normals were generated, \c
  FALSE otherwise. Default method just returns \c FALSE.

  This method is not part of the original SGI Open Inventor API.
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
  if (THIS->normalcache) THIS->normalcache->unref();
  // create new normal cache with no dependencies
  state->push();
  THIS->normalcache = new SoNormalCache(state);
  THIS->normalcache->ref();
  THIS->normalcache->set(num, normals);
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
  return THIS->normalcache;
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
SoVertexShape::generateAndReadLockNormalCache(SoState * const state)
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

  \since 2002-07-11

  \sa readUnlockNormalCache()
*/
void 
SoVertexShape::readLockNormalCache(void)
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
SoVertexShape::readUnlockNormalCache(void)
{
#ifdef COIN_THREADSAFE
  THIS->normalcachemutex.readUnlock();
#endif // COIN_THREADSAFE
}

// write lock normal cache
void 
SoVertexShape::writeLockNormalCache(void)
{
#ifdef COIN_THREADSAFE
  THIS->normalcachemutex.writeLock();
#endif // COIN_THREADSAFE
}

// write unlock normal cache
void 
SoVertexShape::writeUnlockNormalCache(void)
{
#ifdef COIN_THREADSAFE
  THIS->normalcachemutex.writeUnlock();
#endif // COIN_THREADSAFE
}

#undef THIS
