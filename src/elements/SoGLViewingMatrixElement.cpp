/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
  \class SoGLViewingMatrixElement Inventor/elements/SoGLViewingMatrixElement.h
  \brief The SoGLViewingMatrixElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLViewingMatrixElement.h>

#include <Inventor/SbName.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>

SO_ELEMENT_SOURCE(SoGLViewingMatrixElement);

/*!
  This static method initializes static data for the
  SoGLViewingMatrixElement class.
*/

void
SoGLViewingMatrixElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLViewingMatrixElement, inherited);
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLViewingMatrixElement::createInstance(void)
*/

SoGLViewingMatrixElement::SoGLViewingMatrixElement(void)
  : state(NULL)
{
    setTypeId(SoGLViewingMatrixElement::classTypeId);
    setStackIndex(SoGLViewingMatrixElement::classStackIndex);
}

/*!
  The destructor.
*/

SoGLViewingMatrixElement::~SoGLViewingMatrixElement(void)
{
}

//! FIXME: write doc.

void
SoGLViewingMatrixElement::init(SoState * state)
{
#if 0 // too much debug output.. 981021 mortene.
  SoDebugError::postInfo("SoGLViewingMatrixElement::init",
                         "");
#endif // 0
  inherited::init(state);
  this->state = state;
  glLoadIdentity(); // maybe not strictly necessary?
}

//! FIXME: write doc.

void
SoGLViewingMatrixElement::push(SoState * state)
{
#if 0 // too much debug output.. 981021 mortene.
  SoDebugError::postInfo("SoGLViewingMatrixElement::push()", "");
#endif // 0
  inherited::push(state);
  SoGLViewingMatrixElement *elem = (SoGLViewingMatrixElement*)
    this->next;
  elem->state = state;
  //glPushMatrix();
}

//! FIXME: write doc.

void
SoGLViewingMatrixElement::pop(SoState * state,
                              const SoElement * prevTopElement)
{
#if 0 // too much debug output.. 981021 mortene.
  SoDebugError::postInfo("SoGLViewingMatrixElement::pop()", "");
#endif // 0
  //glPopMatrix();
  inherited::pop(state, prevTopElement);
}

//! FIXME: write doc.

uint32_t
SoGLViewingMatrixElement::getNodeId(SoState * const state)
{
  SoGLViewingMatrixElement *elem = (SoGLViewingMatrixElement*)
    SoElement::getConstElement(state, classStackIndex);
  return elem->getNodeId(state);
}

//! FIXME: write doc.

void
SoGLViewingMatrixElement::setElt(const SbMatrix & matrix)
{
  this->viewingMatrix = matrix;

  SbBool isIdentity = FALSE;
  const SbMatrix &mat = SoModelMatrixElement::get(this->state, isIdentity);
  if (!isIdentity) {
#if COIN_DEBUG // debug
    SoDebugError::postInfo("SoGLViewingMatrixElement::setElt",
                           "mult model");
#endif // debug
    this->viewingMatrix.multRight(mat);
  }

  this->updategl();
}

//! FIXME: write doc.

void
SoGLViewingMatrixElement::updategl()
{
#if 0 // debug
  SoDebugError::postInfo("SoGLViewingMatrixElement::updategl", "");
#endif // debug
  glLoadMatrixf((float*)this->viewingMatrix);
}
