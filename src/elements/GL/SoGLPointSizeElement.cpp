/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
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
#ifdef HAVE_CONFIG_H
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
}

// Doc in super.
void
SoGLPointSizeElement::push(SoState * state)
{
  SoGLPointSizeElement * prev = (SoGLPointSizeElement*)this->getNextInStack();
  this->data = prev->data;
  // capture previous element since we might or might not change the
  // GL state in set/pop
  prev->capture(state);
}

// Doc in super.
void
SoGLPointSizeElement::pop(SoState * state,
                          const SoElement * prevTopElement)
{
  SoGLPointSizeElement * prev = (SoGLPointSizeElement*)prevTopElement;
  if (this->data != prev->data) {
    this->updategl();
  }
}

/*! \COININTERNAL */
void
SoGLPointSizeElement::setElt(float val)
{
  if (val != this->data) {
    this->data = val;
    this->updategl();
  }
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

  float useval = this->data;

  // 0.0f is used as a "dummy" default value by our superclass and by
  // SoDrawStyle::pointSize, so handle that case outside of the
  // rangecheck below.

  if (this->data == 0.0f) { useval = 1.0f; }

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
    if ((useval != this->data) && (this->data != 0.0f)) {
      // Only warn once for each case.
      static SbBool warn_below = TRUE;
      static SbBool warn_above = TRUE;
      if ((warn_below && (useval > this->data)) ||
          (warn_above && (useval < this->data))) {
        if (useval > this->data) { warn_below = FALSE; }
        if (useval < this->data) { warn_above = FALSE; }
        SoDebugError::postWarning("SoGLPointSizeElement::updategl",
                                  "%f is outside the legal range of [%f, %f] "
                                  "for this OpenGL implementation's "
                                  "glPointSize() settings. It was now clamped.\n\n"
                                  "See the documentation of SoGLPointSizeElement for "
                                  "information on how the application programmer should "
                                  "acquire the boundary values for the legal "
                                  "range.",
                                  this->data,
                                  SoGLPointSizeElement::sizerange[0],
                                  SoGLPointSizeElement::sizerange[1]);
      }
    }
  }

  // Forward to OpenGL state.

  glPointSize(useval);
}
