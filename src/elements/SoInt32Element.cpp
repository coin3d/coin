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
  \class SoInt32Element Inventor/elements/SoInt32Element.h
  \brief The SoInt32Element class is the base class for all elements
  that only take care of a 32-bit integer.
*/

#include <Inventor/elements/SoInt32Element.h>

#include <Inventor/SbName.h>

#include <assert.h>

/*!
  \fn SoInt32Element::data

  FIXME: write doc.
*/

SO_ELEMENT_ABSTRACT_SOURCE(SoInt32Element);

/*!
  This static method initializes static data for the SoInt32Element class.
*/

void
SoInt32Element::initClass(void)
{
  SO_ELEMENT_INIT_ABSTRACT_CLASS(SoInt32Element, inherited);
}

/*!
  The constructor initialized the int32_t to 0, and sets the class type
  identifier and stack index of the element.
*/

SoInt32Element::SoInt32Element(void)
  : data(0)
{
  setTypeId(SoInt32Element::classTypeId);
  setStackIndex(SoInt32Element::classStackIndex);
}

/*!
  The destructor.
*/

SoInt32Element::~SoInt32Element(void)
{
}

//! FIXME: write doc.

SbBool
SoInt32Element::matches(const SoElement * element) const
{
    assert(element);
    if (getTypeId() != element->getTypeId())
        return FALSE;
    if (((const SoInt32Element *)element)->data != this->data)
        return FALSE;
    return TRUE;
}

//! FIXME: write doc.

SoElement *
SoInt32Element::copyMatchInfo(void) const
{
    assert(getTypeId().canCreateInstance());
    SoInt32Element * element =
        (SoInt32Element *)(getTypeId().createInstance());
    element->data = this->data;

    // DEPRECATED 980807 pederb. copyMatchInfo should only copy
    // information needed in matches(). An exact copy is not needed.
    //    element->dataNode = this->dataNode;
    return (SoElement *)element;
}

//! FIXME: write doc.

void
SoInt32Element::print(FILE * file) const
{
    fprintf(file, "%s[%p]: data = %d\n",
        getTypeId().getName().getString(), this, this->data);
}

//! FIXME: write doc.

void
SoInt32Element::set(const int index,
                    SoState * const state,
                    SoNode * const /* node */,
                    const int32_t value)
{
  SoInt32Element * element;
  element = (SoInt32Element *) getElement(state, index);
  if (element)
    element->setElt(value);
}

/*!
  FIXME: write doc.
*/

void
SoInt32Element::set(const int index, SoState * const state,
                    const int32_t value)
{
  set(index, state, NULL, value);
}

//! FIXME: write doc.

int32_t
SoInt32Element::get(const int index,
                    SoState * const state)
{
  SoInt32Element * element;
  element = (SoInt32Element *) getConstElement(state, index); //, NULL );
  if (element)
    return element->data;
  return 0;
}

//! FIXME: write doc.

void
SoInt32Element::setElt(int32_t value)
{
  this->data = value;
}

//! FIXME: write doc.

void
SoInt32Element::init(SoState * state)
{
  inherited::init(state);
  this->data = 0;
}
