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
  \class SoGLSpecularColorElement Inventor/elements/SoGLSpecularColorElement.h
  \brief The SoGLSpecularColorElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLSpecularColorElement.h>

#include <Inventor/SbColor.h>
#include <Inventor/SbName.h>

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <assert.h>

//$ BEGIN TEMPLATE ElementSource( SoGLSpecularColorElement )

/*!
  \var SoGLSpecularColorElement::classTypeId

  This is the static class type identifier for the
  SoGLSpecularColorElement class.
*/

SoType SoGLSpecularColorElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoGLSpecularColorElement::getClassTypeId(void)
{
  return SoGLSpecularColorElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoGLSpecularColorElement class.
*/
void *
SoGLSpecularColorElement::createInstance(void)
{
  return (void *) new SoGLSpecularColorElement;
}

/*!
  \var SoGLSpecularColorElement::classStackIndex

  This is the static state stack index for the
  SoGLSpecularColorElement class.
*/
int SoGLSpecularColorElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoGLSpecularColorElement class.
*/
int
SoGLSpecularColorElement::getClassStackIndex(void)
{
  return SoGLSpecularColorElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoGLSpecularColorElement class.
*/

void
SoGLSpecularColorElement::initClass()
{
//$ BEGIN TEMPLATE InitElementSource( SoGLSpecularColorElement )
  assert(SoGLSpecularColorElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGLSpecularColorElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoGLSpecularColorElement",
    &SoGLSpecularColorElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoGLSpecularColorElement::classStackIndex =
      createStackIndex( SoGLSpecularColorElement::classTypeId );
  } else {
    SoGLSpecularColorElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the SoGLSpecularColorElement
  class.
*/

void 
SoGLSpecularColorElement::cleanClass()
{
//$ BEGIN TEMPLATE CleanElementSource( SoGLSpecularColorElement )
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLSpecularColorElement::createInstance( void )
*/

SoGLSpecularColorElement::SoGLSpecularColorElement()
{
  setTypeId(classTypeId);
  setStackIndex(classStackIndex);
}

/*!
  The destructor.
*/

SoGLSpecularColorElement::~SoGLSpecularColorElement()
{
}

//! FIXME: write doc.

void 
SoGLSpecularColorElement::init(SoState *state)
{
  inherited::init(state);
  this->current.setValue(0.0f, 0.0f, 0.0f);
  this->updategl();
}

//! FIXME: write doc.

void
SoGLSpecularColorElement::push(SoState *state)
{
  inherited::push(state);
  SoGLSpecularColorElement * const element =
    (SoGLSpecularColorElement *)this->next;
  element->current = this->current;
}

//! FIXME: write doc.

void 
SoGLSpecularColorElement::pop(SoState *state, const SoElement *prevTopElement)
{
  inherited::pop(state, prevTopElement);
  ((SoGLSpecularColorElement*)prevTopElement)->current = this->current;
}

//! FIXME: write doc.

void 
SoGLSpecularColorElement::send(const int index)
{
  int realindex = index < this->numColors ? index : this->numColors - 1;
  if (colors && colors[realindex] != this->current) {
    this->current = colors[realindex];
    this->updategl();
  }
}

//! FIXME: write doc.

void 
SoGLSpecularColorElement::setElt(const int32_t numColors, 
				 const SbColor * const colors)
{
  inherited::setElt(numColors, colors);
}

//! FIXME: write doc.

void 
SoGLSpecularColorElement::updategl()
{
  float col[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  this->current.getValue(col[0], col[1], col[2]);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, col);
}

