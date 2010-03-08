/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoLightAttenuationElement Inventor/elements/SoLightAttenuationElement.h
  \brief The SoLightAttenuationElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include "SbBasicP.h"
#include "tidbitsp.h"

#include <Inventor/elements/SoLightAttenuationElement.h>

#include <cassert>
#include <cstdlib>

// Dynamically allocated to avoid problems on systems which doesn't
// handle static constructors.
static SbVec3f * defaultattenuation = NULL;

extern "C" {

static void
SoLightAttenuationElement_cleanup_func(void)
{
  delete defaultattenuation;
}

} // extern "C"

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
  coin_atexit(SoLightAttenuationElement_cleanup_func, CC_ATEXIT_NORMAL);
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
  SoLightAttenuationElement * element =
    coin_safe_cast<SoLightAttenuationElement *>
    (
     SoReplacedElement::getElement(state, classStackIndex, node)
     );
  if (element) {
    element->lightAttenuation = lightAttenuation;
  }
}

//! FIXME: write doc.

const SbVec3f &
SoLightAttenuationElement::get(SoState * const state)
{
  const SoLightAttenuationElement * element = coin_assert_cast<const SoLightAttenuationElement *>
    (
     SoElement::getConstElement(state, classStackIndex)
     );
  return element->lightAttenuation;
}

//! FIXME: write doc.

SbBool
SoLightAttenuationElement::matches(const SoElement * element) const
{
  if (this->lightAttenuation !=
      coin_assert_cast<const SoLightAttenuationElement *>(element)->lightAttenuation)
    return FALSE;
  return TRUE;
}

//! FIXME: write doc.

SoElement *
SoLightAttenuationElement::copyMatchInfo() const
{
  SoLightAttenuationElement * element = static_cast<SoLightAttenuationElement *>
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
