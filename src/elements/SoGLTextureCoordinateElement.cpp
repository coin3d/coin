/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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
  \class SoGLTextureCoordinateElement Inventor/elements/SoGLTextureCoordinateElement.h
  \brief The SoGLTextureCoordinateElement class stores the current gltexture
  coordinates.
*/

#include <Inventor/elements/SoGLTextureCoordinateElement.h>

#include <Inventor/elements/SoShapeStyleElement.h>


#include <assert.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#if HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>


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
  inherited::push(state);
}

//!  FIXME: write doc.

void
SoGLTextureCoordinateElement::pop(SoState * state,
                                  const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
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
  element->setElt(texgenFunc, texgenData);
  element->doCallback();
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
  if (this->coordsAre2D)
    glTexCoord2fv(coords2[index].getValue());
  else
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
    if (this->coordsAre2D)
      glTexCoord2fv(coords2[index].getValue());
    else
      glTexCoord4fv(coords4[index].getValue());
  }
}

//!  FIXME: write doc.

void
SoGLTextureCoordinateElement::setElt(SoTexCoordTexgenCB * func,
                                     void *data)
{
  this->texgenCB = func;
  this->texgenData = data;
}

void
SoGLTextureCoordinateElement::doCallback() const
{
  if (this->texgenCB) {
    this->texgenCB(this->texgenData);
  }
  else {
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
  }
}
