/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoCreaseAngleElement Inventor/elements/SoCreaseAngleElement.h
  \brief The SoCreaseAngleElement class stores the crease angle during a
  scene graph traversal.
*/

#include <Inventor/elements/SoCreaseAngleElement.h>


#include <assert.h>

SO_ELEMENT_SOURCE(SoCreaseAngleElement);

/*!
  This static method initializes static data for the
  SoCreaseAngleElement class.
*/

void
SoCreaseAngleElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoCreaseAngleElement, inherited);
}

/*!
  The destructor.
*/

SoCreaseAngleElement::~SoCreaseAngleElement(// virtual protected
    void)
{
}

//! FIXME: write doc.

void
SoCreaseAngleElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoCreaseAngleElement::set(SoState * const state, SoNode * const node,
                          const float complexity)
{
  SoFloatElement::set(classStackIndex,state,node, complexity);
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoCreaseAngleElement::set(SoState * const state, const float complexity)
{
  set(state, NULL, complexity);
}

//! FIXME: write doc.

//$ EXPORT INLINE
float
SoCreaseAngleElement::get(SoState * const state)
{
  return SoFloatElement::get(classStackIndex, state);
}

//! FIXME: write doc.

//$ EXPORT INLINE
float
SoCreaseAngleElement::getDefault()
{
  return 0.0f;
}
