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
  \class SoViewVolumeElement Inventor/elements/SoViewVolumeElement.h
  \brief The SoViewVolumeElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoViewVolumeElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

/*!
  \fn SoViewVolumeElement::viewVolume

  FIXME: write doc.
*/

//$ BEGIN TEMPLATE ElementSource(SoViewVolumeElement)

/*!
  \var SoViewVolumeElement::classTypeId

  This is the static class type identifier for the
  SoViewVolumeElement class.
*/

SoType SoViewVolumeElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoViewVolumeElement::getClassTypeId(void)
{
  return SoViewVolumeElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoViewVolumeElement class.
*/
void *
SoViewVolumeElement::createInstance(void)
{
  return (void *) new SoViewVolumeElement;
}

/*!
  \var SoViewVolumeElement::classStackIndex

  This is the static state stack index for the
  SoViewVolumeElement class.
*/
int SoViewVolumeElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoViewVolumeElement class.
*/
int
SoViewVolumeElement::getClassStackIndex(void)
{
  return SoViewVolumeElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the SoViewVolumeElement class.
*/

void
SoViewVolumeElement::initClass(void)
{
//$ BEGIN TEMPLATE InitElementSource(SoViewVolumeElement)
  assert(SoViewVolumeElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoViewVolumeElement::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SoViewVolumeElement",
                       &SoViewVolumeElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoViewVolumeElement::classStackIndex =
      createStackIndex(SoViewVolumeElement::classTypeId);
  }
  else {
    SoViewVolumeElement::classStackIndex = inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoViewVolumeElement::createInstance(void)
*/

SoViewVolumeElement::SoViewVolumeElement(void)
{
  setTypeId(SoViewVolumeElement::classTypeId);
  setStackIndex(SoViewVolumeElement::classStackIndex);

  SbViewVolume v;
  v.ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
  this->viewVolume = v;
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

  SbViewVolume v;
  v.ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
  this->viewVolume = v;
}
