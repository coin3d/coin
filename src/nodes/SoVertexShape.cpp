/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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
  \class SoVertexShape SoVertexShape.h Inventor/nodes/SoVertexShape.h
  \brief The SoVertexShape class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoVertexShape.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoNormalElement.h>
#include <Inventor/elements/SoGLNormalizeElement.h>
#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/elements/SoGLShapeHintsElement.h>
#include <Inventor/elements/SoCacheElement.h>

#include <coindefs.h> // COIN_STUB()

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \var SoSFNode SoVertexShape::vertexProperty

  If you set the vertexProperty field, it should be with a coordinate
  node. Otherwise it will simply be ignored. Nodetypes inheriting
  SoVertexShape will then get their coordinate data from the
  vertexProperty node instead of from the global traversal state.

  The vertexProperty field of SoVertexShape-derived nodes breaks
  somewhat with the basic design of Open Inventor, as its contents are
  not passed to the global state. This is done to provide a simple
  path to highly optimized rendering of vertexbased shapes.

*/


SO_NODE_ABSTRACT_SOURCE(SoVertexShape);

/*!
  Constructor.
*/
SoVertexShape::SoVertexShape(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVertexShape);

  SO_NODE_ADD_FIELD(vertexProperty, (NULL));

  this->normalcache = NULL;
}

/*!
  Destructor.
*/
SoVertexShape::~SoVertexShape()
{
  if (this->normalcache) this->normalcache->unref();
}

// doc from superclass
void
SoVertexShape::initClass(void)
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoVertexShape);
}

/*!
  Overloaded to invalidate caches.
*/
void
SoVertexShape::notify(SoNotList * nl)
{
  if (this->normalcache) this->normalcache->invalidate();
  inherited::notify(nl);
}

/*!
  Subclasses should overload this method to generate default
  normals using the SoNormalBundle class. \e TRUE should
  be returned if normals were generated, \e FALSE otherwise.
*/
SbBool
SoVertexShape::generateDefaultNormals(SoState * ,
                                      SoNormalBundle *)
{
  // FIXME: create to SoNormalBundle class.
  COIN_STUB();
  return FALSE;
}


/*!
  Subclasses should overload this method to generate default
  normals using the SoNormalCache class. This is more
  effective than using SoNormalGenerator. Return \e TRUE if
  normals were generated, \e FALSE otherwise.

  This method is not part of the original OIV API. Do not overload it if
  you intend to create a node that can be used on both Coin and OIV.
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
  if (!SoShape::shouldGLRender(action)) return FALSE;

  SoState * state = action->getState();

  SbBool needNormals =
    (SoLightModelElement::get(state) !=
     SoLightModelElement::BASE_COLOR);

  if (needNormals) {
    const SoNormalElement * elem = SoNormalElement::getInstance(state);
    const SoVertexProperty * vp =
      (SoVertexProperty *) this->vertexProperty.getValue();
    if (elem->getNum() == 0 &&
        (!vp || vp->normal.getNum() <= 0)) {
      if (this->normalcache == NULL ||
          !this->normalcache->isValid(state)) {
        generateNormals(state);
      }
#if 0 // OIV doesn't do this, so it's disabled in Coin also.
      // how it's possible to generate correct normals when vertexordering
      // is unknown is a big mystery to me. But I guess when vertexordering
      // is unknown, it defaults to counterclockwise. pederb, 20000404

      // if normals are automatically generated, and vertexordering
      // is unknown, force tow-side lighting
      if (SoShapeHintsElement::getVertexOrdering(state) ==
          SoShapeHintsElement::UNKNOWN_ORDERING) {
        const SoGLShapeHintsElement * sh = (SoGLShapeHintsElement *)
          state->getConstElement(SoGLShapeHintsElement::getClassStackIndex());
        sh->forceSend(TRUE);
      }
#endif // disabled code
    }

    /* will supply unit normals when normal cache is used */
    if (SoVertexShape::willUpdateNormalizeElement(state)) {
      const SoGLNormalizeElement * ne = (SoGLNormalizeElement *)
        state->getConstElement(SoGLNormalizeElement::getClassStackIndex());
      ne->forceSend(TRUE);
    }
  }
  return TRUE;
}

/*!
  Will return TRUE if normal cache is used (normals are then known
  to be unit length).
*/
SbBool
SoVertexShape::willUpdateNormalizeElement(SoState * state) const
{
  const SoNormalElement * elem = SoNormalElement::getInstance(state);
  const SoVertexProperty * vp =
    (SoVertexProperty *) this->vertexProperty.getValue();

  // TODO:
  // could perhaps add an areNormalsUnitLength() to the normal
  // cache also...
  //

  if (elem->getNum() <= 0 && (!vp || vp->normal.getNum() <= 0) &&
      this->normalcache) return TRUE;
  return FALSE;
}


/*!
  FIXME: write function documentation
*/
void
SoVertexShape::setNormalCache(SoState * const state,
                              const int num,
                              const SbVec3f * normals)
{
  if (this->normalcache) this->normalcache->unref();
  // create new normal cache with no dependencies
  this->normalcache = new SoNormalCache(state);
  this->normalcache->ref();
  this->normalcache->set(num, normals);
}

/*!
  FIXME: write function documentation
*/
SoNormalCache *
SoVertexShape::getNormalCache(void) const
{
  return this->normalcache;
}

/*!
  FIXME: write function documentation
*/
void
SoVertexShape::generateNormals(SoState * const state)
{
  SbBool storeinvalid = SoCacheElement::setInvalid(FALSE);

  if (this->normalcache) this->normalcache->unref();
  state->push(); // need to push for cache dependencies
  this->normalcache = new SoNormalCache(state);
  this->normalcache->ref();
  SoCacheElement::set(state, this->normalcache);
  //
  // See if the node supports the Coin-way of generating normals
  //
  if (!generateDefaultNormals(state, this->normalcache)) {
    //
    // try to generate normals the old Inventor way, using
    // SoNormalBundle.
    //
    if (!generateDefaultNormals(state, (SoNormalBundle *)NULL)) {
      // FIXME: what now?
    }
  }
  // FIXME: check if cache has been invalidated. If it has been
  // we should probably unref the cache and set it to NULL?
  // pederb, 20000604

  state->pop(); // don't forget this pop

  SoCacheElement::setInvalid(storeinvalid);
}

/*!
  FIXME: write function documentation
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
  // FIXME: something missing here? 19991009 mortene.
  inherited::write(action);
}
