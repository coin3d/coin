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
  \class SoGLTextureMatrixElement Inventor/elements/SoGLTextureMatrixElement.h
  \brief The SoGLTextureMatrixElement class is used to update the OpenGL texture matrix.
  \ingroup elements

  Since (for some weird reason) most OpenGL implementations have a very
  small texture matrix stack, and since the matrix stack also is broken
  on many OpenGL implementations, the texture matrix is always loaded
  into OpenGL. We do not push() and pop() matrices.
*/

#include <Inventor/elements/SoGLTextureMatrixElement.h>


#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

SO_ELEMENT_SOURCE(SoGLTextureMatrixElement);


// doc from parent
void
SoGLTextureMatrixElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLTextureMatrixElement, inherited);
}

/*!
  The destructor.
*/

SoGLTextureMatrixElement::~SoGLTextureMatrixElement(void)
{
}

// doc from parent
void
SoGLTextureMatrixElement::init(SoState * state)
{
  inherited::init(state);
}

// doc from parent
void
SoGLTextureMatrixElement::pop(SoState * state,
                              const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
  this->updategl();
}

// doc from parent
void
SoGLTextureMatrixElement::setEltIdentity(void)
{
  assert(0); //seems like an OIV hack for VRML V2.0... pederb, 990503

  // the GL matrix will be correct, but the inherited textureMatrix
  // will not be correct...
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
}


// doc from parent
void
SoGLTextureMatrixElement::makeEltIdentity(void)
{
  inherited::makeEltIdentity();
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
}

// doc from parent
void
SoGLTextureMatrixElement::setElt(const SbMatrix & matrix)
{
  inherited::setElt(matrix);
  this->updategl();
}


// doc from parent
void
SoGLTextureMatrixElement::multElt(const SbMatrix & matrix)
{
  inherited::multElt(matrix);
  this->updategl();
}

// doc from parent
void
SoGLTextureMatrixElement::translateEltBy(const SbVec3f & translation)
{
  inherited::translateEltBy(translation);
  this->updategl();
}

// doc from parent
void
SoGLTextureMatrixElement::rotateEltBy(const SbRotation & rotation)
{
  inherited::rotateEltBy(rotation);
  this->updategl();
}


// doc from parent
void
SoGLTextureMatrixElement::scaleEltBy(const SbVec3f & scaleFactor)
{
  inherited::scaleEltBy(scaleFactor);
  this->updategl();
}

// updates GL state
void
SoGLTextureMatrixElement::updategl(void) const
{
  glMatrixMode(GL_TEXTURE);
  glLoadMatrixf(textureMatrix[0]);
  glMatrixMode(GL_MODELVIEW);
}
