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
  \class SoGLTextureMatrixElement Inventor/elements/SoGLTextureMatrixElement.h
  \brief The SoGLTextureMatrixElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLTextureMatrixElement.h>

#include <Inventor/SbName.h>

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>

//$ BEGIN TEMPLATE ElementSource(SoGLTextureMatrixElement)

/*!
  \var SoGLTextureMatrixElement::classTypeId

  This is the static class type identifier for the
  SoGLTextureMatrixElement class.
*/

SoType SoGLTextureMatrixElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoGLTextureMatrixElement::getClassTypeId(void)
{
  return SoGLTextureMatrixElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoGLTextureMatrixElement class.
*/
void *
SoGLTextureMatrixElement::createInstance(void)
{
  return (void *) new SoGLTextureMatrixElement;
}

/*!
  \var SoGLTextureMatrixElement::classStackIndex

  This is the static state stack index for the
  SoGLTextureMatrixElement class.
*/
int SoGLTextureMatrixElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoGLTextureMatrixElement class.
*/
int
SoGLTextureMatrixElement::getClassStackIndex(void)
{
  return SoGLTextureMatrixElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoGLTextureMatrixElement class.
*/

void
SoGLTextureMatrixElement::initClass(void)
{
//$ BEGIN TEMPLATE InitElementSource(SoGLTextureMatrixElement)
  assert(SoGLTextureMatrixElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGLTextureMatrixElement::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SoGLTextureMatrixElement",
                       &SoGLTextureMatrixElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoGLTextureMatrixElement::classStackIndex =
      createStackIndex(SoGLTextureMatrixElement::classTypeId);
  }
  else {
    SoGLTextureMatrixElement::classStackIndex = inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa SoGLTextureMatrixElement::createInstance(void)
*/

SoGLTextureMatrixElement::SoGLTextureMatrixElement(void)
{
    setTypeId(SoGLTextureMatrixElement::classTypeId);
    setStackIndex(SoGLTextureMatrixElement::classStackIndex);
}

/*!
  The destructor.
*/

SoGLTextureMatrixElement::~SoGLTextureMatrixElement(void)
{
}

//! FIXME: write doc.

void
SoGLTextureMatrixElement::init(SoState * state)
{
  inherited::init(state);
}

//! FIXME: write doc.

void
SoGLTextureMatrixElement::push(SoState * state)
{
  inherited::push(state);
}

//! FIXME: write doc.

void
SoGLTextureMatrixElement::pop(SoState * state, 
			      const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
  ((SoGLTextureMatrixElement*)prevTopElement)->updategl();
}

//! FIXME: write doc.

void
SoGLTextureMatrixElement::setEltIdentity()
{
  assert(0); //seems like an OIV hack for VRML V2.0... pederb, 990503

  // the GL matrix will be correct, but the inherited textureMatrix 
  // will not be correct...
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
}

//! FIXME: write doc.

void
SoGLTextureMatrixElement::makeEltIdentity()
{
  inherited::makeEltIdentity();
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
}

//! FIXME: write doc.

void
SoGLTextureMatrixElement::multElt(const SbMatrix & matrix)
{
  inherited::multElt(matrix);
  this->updategl();
}

//! FIXME: write doc.

void
SoGLTextureMatrixElement::translateEltBy(const SbVec3f & translation)
{
  inherited::translateEltBy(translation);
  this->updategl();
}

//! FIXME: write doc.

void
SoGLTextureMatrixElement::rotateEltBy(const SbRotation &rotation)
{
  inherited::rotateEltBy(rotation);
  this->updategl();
}

//! FIXME: write doc.

void
SoGLTextureMatrixElement::scaleEltBy(const SbVec3f & scaleFactor)
{
  inherited::scaleEltBy(scaleFactor);
  this->updategl();
}

//! FIXME: write doc.

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

