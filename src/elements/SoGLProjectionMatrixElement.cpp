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
  \class SoGLProjectionMatrixElement Inventor/elements/SoGLProjectionMatrixElement.h
  \brief The SoGLProjectionMatrixElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLProjectionMatrixElement.h>

#include <Inventor/SbName.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <assert.h>

SO_ELEMENT_SOURCE(SoGLProjectionMatrixElement);

/*!
  This static method initializes static data for the
  SoGLProjectionMatrixElement class.
*/

void
SoGLProjectionMatrixElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLProjectionMatrixElement, inherited);
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLProjectionMatrixElement::createInstance(void)
*/

SoGLProjectionMatrixElement::SoGLProjectionMatrixElement(void)
{
    setTypeId(SoGLProjectionMatrixElement::classTypeId);
    setStackIndex(SoGLProjectionMatrixElement::classStackIndex);
}

/*!
  The destructor.
*/

SoGLProjectionMatrixElement::~SoGLProjectionMatrixElement(void)
{
}

//! FIXME: write doc.

void
SoGLProjectionMatrixElement::pop(SoState * state,
                                 const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
  ((SoGLProjectionMatrixElement*)prevTopElement)->updategl();
}

//! FIXME: write doc.

void
SoGLProjectionMatrixElement::setElt(const SbMatrix & matrix)
{
  inherited::setElt(matrix);
  this->updategl();
}

//! FIXME: write doc.

void
SoGLProjectionMatrixElement::updategl(void)
{
#if 0 // debug
  SoDebugError::postInfo("SoGLProjectionMatrixElement::updategl", "");
#endif // debug
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf((float*)this->projectionMatrix);
  glMatrixMode(GL_MODELVIEW);
}
