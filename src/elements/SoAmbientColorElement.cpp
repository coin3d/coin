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
  \class SoAmbientColorElement Inventor/elements/SoAmbientColorElement.h
  \brief The SoAmbientColorElement class is yet to be documented.
*/

#include <Inventor/elements/SoAmbientColorElement.h>

#include <Inventor/SbColor.h>
#include <Inventor/SbName.h>

#include <assert.h>

static const SbColor defaultColor(0.2, 0.2, 0.2);

/*!
  \fn SoAmbientColorElement::numColors

  This is the number of ambient colors.
*/

/*!
  \fn SoAmbientColorElement::colors

  This is an array of ambient colors.
*/

//$ BEGIN TEMPLATE ElementSource(SoAmbientColorElement)

/*!
  \var SoAmbientColorElement::classTypeId

  This is the static class type identifier for the
  SoAmbientColorElement class.
*/

SoType SoAmbientColorElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoAmbientColorElement::getClassTypeId(void)
{
  return SoAmbientColorElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoAmbientColorElement class.
*/
void *
SoAmbientColorElement::createInstance(void)
{
  return (void *) new SoAmbientColorElement;
}

/*!
  \var SoAmbientColorElement::classStackIndex

  This is the static state stack index for the
  SoAmbientColorElement class.
*/
int SoAmbientColorElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoAmbientColorElement class.
*/
int
SoAmbientColorElement::getClassStackIndex(void)
{
  return SoAmbientColorElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoAmbientColorElement class.
*/

void
SoAmbientColorElement::initClass()
{
//$ BEGIN TEMPLATE InitElementSource(SoAmbientColorElement)
  assert(SoAmbientColorElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoAmbientColorElement::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SoAmbientColorElement",
                       &SoAmbientColorElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoAmbientColorElement::classStackIndex =
      createStackIndex(SoAmbientColorElement::classTypeId);
  }
  else {
    SoAmbientColorElement::classStackIndex = inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

//! FIXME: write doc.

void 
SoAmbientColorElement::init(SoState * /* state */)
{
  this->colors = &defaultColor;
  this->numColors = 1;
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoAmbientColorElement::createInstance()
*/

SoAmbientColorElement::SoAmbientColorElement()
{
  setTypeId(SoAmbientColorElement::classTypeId);
  setStackIndex(SoAmbientColorElement::classStackIndex);
}

/*!
  The destructor.
*/

SoAmbientColorElement::~SoAmbientColorElement()
{
}

//! FIXME: write doc.

void 
SoAmbientColorElement::set(SoState * const state, SoNode * const node,
			   const int32_t numColors, 
			   const SbColor * const colors)  
{
  SoAmbientColorElement *elem = (SoAmbientColorElement*)
    SoReplacedElement::getElement(state, classStackIndex, node);
  elem->setElt(numColors, colors);
}

//! FIXME: write doc.

void 
SoAmbientColorElement::setElt(const int32_t numColors, 
			      const SbColor * const colors)
{
  this->colors = colors;
  this->numColors = numColors;
}

//! FIXME: write doc.

//$ EXPORT INLINE
int32_t 
SoAmbientColorElement::getNum() const
{
  return this->numColors;
}

//! FIXME: write doc.

const SbColor &
SoAmbientColorElement::get(const int index) const
{
  assert(index >= 0 && index <= this->numColors);
  return this->colors[index];
}

//! FIXME: write doc.

//$ EXPORT INLINE
const SoAmbientColorElement * 
SoAmbientColorElement::getInstance(SoState *state)
{
  return (const SoAmbientColorElement *)
    SoElement::getConstElement(state, classStackIndex);
}

