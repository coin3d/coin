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
  \class SoGLNormalElement Inventor/elements/SoGLNormalElement.h
  \brief The SoGLNormalElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLNormalElement.h>

#include <Inventor/SbName.h>

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <assert.h>

//$ BEGIN TEMPLATE ElementSource(SoGLNormalElement)

/*!
  \var SoGLNormalElement::classTypeId

  This is the static class type identifier for the
  SoGLNormalElement class.
*/

SoType SoGLNormalElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoGLNormalElement::getClassTypeId(void)
{
  return SoGLNormalElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoGLNormalElement class.
*/
void *
SoGLNormalElement::createInstance(void)
{
  return (void *) new SoGLNormalElement;
}

/*!
  \var SoGLNormalElement::classStackIndex

  This is the static state stack index for the
  SoGLNormalElement class.
*/
int SoGLNormalElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoGLNormalElement class.
*/
int
SoGLNormalElement::getClassStackIndex(void)
{
  return SoGLNormalElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the SoGLNormalElement class.
*/

void
SoGLNormalElement::initClass(void)
{
//$ BEGIN TEMPLATE InitElementSource(SoGLNormalElement)
  assert(SoGLNormalElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGLNormalElement::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SoGLNormalElement",
                       &SoGLNormalElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoGLNormalElement::classStackIndex =
      createStackIndex(SoGLNormalElement::classTypeId);
  }
  else {
    SoGLNormalElement::classStackIndex = inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLNormalElement::createInstance(void)
*/

SoGLNormalElement::SoGLNormalElement(void)
{
    setTypeId(SoGLNormalElement::classTypeId);
    setStackIndex(SoGLNormalElement::classStackIndex);
}

/*!
  The destructor.
*/

SoGLNormalElement::~SoGLNormalElement(void)
{
}

//! FIXME: write doc.

void
SoGLNormalElement::init(SoState * state)
{
  inherited::init(state);
}

//! FIXME: write doc.

void
SoGLNormalElement::send(const int index) const
{
  assert(index >= 0 && index < this->numNormals);
  glNormal3fv(this->normals[index].getValue());
}
