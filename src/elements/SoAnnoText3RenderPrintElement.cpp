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
  \class SoAnnoText3RenderPrintElement Inventor/elements/SoAnnoText3RenderPrintElement.h
  \brief The SoAnnoText3RenderPrintElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoAnnoText3RenderPrintElement.h>


#include <assert.h>

/*!
  \fn SoAnnoText3RenderPrintElement::RenderPrintType

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoAnnoText3RenderPrintElement);

/*!
  This static method initializes static data for the
  SoAnnoText3RenderPrintElement class.
*/

void
SoAnnoText3RenderPrintElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoAnnoText3RenderPrintElement, inherited);
}

/*!
  The destructor.
*/

SoAnnoText3RenderPrintElement::~SoAnnoText3RenderPrintElement(// virtual protected
    void)
{
}

//! FIXME: write doc.

void
SoAnnoText3RenderPrintElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoAnnoText3RenderPrintElement::set(SoState * const state, SoNode * const node,
                                const RenderPrintType type)
{
  SoInt32Element::set(classStackIndex,state,node,type);
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoAnnoText3RenderPrintElement::set(SoState * const state,
                                   const RenderPrintType type)
{
  set(state, NULL, type);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoAnnoText3RenderPrintElement::RenderPrintType
SoAnnoText3RenderPrintElement::get(SoState * const state)
{
  return (SoAnnoText3RenderPrintElement::RenderPrintType)
    SoInt32Element::get(classStackIndex, state);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoAnnoText3RenderPrintElement::RenderPrintType
SoAnnoText3RenderPrintElement::getDefault()
{
  return RENDER3D_PRINT_VECTOR;
}
