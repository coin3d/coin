/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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
  \class SoGLCoordinateElement Inventor/elements/SoGLCoordinateElement.h
  \brief The SoGLCoordinateElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLCoordinateElement.h>


#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
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

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoGLCoordinateElement::send(const int index) const
{
  assert(index < this->numCoords);
  if (is3D()) glVertex3fv((const GLfloat*)(this->getPtr3() + index));
  else glVertex4fv((const GLfloat*)(this->getPtr4() + index));
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
