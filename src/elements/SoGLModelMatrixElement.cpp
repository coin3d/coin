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
  \class SoGLModelMatrixElement Inventor/elements/SoGLModelMatrixElement.h
  \brief The SoGLModelMatrixElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLModelMatrixElement.h>
#include <Inventor/elements/SoGLViewingMatrixElement.h>
#include <Inventor/SbRotation.h>
#include <Inventor/SbVec3f.h>

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

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
}
