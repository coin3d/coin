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
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoGLViewportRegionElement Inventor/elements/SoGLViewportRegionElement.h
  \brief The SoGLViewportRegionElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLViewportRegionElement.h>


#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <assert.h>

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
  SoGLViewportRegionElement *elem = (SoGLViewportRegionElement*)
    this->next;
  elem->viewportRegion = this->viewportRegion;
  elem->initialized = TRUE;
}

//! FIXME: write doc.

void
SoGLViewportRegionElement::pop(SoState * state,
                               const SoElement * prevTopElement)
{
  SoGLViewportRegionElement *prev =
    (SoGLViewportRegionElement *)prevTopElement;
  if (!(this->viewportRegion == prev->viewportRegion))
    prev->updategl();
  inherited::pop(state, prevTopElement);
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
