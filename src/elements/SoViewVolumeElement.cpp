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
  \class SoViewVolumeElement Inventor/elements/SoViewVolumeElement.h
  \brief The SoViewVolumeElement class is yet to be documented.

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
  element->viewVolume = viewVolume;
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
