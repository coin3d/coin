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
  \class SoGLPointSizeElement Inventor/elements/SoGLPointSizeElement.h
  \brief The SoGLPointSizeElement class changes the pointsize setting of the OpenGL render state.
  \ingroup elements

  Requests from the scenegraph to change the pointsize when rendering
  point primitives will be made through this element, which forwards
  it to the appropriate native OpenGL call.
*/

#include <Inventor/elements/SoGLPointSizeElement.h>
#include <Inventor/errors/SoDebugError.h>

#include <float.h>
#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

static const float RANGE_NOT_CHECKED = FLT_MAX;

float SoGLPointSizeElement::sizerange[2] = { RANGE_NOT_CHECKED, 0.0f };

SO_ELEMENT_SOURCE(SoGLPointSizeElement);

// Doc in super.
void
SoGLPointSizeElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLPointSizeElement, inherited);
}

/*!
  The destructor.
*/
SoGLPointSizeElement::~SoGLPointSizeElement(void)
{
}

// Doc in super.
void
SoGLPointSizeElement::init(SoState * state)
{
  inherited::init(state);
  this->current = -1.0f;
}

// Doc in super.
void
SoGLPointSizeElement::push(SoState * state)
{
  inherited::push(state);
  SoGLPointSizeElement * prev = (SoGLPointSizeElement*)this->getNextInStack();
  this->current = prev->current;
}

// Doc in super.
void
SoGLPointSizeElement::pop(SoState * state,
                          const SoElement * prevTopElement)
{
  SoGLPointSizeElement * prev = (SoGLPointSizeElement*)prevTopElement;
  this->current = prev->current;
  inherited::pop(state, prevTopElement);
}

/*! \internal */
void
SoGLPointSizeElement::setElt(float val)
{
  inherited::setElt(val);
  // this is a lazy element, do nothing
}


// Doc in super.
void
SoGLPointSizeElement::lazyEvaluate(void) const
{
  if (this->data != current) {
    // cast away constness
    SoGLPointSizeElement *elem = (SoGLPointSizeElement*)this;
    elem->current = this->data;
    elem->updategl();
  }
}

// Doc in super.
SbBool
SoGLPointSizeElement::isLazy(void) const
{
  return TRUE;
}


// update GL state
void
SoGLPointSizeElement::updategl()
{
  if (SoGLPointSizeElement::sizerange[0] == RANGE_NOT_CHECKED) {
    SbVec2f vals = SoGLPointSizeElement::getValidRange();

    // Matthias Koenig reported on coin-discuss that the OpenGL
    // implementation on SGI Onyx 2 InfiniteReality returns 0 for the
    // lowest pointsize, but it will still set the return value of
    // glGetError() to GL_INVALID_VALUE if this size is attempted
    // used. This is a workaround for what looks like an OpenGL
    // implementation bug.
    //
    // On the other hand, I'm not sure this is the correct
    // fix.. shouldn't it in general be possible to specify pointsizes
    // in the range <0.0f, 1.0f>?  (0.0f and lower values are
    // explicitly disallowed, according to the OpenGL 1.3
    // specification, Chapter 3.3.)
    //
    // The 99% correct fix would probably be to check the OpenGL
    // vendor / version string at run-time and then conditionally
    // activate this fix if we are running the particular
    // implementation(s) we know contains the bug.  (I call it only a
    // 99% correct fix because I think that scheme will not work
    // either if the client application does remote rendering (over
    // GLX) -- but that should be investigated).
    //
    //                     mortene@coin3d.org
    //
    if (vals[0] <= 0.0f) { vals[0] = SbMin(1.0f, vals[1]); }

    SoGLPointSizeElement::sizerange[0] = vals[0];
    SoGLPointSizeElement::sizerange[1] = vals[1];
  }

  float useval = this->current;

  // Range check.

  if (useval < SoGLPointSizeElement::sizerange[0]) {
    useval = SoGLPointSizeElement::sizerange[0];
  }
  if (useval > SoGLPointSizeElement::sizerange[1]) {
    useval = SoGLPointSizeElement::sizerange[1];
  }

#if COIN_DEBUG
  // Detect invalid values and warn the application programmer.  (0.0f
  // is used as a "dummy" default value by our superclass and by
  // SoDrawStyle::pointSize, so ignore that case.)
  if ((useval != this->current) && (this->current != 0.0f)) {
    // Only warn once for each case.
    static SbBool warn_below = TRUE;
    static SbBool warn_above = TRUE;
    if ((warn_below && (useval > this->current)) ||
        (warn_above && (useval < this->current))) {
      if (useval > this->current) { warn_below = FALSE; }
      if (useval < this->current) { warn_above = FALSE; }
      SoDebugError::postWarning("SoGLPointSizeElement::updategl",
                                "%f is outside the legal range of [%f, %f] "
                                "for this OpenGL implementation's "
                                "glPointSize() settings. It was now clamped.\n"
                                "Note that the method SoGLPointSizeElement::getValidRange() "
                                "is available for the application program for "
                                "acquiring the boundary values for the legal "
                                "range.",
                                this->current,
                                SoGLPointSizeElement::sizerange[0],
                                SoGLPointSizeElement::sizerange[1]);
    }
  }
#endif // COIN_DEBUG

  // Forward to OpenGL state.

  glPointSize(useval);
}

/*!
  Does a run-time check against the OpenGL implementation and returns
  the valid range of point sizes.

  The OpenGL specification does not demand a particular valid range,
  so different implementations are free to decide this on their own.
  This method should be used to detect which values are safe to use,
  for instance for SoDrawStyle::pointSize.

  This method is not part of the original SGI Open Inventor v2.1 API,
  but is an extension specific for the Coin library.
 */
SbVec2f
SoGLPointSizeElement::getValidRange(void)
{
  GLfloat vals[2];
  glGetFloatv(GL_POINT_SIZE_RANGE, vals);
  return SbVec2f(vals[0], vals[1]);
}
