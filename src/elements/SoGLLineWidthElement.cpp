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
  \class SoGLLineWidthElement Inventor/elements/SoGLLineWidthElement.h
  \brief The SoGLLineWidthElement class changes the linewidth setting of the OpenGL render state.
  \ingroup elements

  Requests from the scenegraph to change the linewidth when rendering
  OpenGL line primitives will be made through this element, which
  forwards it to the appropriate native OpenGL call.

  The Coin library does not place any bounds on the values of the
  linewidths, but be aware that the range and granularity of what is
  valid linewidths depends on the underlying OpenGL
  implementation. Application programmers using line primitives
  (typically through the SoLineSet or SoIndexedLineSet nodes) should
  heed these boundary values. They can be acquired by running the
  following code from within a valid OpenGL context:

  \code
    GLfloat bounds[2];
    glGetFloatv(GL_LINE_WIDTH_RANGE, bounds);
    GLfloat granularity[1];
    glGetFloatv(GL_LINE_WIDTH_GRANULARITY, granularity);
  \endcode

  For the next version of Coin, we expect to add a query function in
  the Coin API which the application programmer can use to get hold of
  these values without resorting to using direct OpenGL calls.
*/

// FIXME: the API extension mentioned at the end of the classdoc above
// seems best suited to be placed in the API of the So*-libraries -- I
// believe that's the only way for us to make it possible to guarantee
// a valid OpenGL context. (One problem with this: we don't _really_
// want more OpenGL code than absolutely necessary inside the So*
// libraries.) 20011019 mortene.

#include <Inventor/elements/SoGLLineWidthElement.h>

#include <float.h>
#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

#include <assert.h>
#include <Inventor/errors/SoDebugError.h>

// Also used in other element .cpp files, so wrap to handle
// --enable-compact build mode.
#ifndef RANGE_NOT_CHECKED
// Important note: do _not_ use a "static const" variable instead, as
// it is then not given that it will be initialized before the static
// "sizerange" class variable array below.
#define RANGE_NOT_CHECKED FLT_MAX
#endif // RANGE_NOT_CHECKED

float SoGLLineWidthElement::sizerange[2] = { RANGE_NOT_CHECKED, -1.0f};


SO_ELEMENT_SOURCE(SoGLLineWidthElement);

// doc in super
void
SoGLLineWidthElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLLineWidthElement, inherited);
}

/*!
  Destructor.
*/
SoGLLineWidthElement::~SoGLLineWidthElement(void)
{
}

// doc in super
void
SoGLLineWidthElement::init(SoState * state)
{
  inherited::init(state);
  this->current = -1.0f;
}

// doc in super
void
SoGLLineWidthElement::push(SoState * state)
{
  inherited::push(state);

  SoGLLineWidthElement * prev = (SoGLLineWidthElement*)this->getNextInStack();
  this->current = prev->current;
}

// doc in super
void
SoGLLineWidthElement::pop(SoState * state, const SoElement * prevTopElement)
{
  SoGLLineWidthElement * prev = (SoGLLineWidthElement*)prevTopElement;
  this->current = prev->current;
  inherited::pop(state, prevTopElement);
}

// doc in super
void
SoGLLineWidthElement::setElt(float width)
{
  inherited::setElt(width);
  // this is a lazy element. Do nothing
}


// doc in parent
void
SoGLLineWidthElement::lazyEvaluate(void) const
{
  if (this->data != this->current) {
    // cast away constness
    SoGLLineWidthElement *elem = (SoGLLineWidthElement*)this;
    elem->current = elem->data;
    elem->updategl();
  }
}

// doc in parent
SbBool
SoGLLineWidthElement::isLazy(void) const
{
  return TRUE;
}

//! FIXME: write doc.

void
SoGLLineWidthElement::updategl()
{
  if (SoGLLineWidthElement::sizerange[0] == RANGE_NOT_CHECKED) {
    GLfloat vals[2];
    glGetFloatv(GL_LINE_WIDTH_RANGE, vals);

    // Matthias Koenig reported on coin-discuss that the OpenGL
    // implementation on SGI Onyx 2 InfiniteReality returns 0 for the
    // lowest linewidth, but it will still set the return value of
    // glGetError() to GL_INVALID_VALUE if this size is attempted
    // used. This is a workaround for what looks like an OpenGL bug.

    if (vals[0] <= 0.0f) { vals[0] = SbMin(1.0f, vals[1]); }

    SoGLLineWidthElement::sizerange[0] = vals[0];
    SoGLLineWidthElement::sizerange[1] = vals[1];

    // FIXME: we should make the range values available for the
    // app-programmer with a public API on this class (or somewhere
    // else?). Along with the range we should also grab the value of
    // GL_LINE_WIDTH_GRANULARITY and make that available
    // aswell. 20011019 mortene.
  }

  float useval = this->current;

  // 0.0f is used as a "dummy" default value by our superclass and by
  // SoDrawStyle::lineWidth, so handle that case outside of the
  // rangecheck below.

  if (this->current == 0.0f) { useval = 1.0f; }

  // Range checks.

  if (useval < SoGLLineWidthElement::sizerange[0]) {
    useval = SoGLLineWidthElement::sizerange[0];
  }
  if (useval > SoGLLineWidthElement::sizerange[1]) {
    useval = SoGLLineWidthElement::sizerange[1];
  }


  if (COIN_DEBUG) {
    // Detect invalid values and warn the application programmer.
    // (0.0f is used as a "dummy" default value by our superclass and
    // by SoDrawStyle::lineWidth, so ignore that case.)
    if ((useval != this->current) && (this->current != 0.0f)) {
      // Only warn once for each case.
      static SbBool warn_below = TRUE;
      static SbBool warn_above = TRUE;
      if ((warn_below && (useval > this->current)) ||
          (warn_above && (useval < this->current))) {
        if (useval > this->current) { warn_below = FALSE; }
        if (useval < this->current) { warn_above = FALSE; }
        SoDebugError::postWarning("SoGLLineWidthElement::updategl",
                                  "%f is outside the legal range of [%f, %f] "
                                  "for this OpenGL implementation's "
                                  "glLineWidth() settings. It was now clamped.\n\n"
                                  "See the documentation of SoGLLineWidthElement for "
                                  "information on how to the application programmer may "
                                  "acquire the boundary values for the legal "
                                  "range.",
                                  this->current,
                                  SoGLLineWidthElement::sizerange[0],
                                  SoGLLineWidthElement::sizerange[1]);
      }
    }
  }

  // Forward to OpenGL state.

  glLineWidth(useval);
}
