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
  \class SoFloatElement Inventor/elements/SoFloatElement.h
  \brief Abstract base class for elements that only take care of a float
  value.
*/

#include <Inventor/elements/SoFloatElement.h>

#include <assert.h>

/*!
  \var SoFloatElement::data

  FIXME: write doc.
*/

SO_ELEMENT_ABSTRACT_SOURCE(SoFloatElement);

/*!
  This static method initializes static data for the SoFloatElement class.
*/

void
SoFloatElement::initClass(void)
{
  SO_ELEMENT_INIT_ABSTRACT_CLASS(SoFloatElement, inherited);
}

/*!
  The constructor initialized the float to 0.0, and sets the class type
  identifier and stack index.
*/

SoFloatElement::SoFloatElement(void)
  : data(0.0f)
{
    setTypeId(SoFloatElement::classTypeId);
    setStackIndex(SoFloatElement::classStackIndex);
}

/*!
  The destructor.
*/

SoFloatElement::~SoFloatElement(void)
{
}

//! FIXME: write doc.

SbBool
SoFloatElement::matches(const SoElement * element) const
{
    assert(element);
    if (getTypeId() != element->getTypeId())
        return FALSE;
    if (this->data != ((SoFloatElement *)element)->data)
        return FALSE;
    return TRUE;
}

//! FIXME: write doc.

SoElement *
SoFloatElement::copyMatchInfo(void) const
{
//    inherited::copyMatchInfo();
    assert(getTypeId().canCreateInstance());
    SoFloatElement * element = (SoFloatElement *) getTypeId().createInstance();
    element->data = this->data;
    // DEPRECATED 980807 pederb. copyMatchInfo() should only copy
    // information needed in matches(). An exact copy is not needed.
    //    element->dataNode = this->dataNode;
    return element;
}

//! FIXME: write doc.

void
SoFloatElement::print(FILE * file) const
{
    fprintf(file, "%s[%p]: data = %f\n",
        getTypeId().getName().getString(), this, this->data);
}

//! FIXME: write doc.

void
SoFloatElement::set(const int index,
                    SoState * const state,
                    SoNode * const /* node */,
                    const float value)
{
  SoFloatElement * element = (SoFloatElement *)
    (getElement(state, index));

  if (element) {
    element->setElt(value);
  }
}

/*!
  FIXME: write doc.
*/

void
SoFloatElement::set(const int stackIndex, SoState * const state,
                    const float value)
{
  set(stackIndex, state, NULL, value);
}

//! FIXME: write doc.

float
SoFloatElement::get(const int index,
                    SoState * const state)
{
  SoFloatElement * element = (SoFloatElement *)
    (getConstElement(state, index)); //, NULL );
  if (element)
    return element->data;
  return 0.0f;
}

//! FIXME: write doc.

void
SoFloatElement::setElt(float value)
{
  this->data = value;
}

//! FIXME: write doc.

void
SoFloatElement::init(SoState * state)
{
  inherited::init(state);
  this->data = 0.0f;
}
