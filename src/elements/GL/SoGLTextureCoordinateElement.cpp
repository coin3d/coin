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
  \class SoGLTextureCoordinateElement Inventor/elements/SoGLTextureCoordinateElement.h
  \brief The SoGLTextureCoordinateElement class stores the current gltexture coordinates.
  \ingroup elements
*/

#include <Inventor/elements/SoGLTextureCoordinateElement.h>

#include <Inventor/elements/SoShapeStyleElement.h>


#include <assert.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>


SO_ELEMENT_SOURCE(SoGLTextureCoordinateElement);

/*!
  This static method initializes static data for the
  SoGLTextureCoordinateElement class.
*/

void
SoGLTextureCoordinateElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLTextureCoordinateElement, inherited);
}

/*!
  The destructor.
*/

SoGLTextureCoordinateElement::~SoGLTextureCoordinateElement()
{
}

//!  FIXME: write doc.

void
SoGLTextureCoordinateElement::init(SoState * state)
{
  inherited::init(state);
  this->texgenCB = NULL;
  this->texgenData = NULL;
}

//!  FIXME: write doc.

void
SoGLTextureCoordinateElement::push(SoState * state)
{
  SoGLTextureCoordinateElement * prev = (SoGLTextureCoordinateElement*)this->getNextInStack();
  this->texgenCB = prev->texgenCB;
  this->texgenData = prev->texgenData;
  // capture previous element since we might or might not change the
  // GL state in set/pop
  prev->capture(state);
}

//!  FIXME: write doc.

void
SoGLTextureCoordinateElement::pop(SoState * state,
                                  const SoElement * prevTopElement)
{
  SoGLTextureCoordinateElement * prev = (SoGLTextureCoordinateElement*) prevTopElement; 

  // only send to GL if something has changed
  if (this->texgenCB && !prev->texgenCB) {
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);
    glEnable(GL_TEXTURE_GEN_Q);
  }
  else if (!this->texgenCB && prev->texgenCB) {
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);
    glDisable(GL_TEXTURE_GEN_Q);
  }
  this->doCallback();
}

//!  FIXME: write doc.

void
SoGLTextureCoordinateElement::setTexGen(SoState * const state,
                                        SoNode * const node,
                                        SoTexCoordTexgenCB * const texgenFunc,
                                        void * const texgenData,
                                        SoTextureCoordinateFunctionCB * const func,
                                        void * const funcData)
{
  SoShapeStyleElement::setTextureFunction(state, texgenFunc && !func);

  SoTextureCoordinateElement::setFunction(state, node, func, funcData);

  SoGLTextureCoordinateElement *element = (SoGLTextureCoordinateElement *)
    SoReplacedElement::getElement(state, classStackIndex, node);
  if (element) {
    element->setElt(texgenFunc, texgenData);
  }
}

//!  FIXME: write doc.

SoGLTextureCoordinateElement::CoordType
SoGLTextureCoordinateElement::getType() const
{
  if (this->texgenCB) return NONE;
  return inherited::getType();
}

//!  FIXME: write doc.

const SoGLTextureCoordinateElement *
SoGLTextureCoordinateElement::getInstance(SoState * const state)
{
  return (SoGLTextureCoordinateElement*)
    SoElement::getConstElement(state, classStackIndex);
}

//!  FIXME: write doc.

void
SoGLTextureCoordinateElement::send(const int index) const
{
  assert(this->whatKind == EXPLICIT);
  assert(index < this->numCoords);
  if (this->coordsDimension==2)
    glTexCoord2fv(coords2[index].getValue());
  else if (this->coordsDimension==3)
    glTexCoord3fv(coords3[index].getValue());
  else // this->coordsDimension==4
    glTexCoord4fv(coords4[index].getValue());
}

//!  FIXME: write doc.

void
SoGLTextureCoordinateElement::send(const int index,
                                   const SbVec3f &c,
                                   const SbVec3f &n) const
{
  if (this->whatKind == FUNCTION) {
    assert(this->funcCB);
    const SbVec4f &tc = this->funcCB(this->funcCBData, c, n);
    glTexCoord4fv(tc.getValue());
  }
  else {
    assert(this->whatKind == EXPLICIT);
    //
    // FIXME: these tests are just here to avoid crashes when illagal
    // files are rendered. Will remove later, when we implement a
    // SoVerifyGraphAction or something. pederb, 20000218
    //
    if (index < 0) return;
    if (index >= this->numCoords) return;
    if (this->coordsDimension==2)
      glTexCoord2fv(coords2[index].getValue());
    else if (this->coordsDimension==3)
      glTexCoord3fv(coords3[index].getValue());
    else // this->coordsDimension==4
      glTexCoord4fv(coords4[index].getValue());
  }
}

//!  FIXME: write doc.

void
SoGLTextureCoordinateElement::setElt(SoTexCoordTexgenCB * func,
                                     void *data)
{
  if (func && !this->texgenCB) {
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);
    glEnable(GL_TEXTURE_GEN_Q);    
  }
  else if (!func && this->texgenCB) {
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);
    glDisable(GL_TEXTURE_GEN_Q);
  }
  if (func) {
    this->whatKind = FUNCTION;
  }
  this->texgenCB = func;
  this->texgenData = data;
  this->doCallback();
}

void
SoGLTextureCoordinateElement::doCallback() const
{
  if (this->texgenCB) {
    this->texgenCB(this->texgenData);
  }
}
