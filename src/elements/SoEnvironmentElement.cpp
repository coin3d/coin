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
  \class SoEnvironmentElement Inventor/elements/SoEnvironmentElement.h
  \brief The SoEnvironmentElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoEnvironmentElement.h>


#include <assert.h>

/*!
  \fn SoEnvironmentElement::FogType

  FIXME: write doc.
*/

/*!
  \fn SoEnvironmentElement::ambientIntensity

  FIXME: write doc.
*/

/*!
  \fn SoEnvironmentElement::ambientColor

  FIXME: write doc.
*/

/*!
  \fn SoEnvironmentElement::attenuation

  FIXME: write doc.
*/

/*!
  \fn SoEnvironmentElement::fogType

  FIXME: write doc.
*/

/*!
  \fn SoEnvironmentElement::fogColor

  FIXME: write doc.
*/

/*!
  \fn SoEnvironmentElement::fogVisibility

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoEnvironmentElement);

/*!
  This static method initializes static data for the
  SoEnvironmentElement class.
*/

void
SoEnvironmentElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoEnvironmentElement, inherited);
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoEnvironmentElement::createInstance(void)
*/

SoEnvironmentElement::SoEnvironmentElement(void)
{
  setTypeId(SoEnvironmentElement::classTypeId);
  setStackIndex(SoEnvironmentElement::classStackIndex);
}

/*!
  The destructor.
*/

SoEnvironmentElement::~SoEnvironmentElement(void)
{
}

//! FIXME: write doc.

void
SoEnvironmentElement::set(SoState * const state,
                          SoNode * const node,
                          const float ambientIntensity,
                          const SbColor & ambientColor,
                          const SbVec3f & attenuation,
                          const int32_t fogType,
                          const SbColor & fogColor,
                          const float fogVisibility)
{
  SoEnvironmentElement * element = (SoEnvironmentElement *)
    SoReplacedElement::getElement(state, classStackIndex, node);
  element->setElt(state, ambientIntensity, ambientColor, attenuation,
                  fogType, fogColor, fogVisibility);
}

//! FIXME: write doc.

void
SoEnvironmentElement::get(SoState * const state,
                          float & ambientIntensity,
                          SbColor & ambientColor,
                          SbVec3f & attenuation,
                          int32_t & fogType,
                          SbColor & fogColor,
                          float & fogVisibility)
{
  SoEnvironmentElement *element = (SoEnvironmentElement *)
    SoElement::getConstElement(state, classStackIndex);
  ambientIntensity = element->ambientIntensity;
  ambientColor = element->ambientColor;
  attenuation = element->attenuation;
  fogType = element->fogType;
  fogColor = element->fogColor;
  fogVisibility = element->fogVisibility;
}

//! FIXME: write doc.

void
SoEnvironmentElement::getDefault(float & ambientIntensity,
                                 SbColor & ambientColor,
                                 SbVec3f & attenuation,
                                 int32_t & fogType,
                                 SbColor & fogColor,
                                 float & fogVisibility)
{
  ambientIntensity = 0.2f;
  ambientColor = SbColor(1.0f, 1.0f, 1.0f);
  attenuation = SbVec3f(0.0f, 0.0f, 1.0f);
  fogType = NONE;
  fogColor = SbColor(1.0f, 1.0f, 1.0f);
  fogVisibility = 0.0f;
}

//! FIXME: write doc.

float
SoEnvironmentElement::getAmbientIntensity(SoState * const state)
{
  SoEnvironmentElement * element = (SoEnvironmentElement *)
    SoElement::getConstElement(state, classStackIndex);
  return element->ambientIntensity;
}

//! FIXME: write doc.

float
SoEnvironmentElement::getFogVisibility(SoState * const state)
{
  SoEnvironmentElement * element = (SoEnvironmentElement *)
    SoElement::getConstElement(state, classStackIndex);
  return element->fogVisibility;
}

//! FIXME: write doc.

const SbVec3f &
SoEnvironmentElement::getLightAttenuation(SoState * const state)
{
  SoEnvironmentElement * element = (SoEnvironmentElement *)
    SoElement::getConstElement(state, classStackIndex);
  return element->attenuation;
}

//! FIXME: write doc.

const SbColor &
SoEnvironmentElement::getAmbientColor(SoState * const state)
{
  SoEnvironmentElement * element = (SoEnvironmentElement *)
    SoElement::getConstElement(state, classStackIndex);
  return element->ambientColor;
}

//! FIXME: write doc.

const SbColor &
SoEnvironmentElement::getFogColor(SoState * const state)
{
  SoEnvironmentElement * element = (SoEnvironmentElement *)
    SoElement::getConstElement(state, classStackIndex);
  return element->fogColor;
}

//! FIXME: write doc.

int32_t
SoEnvironmentElement::getFogType(SoState * const state)
{
  SoEnvironmentElement * element = (SoEnvironmentElement *)
    SoElement::getConstElement(state, classStackIndex);
  return element->fogType;
}

//! FIXME: write doc.

void
SoEnvironmentElement::print(FILE * file) const
{
  fprintf(file, "SoEnvironmentElement[%p]\n", this);
}

//! FIXME: write doc.

void
SoEnvironmentElement::init(SoState * state)
{
  inherited::init(state);
  this->getDefault(ambientIntensity, ambientColor, attenuation,
                   fogType, fogColor, fogVisibility);
}

//! FIXME: doc
void
SoEnvironmentElement::setElt(SoState * const,
                             const float ambientIntensity,
                             const SbColor & ambientColor,
                             const SbVec3f & attenuation,
                             const int32_t fogType,
                             const SbColor & fogColor,
                             const float fogVisibility)
{
  this->ambientIntensity = ambientIntensity;
  this->ambientColor = ambientColor;
  this->attenuation = attenuation;
  this->fogType = fogType;
  this->fogColor = fogColor;
  this->fogVisibility = fogVisibility;
}
