/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

/*!
  \class SoGLLineWidthElement Inventor/elements/SoGLLineWidthElement.h
  \brief The SoGLLineWidthElement class changes the line width setting of the OpenGL render state.

  \ingroup coin_elements

  Requests from the scene graph to change the line width when rendering
  OpenGL line primitives will be made through this element, which
  forwards it to the appropriate native OpenGL call.

  The range and granularity of native line widths depend on the
  underlying OpenGL implementation. This element clamps the native GL
  state to that range while preserving the requested width in Coin's
  traversal state. The native limits can be queried in a valid GL context:

  \code
    GLfloat bounds[2];
    glGetFloatv(GL_LINE_WIDTH_RANGE, bounds);
    GLfloat granularity[1];
    glGetFloatv(GL_LINE_WIDTH_GRANULARITY, granularity);
  \endcode

  Another, perhaps more convenient, way of acquiring the OpenGL
  implementation limits with regard to point sizes is to use the
  So\@Gui\@GLWidget\::getPointSizeLimits() method in the GUI "glue" interface
  library you are using (SoQt, SoXt, SoGtk, SoWin, ...).
*/

#include <Inventor/elements/SoGLLineWidthElement.h>
#include "coindefs.h"

#include <cfloat>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

#include <Inventor/errors/SoDebugError.h>
#include "rendering/SoGL.h"


// Keep the data symbol exported by earlier releases for binary compatibility.
// Rendering uses the range cached for the current OpenGL context.
float SoGLLineWidthElement::sizerange[2] = { FLT_MAX, -1.0f };

SO_ELEMENT_CUSTOM_CONSTRUCTOR_SOURCE(SoGLLineWidthElement);

SoGLLineWidthElement::SoGLLineWidthElement(void)
{
  this->setTypeId(SoGLLineWidthElement::classTypeId);
  this->setStackIndex(SoGLLineWidthElement::classStackIndex);
  this->state = NULL;
}

/*!
  \copydetails SoElement::initClass(void)
*/

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

// doc in superclass
void
SoGLLineWidthElement::init(SoState * stateptr)
{
  this->state = stateptr;
  inherited::init(stateptr);
}

// doc in superclass
void
SoGLLineWidthElement::push(SoState * stateptr)
{
  this->state = stateptr;
  SoGLLineWidthElement * prev = (SoGLLineWidthElement*)this->getNextInStack();
  this->data = prev->data;
  // capture previous element since we might or might not change the
  // GL state in set/pop
  prev->capture(stateptr);
}

// doc in superclass
void
SoGLLineWidthElement::pop(SoState * COIN_UNUSED_ARG(stateptr), const SoElement * prevTopElement)
{
  SoGLLineWidthElement * prev = (SoGLLineWidthElement*)prevTopElement;
  if (this->data != prev->data) {
    this->updategl();
  }
}

// doc in superclass
void
SoGLLineWidthElement::setElt(float width)
{
  if (width != this->data) {
    this->data = width;
    this->updategl();
  }
}

// private
void
SoGLLineWidthElement::updategl(void)
{
  const float * range = cc_glglue_get_line_width_range(sogl_glue_instance(this->state));

  float useval = this->data;

  // 0.0f is used as a "dummy" default value by our superclass and by
  // SoDrawStyle::lineWidth, so handle that case outside of the
  // range check below.

  if (this->data == 0.0f) { useval = 1.0f; }

  // Range checks.

  if (useval < range[0]) {
    useval = range[0];
  }
  if (useval > range[1]) {
    useval = range[1];
  }


  if (COIN_DEBUG) {
    // Detect invalid values and warn the application programmer.
    // (0.0f is used as a "dummy" default value by our superclass and
    // by SoDrawStyle::lineWidth, so ignore that case.)
    if ((useval != this->data) && (this->data != 0.0f)) {
      // Only warn once for each case.
      static SbBool warn_below = TRUE;
      static SbBool warn_above = TRUE;
      if ((warn_below && (useval > this->data)) ||
          (warn_above && (useval < this->data))) {
        if (useval > this->data) { warn_below = FALSE; }
        if (useval < this->data) { warn_above = FALSE; }
        SoDebugError::postWarning("SoGLLineWidthElement::updategl",
                                  "%f is outside the legal range of [%f, %f] "
                                  "for this OpenGL implementation's "
                                  "glLineWidth() settings. It was now clamped.\n\n"
                                  "See the documentation of SoGLLineWidthElement for "
                                  "information on how the application programmer should "
                                  "acquire the boundary values for the legal "
                                  "range.",
                                  this->data,
                                  range[0],
                                  range[1]);
      }
    }
  }

  // Forward to OpenGL state.

  glLineWidth(useval);
}
