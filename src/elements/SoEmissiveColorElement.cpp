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
  \class SoEmissiveColorElement Inventor/elements/SoEmissiveColorElement.h
  \brief The SoEmissiveColorElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoEmissiveColorElement.h>

#include <Inventor/SbColor.h>
#include <Inventor/SbName.h>

#include <assert.h>

static const SbColor defaultColor(0.0f, 0.0f, 0.0f);

/*!
  \fn SoEmissiveColorElement::numColors

  FIXME: write doc.
*/

/*!
  \fn SoEmissiveColorElement::colors

  FIXME: write doc.
*/

//$ BEGIN TEMPLATE ElementSource(SoEmissiveColorElement)

/*!
  \var SoEmissiveColorElement::classTypeId

  This is the static class type identifier for the
  SoEmissiveColorElement class.
*/

SoType SoEmissiveColorElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoEmissiveColorElement::getClassTypeId(void)
{
  return SoEmissiveColorElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoEmissiveColorElement class.
*/
void *
SoEmissiveColorElement::createInstance(void)
{
  return (void *) new SoEmissiveColorElement;
}

/*!
  \var SoEmissiveColorElement::classStackIndex

  This is the static state stack index for the
  SoEmissiveColorElement class.
*/
int SoEmissiveColorElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoEmissiveColorElement class.
*/
int
SoEmissiveColorElement::getClassStackIndex(void)
{
  return SoEmissiveColorElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the SoEmissiveColorElement
  class.
*/

void
SoEmissiveColorElement::initClass()
{
//$ BEGIN TEMPLATE InitElementSource(SoEmissiveColorElement)
  assert(SoEmissiveColorElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoEmissiveColorElement::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SoEmissiveColorElement",
                       &SoEmissiveColorElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoEmissiveColorElement::classStackIndex =
      createStackIndex(SoEmissiveColorElement::classTypeId);
  }
  else {
    SoEmissiveColorElement::classStackIndex = inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the SoEmissiveColorElement
  class.
*/

void 
SoEmissiveColorElement::cleanClass()
{
//$ BEGIN TEMPLATE CleanElementSource(SoEmissiveColorElement)
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoEmissiveColorElement::createInstance(void)
*/

SoEmissiveColorElement::SoEmissiveColorElement()
{
  setTypeId(classTypeId);
  setStackIndex(classStackIndex);
}

/*!
  The destructor.
*/

SoEmissiveColorElement::~SoEmissiveColorElement()
{
}

//! FIXME: write doc.

void 
SoEmissiveColorElement::init(SoState * /* state */)
{
  this->colors = &defaultColor;
  this->numColors = 1;
}

//! FIXME: write doc.

void 
SoEmissiveColorElement::set(SoState * const state, SoNode * const node,
			   const int32_t numColors, 
			   const SbColor * const colors)  
{
  SoEmissiveColorElement *elem = (SoEmissiveColorElement*)
    SoReplacedElement::getElement(state, classStackIndex, node);
  elem->setElt(numColors, colors);
}

//! FIXME: write doc.

void 
SoEmissiveColorElement::setElt(const int32_t numColors, 
			      const SbColor * const colors)
{
  this->colors = colors;
  this->numColors = numColors;
}

//! FIXME: write doc.
 
//$ EXPORT INLINE
int32_t 
SoEmissiveColorElement::getNum() const
{
  return this->numColors;
}

//! FIXME: write doc.

const SbColor &
SoEmissiveColorElement::get(const int index) const
{
  assert(index >= 0 && index <= this->numColors);
  return this->colors[index];
}

//! FIXME: write doc.

//$ EXPORT INLINE
const SoEmissiveColorElement * 
SoEmissiveColorElement::getInstance(SoState *state)
{
  return (const SoEmissiveColorElement *)
    SoElement::getConstElement(state, classStackIndex);
}

