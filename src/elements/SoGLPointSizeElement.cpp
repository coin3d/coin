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
  \class SoGLPointSizeElement Inventor/elements/SoGLPointSizeElement.h
  \brief The SoGLPointSizeElement class changes the pointsize setting of the OpenGL render state.
  \ingroup elements

  Requests from the scenegraph to change the pointsize when rendering
  point primitives will be made through this element, which forwards
  it to the appropriate native OpenGL call.

  The Coin library does not place any bounds on the values of the
  pointsize, but be aware that the range and granularity of what is
  valid pointsizes depends on the underlying OpenGL
  implementation. Application programmers using point primitives
  (typically through the SoPointSet node) should heed these boundary
  values. They can be acquired by running the following code from
  within a valid OpenGL context:

  \code
    GLfloat bounds[2];
    glGetFloatv(GL_POINT_SIZE_RANGE, bounds);
    GLfloat granularity[1];
    glGetFloatv(GL_POINT_SIZE_GRANULARITY, granularity);
  \endcode

  Another, perhaps more convenient, way of acquiring the OpenGL
  implementation limits with regard to pointsizes is to use the
  So*GLWidget::getPointSizeLimits() method in the GUI "glue" interface
  library you are using (SoQt, SoXt, SoGtk, SoWin, ...).
*/

#include <Inventor/elements/SoGLPointSizeElement.h>
#include <Inventor/errors/SoDebugError.h>

#include <float.h>
#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

// Also used in other element .cpp files, so wrap to handle
// --enable-compact build mode.
#ifndef RANGE_NOT_CHECKED
// Important note: do _not_ use a "static const" variable instead, as
// it is then not given that it will be initialized before the static
// "sizerange" class variable array below.
#define RANGE_NOT_CHECKED FLT_MAX
#endif // RANGE_NOT_CHECKED

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

/*! \COININTERNAL */
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
SoGLPointSizeElement::updategl(void)
{
  if (SoGLPointSizeElement::sizerange[0] == RANGE_NOT_CHECKED) {
    GLfloat vals[2];
    glGetFloatv(GL_POINT_SIZE_RANGE, vals);

    // Matthias Koenig reported on coin-discuss that the OpenGL
    // implementation on SGI Onyx 2 InfiniteReality returns 0 for the
    // lowest pointsize, but it will still set the return value of
    // glGetError() to GL_INVALID_VALUE if this size is attempted
    // used. So the boundary range fix in the next line of code is a
    // workaround for that OpenGL implementation bug.
    //
    // 0.0f and lower values are explicitly disallowed, according to
    // the OpenGL 1.3 specification, Chapter 3.3.

    if (vals[0] <= 0.0f) { vals[0] = SbMin(1.0f, vals[1]); }

    SoGLPointSizeElement::sizerange[0] = vals[0];
    SoGLPointSizeElement::sizerange[1] = vals[1];
  }

  float useval = this->current;

  // 0.0f is used as a "dummy" default value by our superclass and by
  // SoDrawStyle::pointSize, so handle that case outside of the
  // rangecheck below.

  if (this->current == 0.0f) { useval = 1.0f; }

  // Range checks.

  if (useval < SoGLPointSizeElement::sizerange[0]) {
    useval = SoGLPointSizeElement::sizerange[0];
  }
  if (useval > SoGLPointSizeElement::sizerange[1]) {
    useval = SoGLPointSizeElement::sizerange[1];
  }

  if (COIN_DEBUG) {
    // Detect invalid values and warn the application programmer.
    // (0.0f is used as a "dummy" default value by our superclass and
    // by SoDrawStyle::pointSize, so ignore that case.)
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
                                  "glPointSize() settings. It was now clamped.\n\n"
                                  "See the documentation of SoGLPointSizeElement for "
                                  "information on how the application programmer should "
                                  "acquire the boundary values for the legal "
                                  "range.",
                                  this->current,
                                  SoGLPointSizeElement::sizerange[0],
                                  SoGLPointSizeElement::sizerange[1]);
      }
    }
  }

  // Forward to OpenGL state.

  glPointSize(useval);
}
