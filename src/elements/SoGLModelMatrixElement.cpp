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
  \class SoGLModelMatrixElement Inventor/elements/SoGLModelMatrixElement.h
  \brief The SoGLModelMatrixElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLModelMatrixElement.h>
#include <Inventor/elements/SoGLViewingMatrixElement.h>
#include <Inventor/SbRotation.h>
#include <Inventor/SbVec3f.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#if HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

SO_ELEMENT_SOURCE(SoGLModelMatrixElement);

/*!
  This static method initializes static data for the
  SoGLModelMatrixElement class.
*/

void
SoGLModelMatrixElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLModelMatrixElement, inherited);
}

/*!
  The destructor.
*/

SoGLModelMatrixElement::~SoGLModelMatrixElement(void)
{
}

//! FIXME: write doc.

void
SoGLModelMatrixElement::init(SoState * state)
{
  this->state = state;
  this->viewEltNodeId = 0;
  inherited::init(state);
}

//! FIXME: write doc.

void
SoGLModelMatrixElement::push(SoState * state)
{
  glPushMatrix();
  inherited::push(state);
  SoGLModelMatrixElement *elem = (SoGLModelMatrixElement*)
    this->next;
  elem->state = state;
}

//! FIXME: write doc.

void
SoGLModelMatrixElement::pop(SoState * state,
                            const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
  glPopMatrix();
}

//! FIXME: write doc.

void
SoGLModelMatrixElement::makeEltIdentity()
{
  const SbMatrix &mat = SoViewingMatrixElement::get(this->state);
  glLoadMatrixf(mat[0]);
  inherited::makeEltIdentity();
}

//! FIXME: write doc.

void
SoGLModelMatrixElement::setElt(const SbMatrix &matrix)
{
  inherited::setElt(matrix);
  const SbMatrix &mat = SoViewingMatrixElement::get(this->state);
  glLoadMatrixf(mat[0]);
  glMultMatrixf(matrix[0]);
}

//! FIXME: write doc.

void
SoGLModelMatrixElement::multElt(const SbMatrix &matrix)
{
  glMultMatrixf(matrix[0]);
  inherited::multElt(matrix);
}

//! FIXME: write doc.

void
SoGLModelMatrixElement::translateEltBy(const SbVec3f &translation)
{
  glTranslatef(translation[0], translation[1], translation[2]);
  inherited::translateEltBy(translation);
}

//! FIXME: write doc.

void
SoGLModelMatrixElement::rotateEltBy(const SbRotation &rotation)
{
  SbVec3f axis;
  float angle;
  rotation.getValue(axis, angle);
  glRotatef(angle*180.0f/float(M_PI), axis[0], axis[1], axis[2]);
  inherited::rotateEltBy(rotation);
}

//! FIXME: write doc.

void
SoGLModelMatrixElement::scaleEltBy(const SbVec3f &scaleFactor)
{
  glScalef(scaleFactor[0], scaleFactor[1], scaleFactor[2]);
  inherited::scaleEltBy(scaleFactor);
}

//! FIXME: write doc.

SbMatrix
SoGLModelMatrixElement::pushMatrixElt()
{
  this->viewEltNodeId = SoGLViewingMatrixElement::getNodeId(this->state);
  glPushMatrix();
  return inherited::pushMatrixElt();
}

//! FIXME: write doc.

void
SoGLModelMatrixElement::popMatrixElt(const SbMatrix &matrix)
{
  glPopMatrix();
  if (this->viewEltNodeId != SoGLViewingMatrixElement::getNodeId(this->state)) {
    this->setElt(matrix);
  }
  inherited::popMatrixElt(matrix);
}
