/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoAnnoText3RenderPrintElement Inventor/elements/SoAnnoText3RenderPrintElement.h
  \brief The SoAnnoText3RenderPrintElement class is yet to be documented.
  \ingroup elements

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
