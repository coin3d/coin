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
  \class SoGLProjectionMatrixElement Inventor/elements/SoGLProjectionMatrixElement.h
  \brief The SoGLProjectionMatrixElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLProjectionMatrixElement.h>

#include <Inventor/SbName.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <assert.h>

//$ BEGIN TEMPLATE ElementSource( SoGLProjectionMatrixElement )

/*!
  \var SoGLProjectionMatrixElement::classTypeId

  This is the static class type identifier for the
  SoGLProjectionMatrixElement class.
*/

SoType SoGLProjectionMatrixElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoGLProjectionMatrixElement::getClassTypeId(void)
{
  return SoGLProjectionMatrixElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoGLProjectionMatrixElement class.
*/
void *
SoGLProjectionMatrixElement::createInstance(void)
{
  return (void *) new SoGLProjectionMatrixElement;
}

/*!
  \var SoGLProjectionMatrixElement::classStackIndex

  This is the static state stack index for the
  SoGLProjectionMatrixElement class.
*/
int SoGLProjectionMatrixElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoGLProjectionMatrixElement class.
*/
int
SoGLProjectionMatrixElement::getClassStackIndex(void)
{
  return SoGLProjectionMatrixElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoGLProjectionMatrixElement class.
*/

void
SoGLProjectionMatrixElement::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoGLProjectionMatrixElement )
  assert(SoGLProjectionMatrixElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGLProjectionMatrixElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoGLProjectionMatrixElement",
    &SoGLProjectionMatrixElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoGLProjectionMatrixElement::classStackIndex =
      createStackIndex( SoGLProjectionMatrixElement::classTypeId );
  } else {
    SoGLProjectionMatrixElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
} // initClass()

/*!
  This static method cleans up static data for the
  SoGLProjectionMatrixElement class.
*/

void
SoGLProjectionMatrixElement::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoGLProjectionMatrixElement )
//$ END TEMPLATE CleanElementSource
} // cleanClass()

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLProjectionMatrixElement::createInstance( void )
*/

SoGLProjectionMatrixElement::SoGLProjectionMatrixElement( // protected
    void )
{
    setTypeId( SoGLProjectionMatrixElement::classTypeId );
    setStackIndex( SoGLProjectionMatrixElement::classStackIndex );
} // SoGLProjectionMatrixElement()

/*!
  The destructor.
*/

SoGLProjectionMatrixElement::~SoGLProjectionMatrixElement( // virtual
    void )
{
} // ~SoGLProjectionMatrixElement()

//! FIXME: write doc.

void
SoGLProjectionMatrixElement::pop(SoState * state,
				 const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
  ((SoGLProjectionMatrixElement*)prevTopElement)->updategl();
} // pop()

//! FIXME: write doc.

void
SoGLProjectionMatrixElement::setElt(const SbMatrix & matrix)
{
  inherited::setElt(matrix);
  this->updategl();
} // setElt()

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
} // updategl()

