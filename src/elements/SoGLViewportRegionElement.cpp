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
  \class SoGLViewportRegionElement Inventor/elements/SoGLViewportRegionElement.h
  \brief The SoGLViewportRegionElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLViewportRegionElement.h>


#include <assert.h>
#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

SO_ELEMENT_SOURCE(SoGLViewportRegionElement);

/*!
  This static method initializes static data for the
  SoGLViewportRegionElement class.
*/

void
SoGLViewportRegionElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLViewportRegionElement, inherited);
}

/*!
  The destructor.
*/

SoGLViewportRegionElement::~SoGLViewportRegionElement()
{
}

//! FIXME: write doc.

void
SoGLViewportRegionElement::init(SoState * state)
{
  inherited::init(state);
  this->initialized = FALSE;
}

//! FIXME: write doc.

void
SoGLViewportRegionElement::push(SoState * state)
{
  inherited::push(state);
  SoGLViewportRegionElement * prev = (SoGLViewportRegionElement*)
    this->getNextInStack();
  this->viewportRegion = prev->viewportRegion;
  this->initialized = TRUE;
}

//! FIXME: write doc.

void
SoGLViewportRegionElement::pop(SoState * state,
                               const SoElement * prevTopElement)
{
  SoGLViewportRegionElement * prev =
    (SoGLViewportRegionElement *)prevTopElement;
  if (!(this->viewportRegion == prev->viewportRegion))
    this->updategl();
  prev->capture(state);
}

//! FIXME: write doc.

void
SoGLViewportRegionElement::setElt(const SbViewportRegion & viewportRegion)
{
  this->viewportRegion = viewportRegion;
  this->initialized = TRUE;
  this->updategl();
}

//! FIXME: write doc.

void
SoGLViewportRegionElement::updategl() const
{
  if (this->initialized) {
    SbVec2s origin = this->viewportRegion.getViewportOriginPixels();
    SbVec2s size = this->viewportRegion.getViewportSizePixels();
    glViewport(origin[0], origin[1], size[0], size[1]);

#if COIN_DEBUG && 0 // debug
    SoDebugError::postInfo("SoGLViewportRegionElement::updategl",
                           "glViewport: %d %d %d %d\n",
                           origin[0], origin[1], size[0], size[1]);
#endif // debug
  }
}
