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
  \class SoGLNormalElement Inventor/elements/SoGLNormalElement.h
  \brief The SoGLNormalElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLNormalElement.h>


#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#if HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H
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
