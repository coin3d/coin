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
  \class SoDiffuseColorElement Inventor/elements/SoDiffuseColorElement.h
  \brief The SoDiffuseColorElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoDiffuseColorElement.h>

#include <Inventor/SbColor.h>
#include <Inventor/SbName.h>

#include <assert.h>

const static SbColor defaultColor(0.8, 0.8, 0.8);

/*!
  \fn SoDiffuseColorElement::numColors

  FIXME: write doc.
*/

/*!
  \fn SoDiffuseColorElement::colors

  FIXME: write doc.
*/

/*!
  \fn SoDiffuseColorElement::packedColors

  FIXME: write doc.
*/

//$ BEGIN TEMPLATE ElementSource( SoDiffuseColorElement )

/*!
  \var SoDiffuseColorElement::classTypeId

  This is the static class type identifier for the
  SoDiffuseColorElement class.
*/

SoType SoDiffuseColorElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoDiffuseColorElement::getClassTypeId(void)
{
  return SoDiffuseColorElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoDiffuseColorElement class.
*/
void *
SoDiffuseColorElement::createInstance(void)
{
  return (void *) new SoDiffuseColorElement;
}

/*!
  \var SoDiffuseColorElement::classStackIndex

  This is the static state stack index for the
  SoDiffuseColorElement class.
*/
int SoDiffuseColorElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoDiffuseColorElement class.
*/
int
SoDiffuseColorElement::getClassStackIndex(void)
{
  return SoDiffuseColorElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoDiffuseColorElement class.
*/

void
SoDiffuseColorElement::initClass()
{
//$ BEGIN TEMPLATE InitElementSource( SoDiffuseColorElement )
  assert(SoDiffuseColorElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoDiffuseColorElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoDiffuseColorElement",
    &SoDiffuseColorElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoDiffuseColorElement::classStackIndex =
      createStackIndex( SoDiffuseColorElement::classTypeId );
  } else {
    SoDiffuseColorElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
} // initClass()

/*!
  This static method cleans up static data for the
  SoDiffuseColorElement class.
*/

void 
SoDiffuseColorElement::cleanClass()
{
//$ BEGIN TEMPLATE CleanElementSource( SoDiffuseColorElement )
//$ END TEMPLATE CleanElementSource
} // cleanClass()

//! FIXME: write doc.

void 
SoDiffuseColorElement::init(SoState * state)
{
  this->colors = &defaultColor;
  this->packedColors = NULL;
  this->numColors = 1;
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoDiffuseColorElement::createInstance( void )
*/

SoDiffuseColorElement::SoDiffuseColorElement()
{
  setTypeId(SoDiffuseColorElement::classTypeId);
  setStackIndex(SoDiffuseColorElement::classStackIndex);
}

/*!
  The destructor.
*/

SoDiffuseColorElement::~SoDiffuseColorElement()
{
}

//! FIXME: write doc.

void 
SoDiffuseColorElement::set(SoState * const state, SoNode * const node,
			   const int32_t numColors, 
			   const SbColor * const colors)  
{
  SoDiffuseColorElement *elem = (SoDiffuseColorElement*)
    SoReplacedElement::getElement(state, classStackIndex, node);
  elem->setElt(numColors, colors);
}

//! FIXME: write doc.

void 
SoDiffuseColorElement::set(SoState * const state, SoNode * const node,
			   const int32_t numColors, 
			   const uint32_t * const colors)
{
  SoDiffuseColorElement *elem = (SoDiffuseColorElement*)
    SoReplacedElement::getElement(state, classStackIndex, node);
  elem->setElt(numColors, colors);
}

//! FIXME: write doc.

void 
SoDiffuseColorElement::setElt(const int32_t numColors, 
			      const SbColor * const colors)
{
  this->packedColors = NULL;
  this->colors = colors;
  this->numColors = numColors;
}

//! FIXME: write doc.

void
SoDiffuseColorElement::setElt(const int32_t numColors,
			      const uint32_t * const packedcolors)
{
  this->packedColors = packedcolors;
  this->numColors = numColors;
  this->colors = NULL;
}

//! FIXME: write doc.

//$ EXPORT INLINE
SbBool 
SoDiffuseColorElement::isPacked() const
{
  return this->packedColors != NULL;
}

//! FIXME: write doc.

//$ EXPORT INLINE
int32_t 
SoDiffuseColorElement::getNum() const
{
  return this->numColors;
}

//! FIXME: write doc.

const SbColor &
SoDiffuseColorElement::get(const int index) const
{
  assert(index >= 0 && index <= this->numColors);
  return this->colors[index];
}

//! FIXME: write doc.

//$ EXPORT INLINE
const SoDiffuseColorElement * 
SoDiffuseColorElement::getInstance(SoState *state)
{
  return (const SoDiffuseColorElement *)
    SoElement::getConstElement(state, classStackIndex);
}

