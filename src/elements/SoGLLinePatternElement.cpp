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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

/*!
  \class SoGLLinePatternElement Inventor/elements/SoGLLinePatternElement.h
  \brief The SoGLLinePatternElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLLinePatternElement.h>

#include <Inventor/SbName.h>

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <assert.h>

//$ BEGIN TEMPLATE ElementSource( SoGLLinePatternElement )

/*!
  \var SoGLLinePatternElement::classTypeId

  This is the static class type identifier for the
  SoGLLinePatternElement class.
*/

SoType SoGLLinePatternElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoGLLinePatternElement::getClassTypeId(void)
{
  return SoGLLinePatternElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoGLLinePatternElement class.
*/
void *
SoGLLinePatternElement::createInstance(void)
{
  return (void *) new SoGLLinePatternElement;
}

/*!
  \var SoGLLinePatternElement::classStackIndex

  This is the static state stack index for the
  SoGLLinePatternElement class.
*/
int SoGLLinePatternElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoGLLinePatternElement class.
*/
int
SoGLLinePatternElement::getClassStackIndex(void)
{
  return SoGLLinePatternElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoGLLinePatternElement class.
*/

void
SoGLLinePatternElement::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoGLLinePatternElement )
  assert(SoGLLinePatternElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGLLinePatternElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoGLLinePatternElement",
    &SoGLLinePatternElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoGLLinePatternElement::classStackIndex =
      createStackIndex( SoGLLinePatternElement::classTypeId );
  } else {
    SoGLLinePatternElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
} // initClass()

/*!
  This static method cleans up static data for the
  SoGLLinePatternElement class.
*/

void
SoGLLinePatternElement::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoGLLinePatternElement )
//$ END TEMPLATE CleanElementSource
} // cleanClass()

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLLinePatternElement::createInstance( void )
*/

SoGLLinePatternElement::SoGLLinePatternElement( // protected
    void )
{
    setTypeId( SoGLLinePatternElement::classTypeId );
    setStackIndex( SoGLLinePatternElement::classStackIndex );
} // SoGLLinePatternElement()

/*!
  The destructor.
*/

SoGLLinePatternElement::~SoGLLinePatternElement( // virtual
    void )
{
} // ~SoGLLinePatternElement()

//! FIXME: write doc.

void
SoGLLinePatternElement::init( // virtual
    SoState * state )
{
  inherited::init( state );
  updategl();
} // init()

//! FIXME: write doc.

void
SoGLLinePatternElement::push(SoState * state)
{
  inherited::push(state);
  ((SoGLLinePatternElement*)this->next)->data = this->data;
} // push()

//! FIXME: write doc.

void
SoGLLinePatternElement::pop( // virtual
    SoState * state,
    const SoElement * prevTopElement )
{
  SoGLLinePatternElement *prev = (SoGLLinePatternElement*) prevTopElement;
  if (prev->data != this->data) prev->updategl();
  inherited::pop(state, prevTopElement);
} // pop()

//! FIXME: write doc.

void
SoGLLinePatternElement::setElt(int32_t pattern)
{
  if (pattern != this->data) {
    this->data = pattern;
    updategl();
  }
} // setElt()

//! FIXME: write doc.

void
SoGLLinePatternElement::updategl()
{
  //
  // FIXME: store flag to keep enable/disable state, pederb 990624
  //
  if (this->data == (int32_t) CONTINUOUS) {
    glDisable(GL_LINE_STIPPLE);
  }
  else {
    glLineStipple(1, (GLushort) this->data);
    glEnable(GL_LINE_STIPPLE);
  }
} // updategl()

