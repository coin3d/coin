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
  \class SoTransparencyElement Inventor/elements/SoTransparencyElement.h
  \brief The SoTransparencyElement is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoTransparencyElement.h>

#include <Inventor/SbColor.h>
#include <Inventor/SbName.h>
#include <assert.h>

const static float defaultValue = 0;

/*!
  \fn SoTransparencyElement::numValues

  FIXME: write doc.
*/

/*!
  \fn SoTransparencyElement::values

  FIXME: write doc.
*/

//$ BEGIN TEMPLATE ElementSource( SoTransparencyElement )

/*!
  \var SoTransparencyElement::classTypeId

  This is the static class type identifier for the
  SoTransparencyElement class.
*/

SoType SoTransparencyElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoTransparencyElement::getClassTypeId(void)
{
  return SoTransparencyElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoTransparencyElement class.
*/
void *
SoTransparencyElement::createInstance(void)
{
  return (void *) new SoTransparencyElement;
}

/*!
  \var SoTransparencyElement::classStackIndex

  This is the static state stack index for the
  SoTransparencyElement class.
*/
int SoTransparencyElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoTransparencyElement class.
*/
int
SoTransparencyElement::getClassStackIndex(void)
{
  return SoTransparencyElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoTransparencyElement class.
*/

void
SoTransparencyElement::initClass()
{
//$ BEGIN TEMPLATE InitElementSource( SoTransparencyElement )
  assert(SoTransparencyElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoTransparencyElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoTransparencyElement",
    &SoTransparencyElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoTransparencyElement::classStackIndex =
      createStackIndex( SoTransparencyElement::classTypeId );
  } else {
    SoTransparencyElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the
  SoTransparencyElement class.
*/

void 
SoTransparencyElement::cleanClass()
{
//$ BEGIN TEMPLATE CleanElementSource( SoTransparencyElement )
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoTransparencyElement::createInstance( void )
*/

SoTransparencyElement::SoTransparencyElement()
{
  setTypeId(classTypeId);
  setStackIndex(classStackIndex);
}

/*!
  The destructor.
*/

SoTransparencyElement::~SoTransparencyElement()
{
}

//! FIXME: write doc.

void 
SoTransparencyElement::init(SoState * state)
{
  this->values = &defaultValue;
  this->numValues = 1;
}

//! FIXME: write doc.

void 
SoTransparencyElement::set(SoState * const state, SoNode * const node,
			   const int32_t numValues, 
			   const float * const values)  
{
  SoTransparencyElement *elem = (SoTransparencyElement*)
    SoReplacedElement::getElement(state, classStackIndex, node);
  elem->values = values;
  elem->numValues = numValues;
}
 
//! FIXME: write doc.

//$ EXPORT INLINE
int32_t 
SoTransparencyElement::getNum() const
{
  return this->numValues;
}

//! FIXME: write doc.

float
SoTransparencyElement::get(const int index) const
{
  assert(index >= 0 && index <= this->numValues);
  return this->values[index];
}

//! FIXME: write doc.

//$ EXPORT INLINE
const SoTransparencyElement * 
SoTransparencyElement::getInstance(SoState *state)
{
  return (const SoTransparencyElement *)
    SoElement::getConstElement(state, classStackIndex);
}

