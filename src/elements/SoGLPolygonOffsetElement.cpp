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
  this->currentstyles = (Style) 0;
  this->currentoffsetfactor = 1.0f;
  this->currentoffsetunits = 1.0f;
  this->state = state;
}

//! FIXME: write doc.

void
SoGLPolygonOffsetElement::push(SoState * state)
{
  inherited::push(state);
  SoGLPolygonOffsetElement * prev = (SoGLPolygonOffsetElement*)this->getNextInStack();

  this->currentstyles = prev->currentstyles;
  this->currentoffsetfactor = prev->currentoffsetfactor;
  this->currentoffsetunits = prev->currentoffsetunits;
  this->state = state;
}

//! FIXME: write doc.

void
SoGLPolygonOffsetElement::pop(SoState * state, const SoElement * prevTopElement)
{
  const SoGLPolygonOffsetElement * prev =
    (const SoGLPolygonOffsetElement*)prevTopElement;

  this->currentstyles = prev->currentstyles;
  this->currentoffsetfactor = prev->currentoffsetfactor;
  this->currentoffsetunits = prev->currentoffsetunits;

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
  SoGLPolygonOffsetElement *elem = (SoGLPolygonOffsetElement*)this;
  elem->updategl();
}

// doc in parent
SbBool
SoGLPolygonOffsetElement::isLazy(void) const
{
  return TRUE;
}


//! FIXME: write doc.

void
SoGLPolygonOffsetElement::updategl(void)
{
#if GL_EXT_polygon_offset && !GL_VERSION_1_1
  static int polygon_offset_ext_id = -1;
  if (polygon_offset_ext_id == -1) {
    polygon_offset_ext_id =
      SoGLCacheContextElement::getExtID("GL_EXT_polygon_offset");
  }
#endif GL_EXT_polygon_offset && ! GL_VERSION_1_1

  if (this->active) {
#if GL_VERSION_1_1
    if ((this->style & FILLED) && !(this->currentstyles & FILLED))
      glEnable(GL_POLYGON_OFFSET_FILL);
    if ((this->style & LINES) && !(this->currentstyles & LINES))
      glEnable(GL_POLYGON_OFFSET_LINE);
    if ((this->style & POINTS) && !(this->currentstyles & POINTS))
      glEnable(GL_POLYGON_OFFSET_POINT);
    glPolygonOffset(this->currentoffsetfactor,
                    this->currentoffsetunits);
#ifdef __sgi
    // glPolygonOffset() is Buggy on some IRIX GL-implementations that
    // reports OpenGL 1.1 support, but only has partial
    // support. Nothing dangerous, but causes GL errors/warning. Just
    // clear GL errors for now.  Problem reported by
    // Jacob@fedem.com. pederb 2001-06-25
    while (glGetError() != GL_NO_ERROR);
#endif // __sgi
    
#elif GL_EXT_polygon_offset
    if (SoGLCacheContextElement::extSupported(this->state, polygon_offset_ext_id)) {
      // FIXME: this value (0.0000001) a hack to make it look
      // ok on old SGI HW
      
      // try to detect if user attempted to specify a bias, and not units
      SbBool isbias = this->currentOffsetunits > 0.0f && this->currentOffsetunits < 0.01f;
      glPolygonOffsetEXT(this->currentoffsetfactor,
                         isbias ? this->currentoffsetunits : 0.000001);
      if ((this->styles & FILLED) && !(this->currentstyles & FILLED)) {
        glEnable(GL_POLYGON_OFFSET_EXT);
      }
    }
#endif // GL_EXT_polygon_offset
    this->currentstyles = this->style;
  }
  else { // ! active
#if GL_VERSION_1_1
    if (this->currentstyles & FILLED) 
      glDisable(GL_POLYGON_OFFSET_FILL);
    if (this->currentstyles & LINES)
      glDisable(GL_POLYGON_OFFSET_LINE);
    if (this->currentstyles & POINTS)
      glDisable(GL_POLYGON_OFFSET_POINT);
#ifdef __sgi
    // glPolygonOffset() is Buggy on some IRIX GL-implementations that
    // reports OpenGL 1.1 support, but only has partial
    // support. Nothing dangerous, but causes GL errors/warning. Just
    // clear GL errors for now.  Problem reported by
    // Jacob@fedem.com. pederb 2001-06-25
    while (glGetError() != GL_NO_ERROR);
#endif // __sgi
#elif GL_EXT_polygon_offset
    if (SoGLCacheContextElement::extSupported(this->state, polygon_offset_ext_id)) {
      if (this->currentstyles & FILLED)
        glDisable(GL_POLYGON_OFFSET_EXT);
    }
#endif // GL_EXT_polygon_offset
    this->currentstyles = (Style) 0;
  }
  
  // update current offset values before returning
  this->currentoffsetfactor = this->offsetfactor;
  this->currentoffsetunits = this->offsetunits;
}
