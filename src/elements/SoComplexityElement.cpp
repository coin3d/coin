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
  \class SoComplexityElement Inventor/elements/SoComplexityElement.h
  \brief The SoComplexityElement class contains the current shape complexity
  for a graph traverser.
*/

#include <Inventor/elements/SoComplexityElement.h>

#include <Inventor/SbName.h>

#include <assert.h>


SO_ELEMENT_SOURCE(SoComplexityElement);


/*!
  This static method initializes static data for the SoComplexityElement
  class.
*/

void
SoComplexityElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoComplexityElement, inherited);
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoComplexityElement::createInstance(void)
*/

SoComplexityElement::SoComplexityElement(void)
{
    setTypeId(SoComplexityElement::classTypeId);
    setStackIndex(SoComplexityElement::classStackIndex);
    this->data = getDefault();
}

/*!
  The destructor.
*/

SoComplexityElement::~SoComplexityElement(// virtual protected
    void)
{
}

//! FIXME: write doc.

void
SoComplexityElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoComplexityElement::set(SoState * const state, SoNode * const node,
                         const float complexity)
{
  SoFloatElement::set(classStackIndex, state, node, complexity);
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoComplexityElement::set(SoState * const state, const float complexity)
{
  set(state, NULL, complexity);
}

//! FIXME: write doc.

//$ EXPORT INLINE
float
SoComplexityElement::get(SoState * const state)
{
  return SoFloatElement::get(classStackIndex, state);
}

//! FIXME: write doc.

//$ EXPORT INLINE
float
SoComplexityElement::getDefault()
{
  return 0.5f;
}
