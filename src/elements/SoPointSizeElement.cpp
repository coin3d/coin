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
  \class SoPointSizeElement Inventor/elements/SoPointSizeElement.h
  \brief The SoPointSizeElement changes the pointsize setting of the render state.
  \ingroup elements

  Requests from the scenegraph to change the pointsize when rendering
  point primitives will be made through this element.

  The SoPointSizeElement class itself is just a generic abstraction
  for the underlying So*PointSizeElement classes with code specific
  for each supported immediate mode rendering library.
*/

#include <Inventor/elements/SoPointSizeElement.h>


#include <assert.h>

SO_ELEMENT_SOURCE(SoPointSizeElement);

// Doc in super.
void
SoPointSizeElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoPointSizeElement, inherited);
}

/*!
  Destructor.
*/
SoPointSizeElement::~SoPointSizeElement(void)
{
}

// Doc in super.
void
SoPointSizeElement::init(SoState * state)
{
  inherited::init(state);
  this->data = SoPointSizeElement::getDefault();
}

/*!
  Static method for setting the current \a pointSize value in the
  given traversal \a state.
*/
void
SoPointSizeElement::set(SoState * const state, SoNode * const node,
                        const float pointSize)
{
  inherited::set(SoPointSizeElement::classStackIndex, state, node, pointSize);
}

/*!
  Static method for setting the current \a pointSize value in the
  given traversal \a state.
*/
void
SoPointSizeElement::set(SoState * const state, const float pointSize)
{
  SoPointSizeElement::set(state, NULL, pointSize);
}

/*!
  Static method for returning the current pointsize setting in the
  given traversal \a state.
*/
float
SoPointSizeElement::get(SoState * const state)
{
  return inherited::get(SoPointSizeElement::classStackIndex, state);
}

/*!
  Returns default pointsize setting.
*/
float
SoPointSizeElement::getDefault(void)
{
  return 0.0f;
}
