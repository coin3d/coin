/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoLineWidthElement Inventor/elements/SoLineWidthElement.h
  \brief The SoLineWidthElement class changes the linewidth setting of the render state.
  \ingroup elements

  Requests from the scenegraph to change the linewidth when rendering
  line primitives will be made through this element, which forwards it
  to the appropriate native call in the underlying rendering library.

  Subsequent nodes rendering line primitives will use the width
  setting (for instance SoLineSet nodes).
*/

#include <Inventor/elements/SoLineWidthElement.h>


#include <assert.h>

SO_ELEMENT_SOURCE(SoLineWidthElement);

// doc in super
void
SoLineWidthElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoLineWidthElement, inherited);
}

/*!
  Destructor.
*/
SoLineWidthElement::~SoLineWidthElement()
{
}

// doc in super
void
SoLineWidthElement::init(SoState * state)
{
  inherited::init(state);
  this->data = SoLineWidthElement::getDefault();
}

/*!
  Set up the current state's \a lineWidth value.
 */
void
SoLineWidthElement::set(SoState * const state, SoNode * const node,
                        const float lineWidth)
{
  SoFloatElement::set(classStackIndex, state, node, lineWidth);
}

/*!
  Set up the current state's \a lineWidth value.
 */
void
SoLineWidthElement::set(SoState * const state, const float lineWidth)
{
  SoLineWidthElement::set(state, NULL, lineWidth);
}

/*!
  Returns the current line width value.
 */
float
SoLineWidthElement::get(SoState * const state)
{
  return SoFloatElement::get(classStackIndex, state);
}

/*!
  Returns the default linewidth value if no value has been set
  explicitly.
 */
float
SoLineWidthElement::getDefault(void)
{
  // 0 is an indicator value which means "use the default value of the
  // rendering-library specific So*LineWidth element".
  return 0.0f;
}
