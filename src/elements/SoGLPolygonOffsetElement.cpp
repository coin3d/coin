/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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
  \class SoGLPolygonOffsetElement Inventor/elements/SoGLPolygonOffsetElement.h
  \brief The SoGLPolygonOffsetElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLPolygonOffsetElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#if HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>

#include <assert.h>

SO_ELEMENT_SOURCE(SoGLPolygonOffsetElement);

/*!
  This static method initializes static data for the
  SoGLPolygonOffsetElement class.
*/

void
SoGLPolygonOffsetElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLPolygonOffsetElement, inherited);
}

/*!
  The destructor.
*/

SoGLPolygonOffsetElement::~SoGLPolygonOffsetElement(void)
{
}

//! FIXME: write doc.

void
SoGLPolygonOffsetElement::init(SoState * state)
{
  inherited::init(state);
  this->currentActive = FALSE;
  this->currentStyle = FILLED;
  this->currentOffsetfactor = 1.0f;
  this->currentOffsetunits = 1.0f;
  this->state = state;
  this->updategl();
}

//! FIXME: write doc.

void
SoGLPolygonOffsetElement::push(SoState * state)
{
  inherited::push(state);
  SoGLPolygonOffsetElement * prev = (SoGLPolygonOffsetElement*)this->getNextInStack();

  this->currentActive = prev->currentActive;
  this->currentStyle = prev->currentStyle;
  this->currentOffsetfactor = prev->currentOffsetfactor;
  this->currentOffsetunits = prev->currentOffsetunits;
  this->state = prev->state;
}

//! FIXME: write doc.

void
SoGLPolygonOffsetElement::pop(SoState * state, const SoElement * prevTopElement)
{
  const SoGLPolygonOffsetElement * prev =
    (const SoGLPolygonOffsetElement*)prevTopElement;

  this->currentActive = prev->currentActive;
  this->currentStyle = prev->currentStyle;
  this->currentOffsetfactor = prev->currentOffsetfactor;
  this->currentOffsetunits = prev->currentOffsetunits;

  inherited::pop(state, prevTopElement);
}

//! FIXME: write doc.

void
SoGLPolygonOffsetElement::setElt(float factor, float units,
                                 Style styles, SbBool on)
{
  inherited::setElt(factor, units, styles, on);
  // this is a lazy element. Do nothing.
}


// doc in parent
void
SoGLPolygonOffsetElement::lazyEvaluate(void) const
{
  if (this->currentActive != this->active ||
      this->currentStyle != this->style ||
      this->currentOffsetfactor != this->offsetfactor ||
      this->currentOffsetunits != this->offsetunits) {

    SoGLPolygonOffsetElement *elem = (SoGLPolygonOffsetElement*)this;
    elem->currentActive = this->active;
    elem->currentStyle = this->style;
    elem->currentOffsetfactor = this->offsetfactor;
    elem->currentOffsetunits = this->offsetunits;
    elem->updategl();
  }
}

// doc in parent
SbBool
SoGLPolygonOffsetElement::isLazy(void) const
{
  return TRUE;
}


//! FIXME: write doc.

void
SoGLPolygonOffsetElement::updategl()
{
  // FIXME: might be faster to update the current-values here
  // and check exactly what has changed

  // FIXME: is it possible to enable more than one Style at
  // a time?

#if GL_EXT_polygon_offset && ! GL_VERSION_1_1
  static int polygon_offset_ext_id = -1;
  if (polygon_offset_ext_id == -1) {
    polygon_offset_ext_id =
      SoGLCacheContextElement::getExtID("GL_EXT_polygon_offset");
  }
#endif // GL_EXT_polygon_offset && ! GL_VERSION_1_1

  if (this->currentActive) {
#if GL_VERSION_1_1
    if (currentStyle & FILLED)
      glEnable(GL_POLYGON_OFFSET_FILL);
    if (currentStyle & LINES)
      glEnable(GL_POLYGON_OFFSET_LINE);
    if (currentStyle & POINTS)
      glEnable(GL_POLYGON_OFFSET_POINT);

    glPolygonOffset(this->currentOffsetfactor,
                    this->currentOffsetunits);
#elif GL_EXT_polygon_offset
    if (SoGLCacheContextElement::extSupported(this->state, polygon_offset_ext_id)) {
      // FIXME: this value (0.0000001) a hack to make it look
      // ok on old SGI HW

      // try to detect if user attempted to specify a bias, and not units
      SbBool isbias = this->currentOffsetunits > 0.0f && this->currentOffsetunits < 0.01f;
      if (currentStyle & FILLED) {
        glPolygonOffsetEXT(this->currentOffsetfactor,
                           isbias ? this->currentOffsetunits : 0.000001);
        glEnable(GL_POLYGON_OFFSET_EXT);
      }
    }
#endif
  }
  else {
#if GL_VERSION_1_1
    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);
    glDisable(GL_POLYGON_OFFSET_POINT);
#elif GL_EXT_polygon_offset
    if (SoGLCacheContextElement::extSupported(this->state, polygon_offset_ext_id)) {
      glDisable(GL_POLYGON_OFFSET_EXT);
    }
#endif
  }
}
