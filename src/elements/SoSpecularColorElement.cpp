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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

/*!
  \class SoSpecularColorElement Inventor/elements/SoSpecularColorElement.h
  \brief The SoSpecularColorElement is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoSpecularColorElement.h>

#include <Inventor/SbColor.h>
#include <Inventor/SbName.h>

#include <assert.h>

const static SbColor defaultColor(0,0,0);

/*!
  \fn SoSpecularColorElement::numColors

  FIXME: write doc.
*/

/*!
  \fn SoSpecularColorElement::colors

  FIXME: write doc.
*/

//$ BEGIN TEMPLATE ElementSource( SoSpecularColorElement )

/*!
  \var SoSpecularColorElement::classTypeId

  This is the static class type identifier for the
  SoSpecularColorElement class.
*/

SoType SoSpecularColorElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoSpecularColorElement::getClassTypeId(void)
{
  return SoSpecularColorElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoSpecularColorElement class.
*/
void *
SoSpecularColorElement::createInstance(void)
{
  return (void *) new SoSpecularColorElement;
}

/*!
  \var SoSpecularColorElement::classStackIndex

  This is the static state stack index for the
  SoSpecularColorElement class.
*/
int SoSpecularColorElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoSpecularColorElement class.
*/
int
SoSpecularColorElement::getClassStackIndex(void)
{
  return SoSpecularColorElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the SoSpecularColorElement
  class.
*/

void
SoSpecularColorElement::initClass()
{
//$ BEGIN TEMPLATE InitElementSource( SoSpecularColorElement )
  assert(SoSpecularColorElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSpecularColorElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoSpecularColorElement",
    &SoSpecularColorElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoSpecularColorElement::classStackIndex =
      createStackIndex( SoSpecularColorElement::classTypeId );
  } else {
    SoSpecularColorElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
} // initClass()

/*!
  This static method cleans up static data for the SoSpecularColorElement
  class.
*/

void 
SoSpecularColorElement::cleanClass()
{
//$ BEGIN TEMPLATE CleanElementSource( SoSpecularColorElement )
//$ END TEMPLATE CleanElementSource
} // cleanClass()

/*!
  A constructor.  Can't be used directly.

  \sa void * SoSpecularColorElement::createInstance( void )
*/

SoSpecularColorElement::SoSpecularColorElement()
{
  setTypeId(classTypeId);
  setStackIndex(classStackIndex);
}

/*!
  The destructor.
*/

SoSpecularColorElement::~SoSpecularColorElement()
{
}

//! FIXME: write doc.

void 
SoSpecularColorElement::init(SoState * state)
{
  this->colors = &defaultColor;
  this->numColors = 1;
}

//! FIXME: write doc.

void 
SoSpecularColorElement::set(SoState * const state, SoNode * const node,
			   const int32_t numColors, 
			   const SbColor * const colors)  
{
  SoSpecularColorElement *elem = (SoSpecularColorElement*)
    SoReplacedElement::getElement(state, classStackIndex, node);
  elem->setElt(numColors, colors);
}

//! FIXME: write doc.

void 
SoSpecularColorElement::setElt(const int32_t numColors, 
			      const SbColor * const colors)
{
  this->colors = colors;
  this->numColors = numColors;
}
 
//! FIXME: write doc.

//$ EXPORT INLINE
int32_t 
SoSpecularColorElement::getNum() const
{
  return this->numColors;
}

//! FIXME: write doc.

const SbColor &
SoSpecularColorElement::get(const int index) const
{
  assert(index >= 0 && index <= this->numColors);
  return this->colors[index];
}

//! FIXME: write doc.

//$ EXPORT INLINE
const SoSpecularColorElement * 
SoSpecularColorElement::getInstance(SoState *state)
{
  return (const SoSpecularColorElement *)
    SoElement::getConstElement(state, classStackIndex);
}

