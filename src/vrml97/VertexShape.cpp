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
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/misc/SoState.h>
#include <stddef.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

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
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLVertexShape);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(coord, (NULL));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(texCoord, (NULL));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(normal, (NULL));
  SO_VRMLNODE_ADD_FIELD(color, (NULL));
  SO_VRMLNODE_ADD_FIELD(colorPerVertex, (TRUE));
  SO_VRMLNODE_ADD_FIELD(normalPerVertex, (TRUE));

  this->normalcache = NULL;
}

/*!
  Destructor.
*/
SoVRMLVertexShape::~SoVRMLVertexShape()
{
  if (this->normalcache) this->normalcache->unref();
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
  if (f == &this->coord && this->normalcache) {
    this->normalcache->invalidate();
  }
  inherited::notify(list);
}

SbBool
SoVRMLVertexShape::shouldGLRender(SoGLRenderAction * action)
{
  if (!SoShape::shouldGLRender(action)) return FALSE;

  SoState * state = action->getState();

  SbBool needNormals =
    (SoLightModelElement::get(state) !=
     SoLightModelElement::BASE_COLOR);

  if (needNormals) {
    SoVRMLNormal * normal = (SoVRMLNormal*) this->normal.getValue();
    if (!normal || normal->vector.getNum() == 0) {
      if (this->normalcache == NULL ||
          !this->normalcache->isValid(state)) {
        this->generateNormals(state);
      }
    }
  }
  return TRUE;
}

void
SoVRMLVertexShape::setNormalCache(SoState * state,
                                  int num,
                                  SbVec3f * normals)
{
  if (this->normalcache) this->normalcache->unref();
  // create new normal cache with no dependencies
  state->push();
  this->normalcache = new SoNormalCache(state);
  this->normalcache->ref();
  this->normalcache->set(num, normals);
  // force element dependencies
  (void) SoCoordinateElement::getInstance(state);
  state->pop();
}

/*!
  Convenience method that can be used by subclasses to create a new
  normal cache. It takes care of unrefing the old cache and pushing
  and popping the state to create element dependencies. This method is
  not part of the OIV API.
*/
void
SoVRMLVertexShape::generateNormals(SoState * const state)
{
  SbBool storeinvalid = SoCacheElement::setInvalid(FALSE);

  if (this->normalcache) this->normalcache->unref();

  state->push(); // need to push for cache dependencies
  this->normalcache = new SoNormalCache(state);
  this->normalcache->ref();
  SoCacheElement::set(state, this->normalcache);

  // See if the node supports the Coin-way of generating normals
  if (!this->generateDefaultNormals(state, this->normalcache)) {
    // FIXME: implement SoNormalBundle
    if (this->generateDefaultNormals(state, (SoNormalBundle *)NULL)) {
      // FIXME: set generator in normal cache
    }
  }
  state->pop(); // don't forget this pop

  SoCacheElement::setInvalid(storeinvalid);
}

SoNormalCache *
SoVRMLVertexShape::getNormalCache(void) const
{
  return this->normalcache;
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
