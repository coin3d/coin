/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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
  \class SoGLProjectionMatrixElement Inventor/elements/SoGLProjectionMatrixElement.h
  \brief The SoGLProjectionMatrixElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLProjectionMatrixElement.h>


#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#if HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H
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
