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
  \class SoGLTextureMatrixElement Inventor/elements/SoGLTextureMatrixElement.h
  \brief The SoGLTextureMatrixElement class is used to update the OpenGL texture matrix.

  Since (for some weird reason) most OpenGL implementations have a very
  small texture matrix stack, and since the matrix stack also is broken
  on many OpenGL implementations, the texture matrix is always loaded
  into OpenGL. We do not push() and pop() matrices.
*/

#include <Inventor/elements/SoGLTextureMatrixElement.h>


#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>

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
  ((SoGLTextureMatrixElement*)prevTopElement)->updategl();
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
SoGLTextureMatrixElement::updategl() const
{
  // FIXME: matrix mode will always be MODELVIEW when we
  // get here, right?
  // pederb, 990503
  glMatrixMode(GL_TEXTURE);
  glLoadMatrixf(textureMatrix[0]);
  glMatrixMode(GL_MODELVIEW);
}
