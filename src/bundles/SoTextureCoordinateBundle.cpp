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
  \class SoTextureCoordinateBundle include/Inventor/bundles/SoTextureCoordinateBundle.h
  \brief The SoTextureCoordinateBundle class simplifies texture coordinate handling.
  \ingroup bundles

  It is unlikely that application programmers should need to know how
  to use this class, as it is mostly intended for internal use.
*/
// FIXME: document class better.


#include <Inventor/bundles/SoTextureCoordinateBundle.h>

#include <Inventor/misc/SoState.h>
#include <Inventor/elements/SoTextureImageElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLTexture3EnabledElement.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>

#include <Inventor/nodes/SoVertexShape.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/caches/SoBoundingBoxCache.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <Inventor/system/gl.h>

#include <assert.h>

#define FLAG_FUNCTION           0x01
#define FLAG_NEEDCOORDS         0x02
#define FLAG_DEFAULT            0x04
#define FLAG_DIDPUSH            0x08
#define FLAG_3DTEXTURES         0x10

/*!
  Constructor with \a action being the action applied to the node.
  The \a forRendering parameter must be \e TRUE if the bundle is to
  be used for sending texture coordinates to GL during rendering.
  The \a setUpDefault must be \e TRUE if default texture coordinates
  should be generated.
*/
SoTextureCoordinateBundle::
SoTextureCoordinateBundle(SoAction * const action,
                          const SbBool forRendering,
                          const SbBool setUpDefault)
  : SoBundle(action)
{
  this->flags = 0;
  //
  // return immediately if there is no texture
  //
  if (!SoTextureEnabledElement::get(this->state) &&
      !SoTexture3EnabledElement::get(this->state))
    return;

  // FIXME: shapenode is not guaranteed to be a SoVertexShape, since
  // the VRML nodes have a different node hierarchy. It is safe to
  // assume that shapenode is of type SoShape though, so remember to
  // cast to SoShape before doing any operations on the node. For Coin
  // V2.0 we'll change the shapenode member to be of type SoShape
  this->shapenode = (SoVertexShape*) action->getCurPathTail();

  this->coordElt = SoTextureCoordinateElement::getInstance(this->state);
  switch (this->coordElt->getType()) {
  case SoTextureCoordinateElement::DEFAULT:
    this->initDefault(action, forRendering);
    break;
  case SoTextureCoordinateElement::EXPLICIT:
    if (this->coordElt->getNum() > 0) {
      this->flags |= FLAG_NEEDCOORDS;
    }
    else {
      this->initDefault(action, forRendering);
    }
    break;
  case SoTextureCoordinateElement::FUNCTION:
    this->flags |= FLAG_FUNCTION;
    this->flags |= FLAG_NEEDCOORDS; // not automatically generated
    break;

  case SoTextureCoordinateElement::TEXGEN:
    // texcoord won't be needed. This will only happen during SoGLRenderAction,
    // when GL generates texture coorinates. Therefore, we will not set
    // the FLAG_NEEDCOORDS here.
    this->flags |= FLAG_FUNCTION;
    if (!forRendering) {
      this->flags |= FLAG_NEEDCOORDS;
    }
    break;
  default:
    assert(0 && "unknown CoordType");
    break;
  }

  this->glElt = NULL;
  if (forRendering) {
    this->glElt = (SoGLTextureCoordinateElement*) this->coordElt;
  }
  if ((this->flags & FLAG_DEFAULT) && !setUpDefault) {
    // FIXME: I couldn't be bothered to support this yet. It is for picking
    // optimization only, I think. pederb, 20000218
  }
}

/*!
  Destructor.
*/
SoTextureCoordinateBundle::~SoTextureCoordinateBundle()
{
  if (this->flags & FLAG_DIDPUSH) this->state->pop();
}

/*!
  Returns \e TRUE if texture coordinates is needed during rendering.
*/
SbBool
SoTextureCoordinateBundle::needCoordinates() const
{
  return (this->flags & FLAG_NEEDCOORDS) != 0;
}

/*!
  Returns \e TRUE if a texture coordinate function should be used.
*/
SbBool
SoTextureCoordinateBundle::isFunction() const
{
  return (this->flags & FLAG_FUNCTION) != 0;
}

/*!
  Returns the texture coordinates based on \a point and \a normal.
  Should only be used if SoTextureCoordinateBundle::isFunction() is \a TRUE.
*/
const SbVec4f &
SoTextureCoordinateBundle::get(const SbVec3f &point, const SbVec3f &normal)
{
  assert(this->coordElt != NULL && (this->flags & FLAG_FUNCTION));
  if (this->flags & FLAG_DEFAULT) {
    SbVec3f pt;
    if (this->flags & FLAG_3DTEXTURES) {
      pt = point - this->defaultorigo;
      this->dummyInstance[2] = pt[2]/this->defaultsize[2];
    }
    else {
      pt.setValue(point[this->defaultdim0]-this->defaultorigo[0],
                  point[this->defaultdim1]-this->defaultorigo[1],
                  0.0f);
    }
    this->dummyInstance[0] = pt[0]/this->defaultsize[0];
    this->dummyInstance[1] = pt[1]/this->defaultsize[1];
    return this->dummyInstance;
  }
  else {
    return coordElt->get(point, normal);
  }
}

/*!
  Returns the texture coordinates at index \a index.
  Should only be used if SoTextureCoordinateBundle::isFunction() is \a FALSE.
*/
const SbVec4f &
SoTextureCoordinateBundle::get(const int index)
{
  assert(coordElt && !(this->flags & FLAG_FUNCTION));
  return coordElt->get4(index);
}


/*!
  \fn void SoTextureCoordinateBundle::send(const int index) const
  Send texture coordinates to GL. Should only be used if
  SoTextureCoordinateBundle::isFunction() is \a FALSE.
*/

/*!
  \fn void SoTextureCoordinateBundle::send(const int index, const SbVec3f &point, const SbVec3f &normal) const
  Convenience function that will make it transparent to the rendering
  code if ordinary texture coordinates or function texture coordinates
  are used.
*/


void
SoTextureCoordinateBundle::initDefault(SoAction * const action,
                                       const SbBool forRendering)
{
  this->flags |= FLAG_NEEDCOORDS;
  this->flags |= FLAG_DEFAULT;
  this->flags |= FLAG_FUNCTION;

  if (!(this->flags & FLAG_DIDPUSH)) {
    this->state->push();
    this->flags |= FLAG_DIDPUSH;
  }
  // have coordelt generate the default texture coordinates using a
  // callback to this instance.
  SoTextureCoordinateElement::setFunction(this->state, this->shapenode,
                                          SoTextureCoordinateBundle::defaultCB,
                                          this);
  this->coordElt = SoTextureCoordinateElement::getInstance(this->state);

  //
  // calculate needed stuff for default mapping
  //
  SbBox3f box;
  SbVec3f center;
  // this could be very slow, but if you're looking for speed, default
  // texture coordinate mapping shouldn't be used. We might optimize this
  // by using a SoTextureCoordinateCache soon though. pederb, 20000218

  // just in case, cast to SoShape since it's not guaranteed that
  // shapenode is of type SoVertexShape.
  SoShape * shape = (SoShape*) this->shapenode;
  const SoBoundingBoxCache * bboxcache = shape->getBoundingBoxCache();
  if (bboxcache && bboxcache->isValid(action->getState())) {
    box = bboxcache->getProjectedBox();
    if (bboxcache->isCenterSet()) center = bboxcache->getCenter();
    else center = box.getCenter();
  }
  else {
    shape->computeBBox(action, box, center);
  }

  // just use som default values if the shape bbox is empty
  SbVec3f size(1.0f, 1.0f, 1.0f);
  SbVec3f origo(0.f, 0.0f, 0.0f);
  if (!box.isEmpty()) {
    box.getSize(size[0], size[1], size[2]);
    origo = box.getMin();
  }

  // Map S,T,R to X,Y,Z for 3D texturing
  if (SoTexture3EnabledElement::get(this->state)) {
    this->flags |= FLAG_3DTEXTURES;
    this->defaultdim0 = 0;
    this->defaultdim1 = 1;
    
    // disable texture coordinate generation if empty or one
    // dimensional bounding box.
    if (size[2] <= 0.0f) {
      this->flags &= ~(FLAG_NEEDCOORDS|FLAG_DEFAULT|FLAG_FUNCTION);
      return;
    }
    
    this->defaultorigo[2] = origo[2];
    this->defaultsize[2] = size[2];
    assert(this->defaultsize[2] > 0.0f);
  }
  else { // 2D textures
    this->flags &= ~FLAG_3DTEXTURES;
    // find the two biggest dimensions
    int smallest = 0;
    float smallval = size[0];
    if (size[1] < smallval) {
      smallest = 1;
      smallval = size[1];
    }
    if (size[2] < smallval) {
      smallest = 2;
    }
    
    this->defaultdim0 = (smallest + 1) % 3;
    this->defaultdim1 = (smallest + 2) % 3;
    
    if (size[this->defaultdim0] == size[this->defaultdim1]) {
      // FIXME: this is probably an OIV bug. The OIV man pages are not
      // clear on this point (surprise), but the VRML specification states
      // that if the two dimensions are equal, the ordering X>Y>Z should
      // be used.
#if 0 // the correct way to do it
      if (this->defaultdim0 > this->defaultdim1) {
        SbSwap(this->defaultdim0, this->defaultdim1);
      }
#else // the OIV way to do it.
      if (this->defaultdim0 < this->defaultdim1) {
        SbSwap(this->defaultdim0, this->defaultdim1);
      }
#endif // OIV compatibility fix
    }
    else if (size[this->defaultdim0] < size[this->defaultdim1]) {
      SbSwap(this->defaultdim0, this->defaultdim1);
    }
  }    

  // disable texture coordinate generation if empty or one
  // dimensional bounding box.
  if (size[this->defaultdim0] <= 0.0f ||
      size[this->defaultdim1] <= 0.0f) {
    this->flags &= ~(FLAG_NEEDCOORDS|FLAG_DEFAULT|FLAG_FUNCTION);
    return;
  }

  this->defaultorigo[0] = origo[this->defaultdim0];
  this->defaultorigo[1] = origo[this->defaultdim1];
  this->defaultsize[0] = size[this->defaultdim0];
  this->defaultsize[1] = size[this->defaultdim1];

  this->dummyInstance[2] = 0.0f;
  this->dummyInstance[3] = 1.0f;

  assert(this->defaultsize[0] > 0.0f);
  assert(this->defaultsize[1] > 0.0f);
}

const SbVec4f &
SoTextureCoordinateBundle::defaultCB(void * userdata,
                                     const SbVec3f & point,
                                     const SbVec3f & normal)
{
  return ((SoTextureCoordinateBundle*)userdata)->get(point, normal);
}
