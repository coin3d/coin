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
