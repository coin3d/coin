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
  \class SoTextureCoordinateBundle include/Inventor/bundles/SoTextureCoordinateBundle.h
  \brief The SoTextureCoordinateBundle class simplifies texture coordinate handling.
  bla, bla, bla, FIXME: doc
*/

#include <Inventor/bundles/SoTextureCoordinateBundle.h>

#include <coindefs.h> // COIN_STUB()
#include <Inventor/misc/SoState.h>
#include <Inventor/elements/SoTextureImageElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>

#include <Inventor/nodes/SoVertexShape.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/actions/SoPickAction.h>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

#include <GL/gl.h>
#include <assert.h>

#define FLAG_FUNCTION           0x01
#define FLAG_NEEDCOORDS         0x02
#define FLAG_DEFAULT            0x04
#define FLAG_DIDPUSH            0x08

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
  if (forRendering && !SoGLTextureEnabledElement::get(this->state))
    return;
  if (!forRendering) {

    if (action->getTypeId().isDerivedFrom(SoPickAction::getClassTypeId())) {
      // FIXME: this is a temporary fix. Since SoTextureImageElement is not
      // enabled for SoPickAction or SoRayPickAction (I think it perhaps 
      // should be) it's not possible to detect if there's an active 
      // texture in the state; if texture coordinates are needed. Since it would 
      // be a _big_ waste of CPU-time to simply generate default texture
      // coordinates even if a shape isn't textured (most shapes use 
      // SoShape::generatePrimitives() for picking), we should find a solution 
      // to this. For now, I just disable texture coordinates when picking.
      // I haven't implemented proper support for calculating them when picking
      // anyway.
      // pederb, 20000218
      return;
    }
    
    SbVec2s dummysize;
    int dummynum;
    if (!SoTextureImageElement::getImage(this->state, dummysize, dummynum))
      return;
  }
  
  assert(action->getCurPathTail()->isOfType(SoVertexShape::getClassTypeId()));
  this->shapenode = (SoVertexShape*)action->getCurPathTail();
  
  SoVertexProperty *vp = (SoVertexProperty*)
    this->shapenode->vertexProperty.getValue();
  
  if (vp && vp->texCoord.getNum() > 0) {    
    // FIXME:
    // the SoVertexProperty node is a bad design idea, IMHO
    // Just push and place needed stuff on the element stack for now.
    // will probably optimize later.
    // pederb, 20000218
    //
    
    const SbVec2f *texcoords = vp->texCoord.getValues(0);
    this->state->push();
    this->flags |= FLAG_DIDPUSH;
    if (forRendering) {
      SoGLTextureCoordinateElement::setTexGen(this->state,
                                              vp, NULL);
    }
    SoTextureCoordinateElement::set2(this->state, vp,
                                     vp->texCoord.getNum(),
                                     vp->texCoord.getValues(0));
  }
  
  this->coordElt = SoTextureCoordinateElement::getInstance(this->state);
  switch (coordElt->getType()) {
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
    break;
  default:
    assert(0 && "FIXME: unknown CoordType");
    break;
  }

  this->glElt = NULL; 
  if (forRendering) {
    this->glElt = (SoGLTextureCoordinateElement*) coordElt;
  }
  if ((this->flags & FLAG_DEFAULT) && !setUpDefault) {
    //
    // FIXME: I couldn't be bothered to support this yet. It is for picking 
    // optimization only, pederb, 20000218
    //
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
  Returns \e TRUE if texture coordinates is needed dureing rendering.
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
  assert(coordElt != NULL && (this->flags & FLAG_FUNCTION));
  if (this->flags & FLAG_DEFAULT) {
    SbVec2f pt(point[this->defaultdim0]-this->defaultorigo[0],
               point[this->defaultdim1]-this->defaultorigo[1]);
    //
    // FIXME: when support for 3D textures is implemented, should
    // we provide the third texture coordinate also? pederb
    //
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

  if (forRendering) {
    if (!this->flags & FLAG_DIDPUSH) {
      this->state->push();
      this->flags |= FLAG_DIDPUSH;
    }
    // have glElt generate the default texture coordinates using a
    // callback to this instance.
    SoTextureCoordinateElement::setFunction(this->state, this->shapenode,
                                            SoTextureCoordinateBundle::defaultCB,
                                            this);
    this->coordElt = SoTextureCoordinateElement::getInstance(this->state);
  }
  
  //
  // calculate needed stuff for default mapping
  //
  SbBox3f box;
  SbVec3f center;
  // this could be very slow, but if you're looking for speed, default
  // texture coordinate mapping shouldn't be used. We might optimize this
  // by using a SoTextureCoordinateCache soon though. pederb, 20000218
  this->shapenode->computeBBox(action, box, center);
  SbVec3f size;
  box.getSize(size[0], size[1], size[2]);
  
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
  
  SbVec3f origo = box.getMin();
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
