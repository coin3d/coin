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
  \class SoGLAmbientColorElement Inventor/elements/SoGLAmbientColorElement.h
  \brief The SoGLAmbientColorElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLAmbientColorElement.h>

#include <Inventor/SbName.h>
#include <Inventor/SbColor.h>

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <assert.h>

SO_ELEMENT_SOURCE(SoGLAmbientColorElement);

/*!
  This static method initializes static data for the
  SoGLAmbientColorElement class.
*/

void
SoGLAmbientColorElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoGLAmbientColorElement, inherited);
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLAmbientColorElement::createInstance(void)
*/

SoGLAmbientColorElement::SoGLAmbientColorElement()
{
  setTypeId(classTypeId);
  setStackIndex(classStackIndex);
}

/*!
  The destructor.
*/

SoGLAmbientColorElement::~SoGLAmbientColorElement()
{
}

//! FIXME: write doc.

void
SoGLAmbientColorElement::init(SoState *state)
{
  inherited::init(state);
  this->current.setValue(0.2f, 0.2f, 0.2f);
  this->updategl();
}

//! FIXME: write doc.

void
SoGLAmbientColorElement::push(SoState *state)
{
  inherited::push(state);
  SoGLAmbientColorElement * const element =
    (SoGLAmbientColorElement *)this->next;
  element->current = this->current;
}

//! FIXME: write doc.

void
SoGLAmbientColorElement::pop(SoState *state, const SoElement *prevTopElement)
{
  inherited::pop(state, prevTopElement);
  ((SoGLAmbientColorElement*)prevTopElement)->current = this->current;
}

//! FIXME: write doc.

void
SoGLAmbientColorElement::send(const int index)
{
  int realindex = index < this->numColors ? index : this->numColors - 1;
  if (colors && colors[realindex] != this->current) {
    this->current = colors[realindex];
    this->updategl();
  }
}

//! FIXME: write doc.

void
SoGLAmbientColorElement::setElt(const int32_t numColors,
                                const SbColor * const colors)
{
  inherited::setElt(numColors, colors);
}

//! FIXME: write doc.

void
SoGLAmbientColorElement::updategl()
{
  float col[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  this->current.getValue(col[0], col[1], col[2]);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, col);
}
