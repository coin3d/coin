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
  \class SoGLProjectionMatrixElement Inventor/elements/SoGLProjectionMatrixElement.h
  \brief The SoGLProjectionMatrixElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLProjectionMatrixElement.h>


#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

#include <assert.h>

SO_ELEMENT_SOURCE(SoGLProjectionMatrixElement);

/*!
  This static method initializes static data for the
  SoGLProjectionMatrixElement class.
*/

void
SoGLProjectionMatrixElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLProjectionMatrixElement, inherited);
}

/*!
  The destructor.
*/

SoGLProjectionMatrixElement::~SoGLProjectionMatrixElement(void)
{
}

//! FIXME: write doc.

void
SoGLProjectionMatrixElement::pop(SoState * state,
                                 const SoElement * prevTopElement)
{
  this->capture(state);
  this->updategl();
}

//! FIXME: write doc.

void
SoGLProjectionMatrixElement::setElt(const SbMatrix & matrix)
{
  inherited::setElt(matrix);
  this->updategl();
}

//! FIXME: write doc.

void
SoGLProjectionMatrixElement::updategl(void)
{
#if 0 // debug
  SoDebugError::postInfo("SoGLProjectionMatrixElement::updategl", "");
#endif // debug
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf((float*)this->projectionMatrix);
  glMatrixMode(GL_MODELVIEW);
}
