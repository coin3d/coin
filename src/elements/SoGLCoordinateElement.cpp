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
  \class SoGLCoordinateElement Inventor/elements/SoGLCoordinateElement.h
  \brief The SoGLCoordinateElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLCoordinateElement.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

#include <assert.h>

SO_ELEMENT_SOURCE(SoGLCoordinateElement);

/*!
  This static method initializes static data for the SoGLCoordinateElement
  class.
*/

void
SoGLCoordinateElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLCoordinateElement, inherited);
}

/*!
  The destructor.
*/

SoGLCoordinateElement::~SoGLCoordinateElement(void)
{
}

/*!
  Send coordinates \a index to GL. Handles both 3D and 4D coordinates.
*/
void
SoGLCoordinateElement::send(const int index) const
{
  if (this->areCoords3D) glVertex3fv((const GLfloat*)(this->coords3D + index));
  else glVertex4fv((const GLfloat*)(this->coords4D + index));
}

//! FIXME: write doc.

//$ EXPORT INLINE
const SbVec3f *
SoGLCoordinateElement::getPtr3() const
{
  return this->coords3D;
}

//! FIXME: write doc.

//$ EXPORT INLINE
const SbVec4f *
SoGLCoordinateElement::getPtr4() const
{
  return this->coords4D;
}
