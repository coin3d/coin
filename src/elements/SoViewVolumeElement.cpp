/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoViewVolumeElement Inventor/elements/SoViewVolumeElement.h
  \brief The SoViewVolumeElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoViewVolumeElement.h>


#include <assert.h>

/*!
  \fn SoViewVolumeElement::viewVolume

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoViewVolumeElement);

/*!
  This static method initializes static data for the SoViewVolumeElement class.
*/

void
SoViewVolumeElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoViewVolumeElement, inherited);
}

/*!
  The destructor.
*/

SoViewVolumeElement::~SoViewVolumeElement(void)
{
}

//! FIXME: write doc.

void
SoViewVolumeElement::set(SoState * const state,
                         SoNode * const node,
                         const SbViewVolume & viewVolume)
{
  SoViewVolumeElement * element = (SoViewVolumeElement *)
    SoReplacedElement::getElement(state, classStackIndex, node);
  if (element) {
    element->viewVolume = viewVolume;
  }
}

//! FIXME: write doc.

const SbViewVolume &
SoViewVolumeElement::get(SoState * const state)
{
  SoViewVolumeElement * element = (SoViewVolumeElement *)
    SoElement::getConstElement(state, classStackIndex);
  return element->viewVolume;
}

//! FIXME: write doc.

void
SoViewVolumeElement::init(SoState * state)
{
  inherited::init(state);
  this->viewVolume.ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
}
