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
  \class SoGLViewingMatrixElement Inventor/elements/SoGLViewingMatrixElement.h
  \brief The SoGLViewingMatrixElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLViewingMatrixElement.h>

#include <Inventor/elements/SoModelMatrixElement.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

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
  The destructor.
*/

SoGLViewingMatrixElement::~SoGLViewingMatrixElement(void)
{
}

//! FIXME: write doc.

void
SoGLViewingMatrixElement::init(SoState * state)
{
  inherited::init(state);
  this->state = state;
}

//! FIXME: write doc.

void
SoGLViewingMatrixElement::push(SoState * state)
{
  inherited::push(state);
  this->state = state;
}

//! FIXME: write doc.

void
SoGLViewingMatrixElement::pop(SoState * state,
                              const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
  this->updategl();
}

//! FIXME: write doc.

uint32_t
SoGLViewingMatrixElement::getNodeId(SoState * const state)
{
  const SoReplacedElement *elem = (const SoReplacedElement*)
    SoElement::getConstElement(state, classStackIndex);
  return elem->getNodeId();
}

//! FIXME: write doc.

void
SoGLViewingMatrixElement::setElt(const SbMatrix & matrix)
{
  inherited::setElt(matrix);
  this->updategl();
}

//! FIXME: write doc.

void
SoGLViewingMatrixElement::updategl()
{
  SbMatrix vm = this->viewingMatrix;
  SbBool isIdentity = FALSE;
  const SbMatrix &mm = SoModelMatrixElement::get(this->state, isIdentity);
  if (!isIdentity) {
    vm.multRight(mm);
  }
  glLoadMatrixf((float*)vm);
}
