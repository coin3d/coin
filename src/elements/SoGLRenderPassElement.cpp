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
  \class SoGLRenderPassElement Inventor/elements/SoGLRenderPassElement.h
  \brief The SoGLRenderPassElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLRenderPassElement.h>

/*!
  \fn SoGLRenderPassElement::passnr

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoGLRenderPassElement);

/*!
  This static method initializes static data for the
  SoGLRenderPassElement class.
*/

void
SoGLRenderPassElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLRenderPassElement, inherited);
}

/*!
  The destructor.
*/

SoGLRenderPassElement::~SoGLRenderPassElement(void)
{
}

//! FIXME: write doc.

void
SoGLRenderPassElement::init(SoState * state)
{
  inherited::init(state);
  this->passnr = 0;
}

//! FIXME: write doc.

SbBool
SoGLRenderPassElement::matches(const SoElement * element) const
{
  return this->passnr == ((SoGLRenderPassElement*)element)->passnr;
}

//! FIXME: write doc.

SoElement *
SoGLRenderPassElement::copyMatchInfo(void) const
{
  SoGLRenderPassElement * elem = (SoGLRenderPassElement*) this->getTypeId().createInstance();
  elem->passnr = this->passnr;
  return elem;
}

//! FIXME: write doc.

void
SoGLRenderPassElement::set(SoState * const state,
                           const int pass)
{
  SoGLRenderPassElement * element = (SoGLRenderPassElement *)
    getElement(state, classStackIndex);
  if (element) {
    element->passnr = pass;
  }
}

//! FIXME: write doc.

int
SoGLRenderPassElement::get(SoState * const state)
{
  const SoGLRenderPassElement * element = (const SoGLRenderPassElement *)
    getConstElement(state, classStackIndex);
  return element->passnr;
}
