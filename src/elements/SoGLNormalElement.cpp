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
  \class SoGLNormalElement Inventor/elements/SoGLNormalElement.h
  \brief The SoGLNormalElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLNormalElement.h>

#include <Inventor/SbName.h>

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <assert.h>

SO_ELEMENT_SOURCE(SoGLNormalElement);

/*!
  This static method initializes static data for the SoGLNormalElement class.
*/

void
SoGLNormalElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLNormalElement, inherited);
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLNormalElement::createInstance(void)
*/

SoGLNormalElement::SoGLNormalElement(void)
{
    setTypeId(SoGLNormalElement::classTypeId);
    setStackIndex(SoGLNormalElement::classStackIndex);
}

/*!
  The destructor.
*/

SoGLNormalElement::~SoGLNormalElement(void)
{
}

//! FIXME: write doc.

void
SoGLNormalElement::init(SoState * state)
{
  inherited::init(state);
}

//! FIXME: write doc.

void
SoGLNormalElement::send(const int index) const
{
  assert(index >= 0 && index < this->numNormals);
  glNormal3fv(this->normals[index].getValue());
}
