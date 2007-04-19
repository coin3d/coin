/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoTextureImageElement Inventor/elements/SoTextureImageElement.h
  \brief The SoTextureImageElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoBumpMapElement.h>
#include <assert.h>

/*!
  \fn SoBumpMapElement::size

  FIXME: write doc.
*/

/*!
  \fn SoBumpMapElement::numcomponents

  FIXME: write doc.
*/

/*!
  \fn SoBumpMapElement::bytes

  FIXME: write doc.
*/


SO_ELEMENT_SOURCE(SoBumpMapElement);

/*!
  This static method initializes static data for the
  SoBumpMapElement class.
*/

void
SoBumpMapElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoBumpMapElement, inherited);
}

/*!
  The destructor.
*/

SoBumpMapElement::~SoBumpMapElement()
{
}

//! FIXME: write doc.

void
SoBumpMapElement::init(SoState * state)
{
  inherited::init(state);
  this->image = NULL;
}

//! FIXME: write doc.

void
SoBumpMapElement::set(SoState * state, SoNode * node,
                      SoGLImage * image)
{
  SoBumpMapElement * elem = (SoBumpMapElement *)
    SoReplacedElement::getElement(state, classStackIndex, node);
  elem->setElt(image);
}

//! FIXME: write doc.

SoGLImage *
SoBumpMapElement::get(SoState * const state)
{
  SoBumpMapElement * elem = (SoBumpMapElement *)
    state->getConstElement(classStackIndex);

  return elem->image;
}

//! FIXME: write doc.

void
SoBumpMapElement::setElt(SoGLImage * imageptr)
{
  this->image = imageptr;
}
