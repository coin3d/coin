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
  \class SoGLSpecularColorElement Inventor/elements/SoGLSpecularColorElement.h
  \brief The SoGLSpecularColorElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLSpecularColorElement.h>


#include <assert.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

SO_ELEMENT_SOURCE(SoGLSpecularColorElement);

/*!
  This static method initializes static data for the
  SoGLSpecularColorElement class.
*/

void
SoGLSpecularColorElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoGLSpecularColorElement, inherited);
}

/*!
  The destructor.
*/

SoGLSpecularColorElement::~SoGLSpecularColorElement()
{
}

//! FIXME: write doc.

void
SoGLSpecularColorElement::init(SoState *state)
{
  inherited::init(state);
  this->current.setValue(0.0f, 0.0f, 0.0f);
  this->updategl();
}

//! FIXME: write doc.

void
SoGLSpecularColorElement::push(SoState * state)
{
  inherited::push(state);
  SoGLSpecularColorElement * const prev =
    (SoGLSpecularColorElement *)this->getNextInStack();
  this->current = prev->current;
}

//! FIXME: write doc.

void
SoGLSpecularColorElement::pop(SoState *state, const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
  this->current = ((SoGLSpecularColorElement*)prevTopElement)->current;
}

//! FIXME: write doc.

void
SoGLSpecularColorElement::send(const int index)
{
  int realindex = index < this->numColors ? index : this->numColors - 1;
  if (colors && colors[realindex] != this->current) {
    this->current = colors[realindex];
    this->updategl();
  }
}

//! FIXME: write doc.

void
SoGLSpecularColorElement::setElt(const int32_t numColors,
                                 const SbColor * const colors)
{
  inherited::setElt(numColors, colors);
}

//! FIXME: write doc.

void
SoGLSpecularColorElement::updategl()
{
  float col[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  this->current.getValue(col[0], col[1], col[2]);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, col);
}
