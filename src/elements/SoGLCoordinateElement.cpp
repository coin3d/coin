/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoGLCoordinateElement Inventor/elements/SoGLCoordinateElement.h
  \brief The SoGLCoordinateElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLCoordinateElement.h>

#include <Inventor/SbName.h>

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
  A constructor.  Can't be used directly.

  \sa void * SoGLCoordinateElement::createInstance(void)
*/

SoGLCoordinateElement::SoGLCoordinateElement(void)
{
    setTypeId(SoGLCoordinateElement::classTypeId);
    setStackIndex(SoGLCoordinateElement::classStackIndex);
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
