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
  \class SoGLTextureCoordinateElement Inventor/elements/SoGLTextureCoordinateElement.h
  \brief The SoGLTextureCoordinateElement class stores the current gltexture
  coordinates.
*/

#include <Inventor/elements/SoGLTextureCoordinateElement.h>

#include <Inventor/elements/SoShapeStyleElement.h>

#include <Inventor/SbName.h>

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

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
  A constructor.  Can't be used directly.

  \sa void * SoGLTextureCoordinateElement::createInstance(void)
*/

SoGLTextureCoordinateElement::SoGLTextureCoordinateElement()
{
    setTypeId(SoGLTextureCoordinateElement::classTypeId);
    setStackIndex(SoGLTextureCoordinateElement::classStackIndex);
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
//   SoGLTextureCoordinateElement *elem = (SoGLTextureCoordinateElement*)
//     this->next;
}

//!  FIXME: write doc.

void
SoGLTextureCoordinateElement::pop(SoState * state,
                                  const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
  ((SoGLTextureCoordinateElement*)prevTopElement)->doCallback();
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
