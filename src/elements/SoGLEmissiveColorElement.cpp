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
  \class SoGLEmissiveColorElement Inventor/elements/SoGLEmissiveColorElement.h
  \brief The SoGLEmissiveColorElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLEmissiveColorElement.h>

#include <Inventor/SbColor.h>
#include <Inventor/SbName.h>

#include <GL/gl.h>
#include <assert.h>

//$ BEGIN TEMPLATE ElementSource( SoGLEmissiveColorElement )

/*!
  \var SoGLEmissiveColorElement::classTypeId

  This is the static class type identifier for the
  SoGLEmissiveColorElement class.
*/

SoType SoGLEmissiveColorElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoGLEmissiveColorElement::getClassTypeId(void)
{
  return SoGLEmissiveColorElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoGLEmissiveColorElement class.
*/
void *
SoGLEmissiveColorElement::createInstance(void)
{
  return (void *) new SoGLEmissiveColorElement;
}

/*!
  \var SoGLEmissiveColorElement::classStackIndex

  This is the static state stack index for the
  SoGLEmissiveColorElement class.
*/
int SoGLEmissiveColorElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoGLEmissiveColorElement class.
*/
int
SoGLEmissiveColorElement::getClassStackIndex(void)
{
  return SoGLEmissiveColorElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoGLEmissiveColorElement class.
*/

void
SoGLEmissiveColorElement::initClass()
{
//$ BEGIN TEMPLATE InitElementSource( SoGLEmissiveColorElement )
  assert(SoGLEmissiveColorElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGLEmissiveColorElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoGLEmissiveColorElement",
    &SoGLEmissiveColorElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoGLEmissiveColorElement::classStackIndex =
      createStackIndex( SoGLEmissiveColorElement::classTypeId );
  } else {
    SoGLEmissiveColorElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
} // initClass()

/*!
  This static method cleans up static data for the SoGLEmissiveColorElement
  class.
*/

void 
SoGLEmissiveColorElement::cleanClass()
{
//$ BEGIN TEMPLATE CleanElementSource( SoGLEmissiveColorElement )
//$ END TEMPLATE CleanElementSource
} // cleanClass()

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLEmissiveColorElement::createInstance( void )
*/

SoGLEmissiveColorElement::SoGLEmissiveColorElement()
{
  setTypeId(classTypeId);
  setStackIndex(classStackIndex);
}

/*!
  The destructor.
*/

SoGLEmissiveColorElement::~SoGLEmissiveColorElement()
{
}

//! FIXME: write doc.

void 
SoGLEmissiveColorElement::init(SoState *state)
{
  inherited::init(state);
  this->current.setValue(0.0f, 0.0f, 0.0f);
  this->updategl();
}

//! FIXME: write doc.

void
SoGLEmissiveColorElement::push(SoState *state)
{
  inherited::push(state);
  SoGLEmissiveColorElement * const element =
    (SoGLEmissiveColorElement *)this->next;
  element->current = this->current;
}

//! FIXME: write doc.

void 
SoGLEmissiveColorElement::pop(SoState *state, const SoElement *prevTopElement)
{
  inherited::pop(state, prevTopElement);
  ((SoGLEmissiveColorElement*)prevTopElement)->current = this->current;
}

//! FIXME: write doc.

void 
SoGLEmissiveColorElement::send(const int index)
{
  int realindex = index < this->numColors ? index : this->numColors - 1;
  if (colors && colors[realindex] != this->current) {
    this->current = colors[realindex];
    this->updategl();
  }
}

//! FIXME: write doc.

void 
SoGLEmissiveColorElement::setElt(const int32_t numColors, 
				 const SbColor * const colors)
{
  inherited::setElt(numColors, colors);
}

//! FIXME: write doc.

void 
SoGLEmissiveColorElement::updategl()
{
  float col[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  this->current.getValue(col[0], col[1], col[2]);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, col);
}

