/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoLightAttenuationElement Inventor/elements/SoLightAttenuationElement.h
  \brief The SoLightAttenuationElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoLightAttenuationElement.h>

#include <Inventor/C/tidbits.h>
#include <assert.h>
#include <stdlib.h>

// Dynamically allocated to avoid problems on systems which doesn't
// handle static constructors.
static SbVec3f * defaultattenuation = NULL;

static void
SoLightAttenuationElement_cleanup_func(void)
{
  delete defaultattenuation;
}

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
  defaultattenuation = new SbVec3f;
  defaultattenuation->setValue(0.0f, 0.0f, 1.0f);
  coin_atexit((coin_atexit_f *)SoLightAttenuationElement_cleanup_func);
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
  if (element) {
    element->lightAttenuation = lightAttenuation;
  }
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
SoLightAttenuationElement::matches(const SoElement * element) const
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
  this->lightAttenuation = SoLightAttenuationElement::getDefault();
}

//! FIXME: write doc.

const SbVec3f &
SoLightAttenuationElement::getDefault(void)
{
  return *defaultattenuation;
}
