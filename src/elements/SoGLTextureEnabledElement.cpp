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
  \class SoGLTextureEnabledElement Inventor/elements/SoGLTextureEnabledElement.h
  \brief The SoGLTextureEnabledElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLTextureEnabledElement.h>

#include <Inventor/elements/SoShapeStyleElement.h>


#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <assert.h>

SO_ELEMENT_SOURCE(SoGLTextureEnabledElement);

/*!
  This static method initializes static data for the
  SoGLTextureEnabledElement class.
*/

void
SoGLTextureEnabledElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLTextureEnabledElement, inherited);
}

/*!
  The destructor.
*/

SoGLTextureEnabledElement::~SoGLTextureEnabledElement(void)
{
}

//! FIXME: write doc.

void
SoGLTextureEnabledElement::set(SoState * const state,
                               SoNode * const node,
                               const SbBool enabled)
{
  SoInt32Element::set(classStackIndex, state, node, (int32_t) enabled);
  SoShapeStyleElement::setTextureEnabled(state, enabled);
}

//! FIXME: write doc.

void
SoGLTextureEnabledElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
  this->glstate = 0;
  glDisable(GL_TEXTURE_2D);
}

//! FIXME: write doc.

void
SoGLTextureEnabledElement::push(SoState * state)
{
  inherited::push(state);
  ((SoGLTextureEnabledElement*)this->next)->data = this->data;
  ((SoGLTextureEnabledElement*)this->next)->glstate = this->glstate;
}

//! FIXME: write doc.

void
SoGLTextureEnabledElement::pop(SoState * state,
                               const SoElement * prevTopElement)
{
  ((SoGLTextureEnabledElement*)prevTopElement)->glstate = this->glstate;
  inherited::pop(state, prevTopElement);
}

void
SoGLTextureEnabledElement::evaluate() const
{
  ((SoGLTextureEnabledElement*)this)->updategl();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoGLTextureEnabledElement::set(SoState * const state, const SbBool enabled)
{
  set(state, NULL, enabled);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SbBool
SoGLTextureEnabledElement::get(SoState * const state)
{
  return (SbBool) SoInt32Element::get(classStackIndex, state);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SbBool
SoGLTextureEnabledElement::getDefault()
{
  return FALSE;
}

//! FIXME: write doc.

void
SoGLTextureEnabledElement::setElt(int32_t value)
{
  if (this->data != value) {
    inherited::setElt(value);
    this->data = value;
    updategl();
  }
}

//! FIXME: write doc.

void
SoGLTextureEnabledElement::updategl()
{
  if (this->data != this->glstate) {
    this->glstate = this->data;
    if (this->data) glEnable(GL_TEXTURE_2D);
    else glDisable(GL_TEXTURE_2D);
  }
}
