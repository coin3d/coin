/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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
  \class SoGLPolygonOffsetElement Inventor/elements/SoGLPolygonOffsetElement.h
  \brief The SoGLPolygonOffsetElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLPolygonOffsetElement.h>

#include <Inventor/SbName.h>
#include <Inventor/misc/SoGL.h>

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

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
  A constructor.  Can't be used directly.

  \sa void * SoGLPolygonOffsetElement::createInstance(void)
*/

SoGLPolygonOffsetElement::SoGLPolygonOffsetElement(void)
{
  setTypeId(SoGLPolygonOffsetElement::classTypeId);
  setStackIndex(SoGLPolygonOffsetElement::classStackIndex);
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
  this->updategl();
}

//! FIXME: write doc.

void
SoGLPolygonOffsetElement::push(SoState * state)
{
  inherited::push(state);
  SoGLPolygonOffsetElement *top = (SoGLPolygonOffsetElement*)this->next;

  top->currentActive = this->currentActive;
  top->currentStyle = this->currentStyle;
  top->currentOffsetfactor = this->currentOffsetfactor;
  top->currentOffsetunits = this->currentOffsetunits;
}

//! FIXME: write doc.

void
SoGLPolygonOffsetElement::pop(SoState * state, const SoElement * prevTopElement)
{
  SoGLPolygonOffsetElement *prev =
    (SoGLPolygonOffsetElement*)prevTopElement;

  prev->currentActive = this->currentActive;
  prev->currentStyle = this->currentStyle;
  prev->currentOffsetfactor = this->currentOffsetfactor;
  prev->currentOffsetunits = this->currentOffsetunits;

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

//! FIXME: write doc.

void
SoGLPolygonOffsetElement::evaluate() const
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

//! FIXME: write doc.

void
SoGLPolygonOffsetElement::updategl()
{
  // FIXME: might be faster to update the current-values here
  // and check exactly what has changed

  // FIXME: is it possible to enable more than one Style at
  // a time?

  if (this->currentActive) {
#if GL_VERSION_1_1
    if (currentStyle & FILLED)
      glEnable(GL_POLYGON_OFFSET_FILL);
    if (currentStyle & LINES)
      glEnable(GL_POLYGON_OFFSET_LINE);
    if (currentStyle & POINTS)
      glEnable(GL_POLYGON_OFFSET_POINT);

    glPolygonOffset(this->currentOffsetunits, this->currentOffsetfactor);
#elif GL_EXT_polygon_offset
    if (sogl_polygon_offset_ext()) {
      // FIXME: this value (0.0000001) a hack to make it look
      // ok on old SGI HW
      if (currentStyle & FILLED) {
        glPolygonOffsetEXT(this->currentOffsetunits, 0.000001);
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
    if (sogl_polygon_offset_ext()) {
      glDisable(GL_POLYGON_OFFSET_EXT);
    }
#endif
  }
}
