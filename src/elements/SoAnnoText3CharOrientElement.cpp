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
  \class SoAnnoText3CharOrientElement Inventor/elements/SoAnnoText3CharOrientElement.h
  \brief The SoAnnoText3CharOrientElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoAnnoText3CharOrientElement.h>


#include <assert.h>

SO_ELEMENT_SOURCE(SoAnnoText3CharOrientElement);

/*!
  This static method initializes static data for the
  SoAnnoText3CharOrientElement class.
*/

void
SoAnnoText3CharOrientElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoAnnoText3CharOrientElement, inherited);
}

/*!
  The destructor.
*/

SoAnnoText3CharOrientElement::~SoAnnoText3CharOrientElement(// virtual protected
    void)
{
}

//! FIXME: write doc.

void
SoAnnoText3CharOrientElement::init(SoState * state)
{
  inherited::init(state);
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoAnnoText3CharOrientElement::set(SoState * const state, SbBool isOriented)
{
  inherited::set(classStackIndex, state, isOriented);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SbBool
SoAnnoText3CharOrientElement::get(SoState * state)
{
  return (SbBool) SoInt32Element::get(classStackIndex, state);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SbBool
SoAnnoText3CharOrientElement::getDefault(void)
{
  return TRUE;
}
