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
  \class SoGLDiffuseColorElement Inventor/elements/SoGLDiffuseColorElement.h
  \brief The SoGLDiffuseColorElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLDiffuseColorElement.h>

#include <Inventor/SbColor.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#if HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>

#include <assert.h>

SO_ELEMENT_SOURCE(SoGLDiffuseColorElement);

/*!
  This static method initializes static data for the
  SoGLDiffuseColorElement class.
*/

void
SoGLDiffuseColorElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoGLDiffuseColorElement, inherited);
}

/*!
  The destructor.
*/

SoGLDiffuseColorElement::~SoGLDiffuseColorElement()
{
}

//! FIXME: write doc.

void
SoGLDiffuseColorElement::init(SoState *state)
{
  inherited::init(state);
  this->current.setValue(0.8f, 0.8f, 0.8f, 1.0f);
  this->currentpacked = 0xccccccff;
  this->currentispacked = FALSE;
  this->updategl(SbVec4f(0.8f, 0.8f, 0.8f, 1.0f));
}

//! FIXME: write doc.

void
SoGLDiffuseColorElement::push(SoState * state)
{
  inherited::push(state);
  SoGLDiffuseColorElement * const prev =
    (SoGLDiffuseColorElement *)this->getNextInStack();
  this->currentispacked = prev->currentispacked;
  if (this->currentispacked) {
    this->currentpacked = prev->currentpacked;
  }
  else {
    this->current = prev->current;
  }
}

//! FIXME: write doc.

void
SoGLDiffuseColorElement::pop(SoState *state, const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
  SoGLDiffuseColorElement * const prev =
    (SoGLDiffuseColorElement *)prevTopElement;
  this->currentispacked = prev->currentispacked;
  if (this->currentispacked) {
    this->currentpacked = prev->currentpacked;
  }
  else {
    this->current = prev->current;
  }
}


inline void 
SoGLDiffuseColorElement::updategl(const uint32_t col)
{
  this->currentpacked = col;
  glColor4ub((col>>24)&0xff, (col>>16)&0xff, (col>>8)&0xff, col&0xff);
}

inline void 
SoGLDiffuseColorElement::updategl(const SbVec4f & col)
{
  this->current = col;
  glColor4fv((const GLfloat*) &col);
}


//! FIXME: write doc.

void
SoGLDiffuseColorElement::send(const int index, const float alpha)
{
  int realindex = SbClamp(index, 0, this->numColors - 1);
  if (this->colors) {
    const SbVec3f & c = this->colors[realindex];
    SbVec4f col(c[0], c[1], c[2], alpha);

    if (!this->currentispacked) {
      if (this->current != col) this->updategl(col);
    }
    else {
      this->currentispacked = FALSE;
      this->updategl(col);
    }
  }
  else if (this->packedColors) {
    uint32_t col = this->packedColors[realindex];
    if (this->currentispacked) {
      if (col != this->currentpacked) this->updategl(col);
    }
    else {
      this->currentispacked = TRUE;
      this->updategl(col);
    }
  }
}

/*!
  FIXME: write doc.
*/
void
SoGLDiffuseColorElement::send(const int index)
{
  this->send(index, this->current[3]);
}

/*!
  Do not use. Provided only to support SoGLLazyElement.
*/
void
SoGLDiffuseColorElement::sendOnePacked(const uint32_t packedcol)
{
  if (this->currentispacked) {
    if (this->currentpacked != packedcol) this->updategl(packedcol);
  }
  else {
    this->currentispacked = TRUE;
    this->updategl(packedcol);
  }
}
