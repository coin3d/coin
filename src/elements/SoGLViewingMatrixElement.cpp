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
  \class SoGLViewingMatrixElement Inventor/elements/SoGLViewingMatrixElement.h
  \brief The SoGLViewingMatrixElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLViewingMatrixElement.h>

#include <Inventor/SbName.h>
#if !defined(COIN_EXCLUDE_SOMODELMATRIXELEMENT)
#include <Inventor/elements/SoModelMatrixElement.h>
#endif // !COIN_EXCLUDE_SOMODELMATRIXELEMENT
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>

//$ BEGIN TEMPLATE ElementSource( SoGLViewingMatrixElement )

/*!
  \var SoGLViewingMatrixElement::classTypeId

  This is the static class type identifier for the
  SoGLViewingMatrixElement class.
*/

SoType SoGLViewingMatrixElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoGLViewingMatrixElement::getClassTypeId(void)
{
  return SoGLViewingMatrixElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoGLViewingMatrixElement class.
*/
void *
SoGLViewingMatrixElement::createInstance(void)
{
  return (void *) new SoGLViewingMatrixElement;
}

/*!
  \var SoGLViewingMatrixElement::classStackIndex

  This is the static state stack index for the
  SoGLViewingMatrixElement class.
*/
int SoGLViewingMatrixElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoGLViewingMatrixElement class.
*/
int
SoGLViewingMatrixElement::getClassStackIndex(void)
{
  return SoGLViewingMatrixElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoGLViewingMatrixElement class.
*/

void
SoGLViewingMatrixElement::initClass(
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoGLViewingMatrixElement )
  assert(SoGLViewingMatrixElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGLViewingMatrixElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoGLViewingMatrixElement",
    &SoGLViewingMatrixElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoGLViewingMatrixElement::classStackIndex =
      createStackIndex( SoGLViewingMatrixElement::classTypeId );
  } else {
    SoGLViewingMatrixElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the
  SoGLViewingMatrixElement class.
*/

void
SoGLViewingMatrixElement::cleanClass(
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoGLViewingMatrixElement )
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLViewingMatrixElement::createInstance( void )
*/

SoGLViewingMatrixElement::SoGLViewingMatrixElement(
    void )
  : state( NULL )
{
    setTypeId( SoGLViewingMatrixElement::classTypeId );
    setStackIndex( SoGLViewingMatrixElement::classStackIndex );
}

/*!
  The destructor.
*/

SoGLViewingMatrixElement::~SoGLViewingMatrixElement(
    void )
{
}

//! FIXME: write doc.

void
SoGLViewingMatrixElement::init(SoState * state)
{
#if 0 // too much debug output.. 981021 mortene.
  SoDebugError::postInfo("SoGLViewingMatrixElement::init",
			 "");
#endif // 0
  inherited::init( state );
  this->state = state;
  glLoadIdentity(); // maybe not strictly necessary?
}

//! FIXME: write doc.

void
SoGLViewingMatrixElement::push(SoState * state)
{
#if 0 // too much debug output.. 981021 mortene.
  SoDebugError::postInfo("SoGLViewingMatrixElement::push()", "");
#endif // 0
  inherited::push(state);
  SoGLViewingMatrixElement *elem = (SoGLViewingMatrixElement*)
    this->next;
  elem->state = state;
  //glPushMatrix();
}

//! FIXME: write doc.

void
SoGLViewingMatrixElement::pop(SoState * state,
			      const SoElement * prevTopElement)
{
#if 0 // too much debug output.. 981021 mortene.
  SoDebugError::postInfo("SoGLViewingMatrixElement::pop()", "");
#endif // 0
  //glPopMatrix();
  inherited::pop(state, prevTopElement);
}

//! FIXME: write doc.

uint32_t
SoGLViewingMatrixElement::getNodeId(SoState * const state)
{
  SoGLViewingMatrixElement *elem = (SoGLViewingMatrixElement*)
    SoElement::getConstElement(state, classStackIndex);
  return elem->getNodeId(state);
}

//! FIXME: write doc.

void
SoGLViewingMatrixElement::setElt(const SbMatrix & matrix)
{
#if 0 // too much debug output.. 981021 mortene.
  SoDebugError::postInfo("SoGLViewingMatrixElement::setElt()", "");
#endif // 0
  this->viewingMatrix = matrix;

#if !defined(COIN_EXCLUDE_SOMODELMATRIXELEMENT)
  SbBool isIdentity = FALSE;
  const SbMatrix &mat = SoModelMatrixElement::get(this->state, isIdentity);
  if (!isIdentity) this->viewingMatrix.multRight(mat);
#endif // !COIN_EXCLUDE_SOMODELMATRIXELEMENT
  this->updategl();
}

//! FIXME: write doc.

void
SoGLViewingMatrixElement::updategl()
{
#if 0 // debug
  SoDebugError::postInfo("SoGLViewingMatrixElement::updategl", "");
#endif // debug
  glLoadMatrixf((float*)this->viewingMatrix);
}

