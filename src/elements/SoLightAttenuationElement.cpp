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
  \class SoLightAttenuationElement Inventor/elements/SoLightAttenuationElement.h
  \brief The SoLightAttenuationElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoLightAttenuationElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

/*!
  \fn SoLightAttenuationElement::defaultAttenuation

  FIXME: write doc.
*/

const SbVec3f SoLightAttenuationElement::defaultAttenuation(0,0,1);

/*!
  \fn SoLightAttenuationElement::lightAttenuation

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoLightAttenuationElement);

/*!
  This static method initializes static data for the
  SoLightAttenuationElement class.
*/

void
SoLightAttenuationElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoLightAttenuationElement, inherited);
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoLightAttenationElement::createInstance(void)
*/

SoLightAttenuationElement::SoLightAttenuationElement(void)
  : lightAttenuation(defaultAttenuation)
{
  setTypeId(SoLightAttenuationElement::classTypeId);
  setStackIndex(SoLightAttenuationElement::classStackIndex);
}

/*!
  The destructor.
*/

SoLightAttenuationElement::~SoLightAttenuationElement(void)
{
}

//! FIXME: write doc.

void
SoLightAttenuationElement::set(SoState * const state,
                               SoNode * const node,
                               const SbVec3f & lightAttenuation)
{
  SoLightAttenuationElement * element = (SoLightAttenuationElement *)
    SoReplacedElement::getElement(state, classStackIndex, node);
  element->lightAttenuation = lightAttenuation;
}

//! FIXME: write doc.

const SbVec3f &
SoLightAttenuationElement::get(SoState * const state)
{
  SoLightAttenuationElement * element = (SoLightAttenuationElement *)
    SoElement::getConstElement(state, classStackIndex);
  return element->lightAttenuation;
}

//! FIXME: write doc.

SbBool
SoLightAttenuationElement::matches(SoElement * element) const
{
  if (this->lightAttenuation !=
      ((SoLightAttenuationElement *)element)->lightAttenuation)
    return FALSE;
  return TRUE;
}

//! FIXME: write doc.

SoElement *
SoLightAttenuationElement::copyMatchInfo() const
{
  SoLightAttenuationElement * element = (SoLightAttenuationElement *)
    (SoLightAttenuationElement::getClassTypeId().createInstance());
  element->lightAttenuation = this->lightAttenuation;
  return element;
}

//! FIXME: write doc.

void
SoLightAttenuationElement::print(FILE * file) const
{
  fprintf(file, "SoLightAttenuationElement[%p]: attenuation = ", this);
  fprintf(file, "<%f, %f, %f>\n",
           this->lightAttenuation[0],
           this->lightAttenuation[1],
           this->lightAttenuation[2]);
}

//! FIXME: write doc.

void
SoLightAttenuationElement::init(SoState * state)
{
  inherited::init(state);
  this->lightAttenuation = defaultAttenuation;
}

//! FIXME: write doc.

//$ EXPORT INLINE
const SbVec3f &
SoLightAttenuationElement::getDefault()
{
  return SoLightAttenuationElement::defaultAttenuation;
}
