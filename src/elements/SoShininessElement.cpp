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
  \class SoShininessElement Inventor/elements/SoShininessElement.h
  \brief The SoShininessElement class is yet to be documented.

  FIXME: write doc.

  This element is part of the SoLazyElement in some other OI implementetions.
*/

#include <Inventor/elements/SoShininessElement.h>

#include <Inventor/SbColor.h>
#include <Inventor/SbName.h>

#include <assert.h>

const static float defaultValue = 0.2;

/*!
  \fn SoShininessElement::numValues

  FIXME: write doc.
*/

/*!
  \fn SoShininessElement::values

  FIXME: write doc.
*/

//$ BEGIN TEMPLATE ElementSource( SoShininessElement )

/*!
  \var SoShininessElement::classTypeId

  This is the static class type identifier for the
  SoShininessElement class.
*/

SoType SoShininessElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoShininessElement::getClassTypeId(void)
{
  return SoShininessElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoShininessElement class.
*/
void *
SoShininessElement::createInstance(void)
{
  return (void *) new SoShininessElement;
}

/*!
  \var SoShininessElement::classStackIndex

  This is the static state stack index for the
  SoShininessElement class.
*/
int SoShininessElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoShininessElement class.
*/
int
SoShininessElement::getClassStackIndex(void)
{
  return SoShininessElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the SoShininessElement class.
*/

void
SoShininessElement::initClass()
{
//$ BEGIN TEMPLATE InitElementSource( SoShininessElement )
  assert(SoShininessElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoShininessElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoShininessElement",
    &SoShininessElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoShininessElement::classStackIndex =
      createStackIndex( SoShininessElement::classTypeId );
  } else {
    SoShininessElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the SoShininessElement class.
*/

void 
SoShininessElement::cleanClass()
{
//$ BEGIN TEMPLATE CleanElementSource( SoShininessElement )
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoShininessElement::createInstance( void )
*/

SoShininessElement::SoShininessElement()
{
  setTypeId(classTypeId);
  setStackIndex(classStackIndex);
}

/*!
  The destructor.
*/

SoShininessElement::~SoShininessElement()
{
}

//! FIXME: write doc.

void 
SoShininessElement::init(SoState * state)
{
  this->values = &defaultValue;
  this->numValues = 1;
}

//! FIXME: write doc.

void 
SoShininessElement::set(SoState * const state, SoNode * const node,
			   const int32_t numValues, 
			   const float * const values)  
{
  SoShininessElement *elem = (SoShininessElement*)
    SoReplacedElement::getElement(state, classStackIndex, node);
  elem->setElt(numValues, values);
}

//! FIXME: write doc.

void 
SoShininessElement::setElt(const int32_t numValues,
			   const float * const values)
{
  this->values = values;
  this->numValues = numValues;
}
 
//! FIXME: write doc.

//$ EXPORT INLINE
int32_t 
SoShininessElement::getNum() const
{
  return this->numValues;
}

//! FIXME: write doc.

float
SoShininessElement::get(const int index) const
{
  assert(index >= 0 && index <= this->numValues);
  return this->values[index];
}

//! FIXME: write doc.

//$ EXPORT INLINE
const SoShininessElement * 
SoShininessElement::getInstance(SoState *state)
{
  return (const SoShininessElement *)
    SoElement::getConstElement(state, classStackIndex);
}

