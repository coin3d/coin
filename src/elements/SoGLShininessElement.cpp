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
  \class SoGLShininessElement Inventor/elements/SoGLShininessElement.h
  \brief The SoGLShininessElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLShininessElement.h>

#include <Inventor/SbColor.h>
#include <Inventor/SbName.h>

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <assert.h>

//$ BEGIN TEMPLATE ElementSource(SoGLShininessElement)

/*!
  \var SoGLShininessElement::classTypeId

  This is the static class type identifier for the
  SoGLShininessElement class.
*/

SoType SoGLShininessElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoGLShininessElement::getClassTypeId(void)
{
  return SoGLShininessElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoGLShininessElement class.
*/
void *
SoGLShininessElement::createInstance(void)
{
  return (void *) new SoGLShininessElement;
}

/*!
  \var SoGLShininessElement::classStackIndex

  This is the static state stack index for the
  SoGLShininessElement class.
*/
int SoGLShininessElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoGLShininessElement class.
*/
int
SoGLShininessElement::getClassStackIndex(void)
{
  return SoGLShininessElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoGLShininessElement class.
*/

void
SoGLShininessElement::initClass()
{
//$ BEGIN TEMPLATE InitElementSource(SoGLShininessElement)
  assert(SoGLShininessElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGLShininessElement::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SoGLShininessElement",
                       &SoGLShininessElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoGLShininessElement::classStackIndex =
      createStackIndex(SoGLShininessElement::classTypeId);
  }
  else {
    SoGLShininessElement::classStackIndex = inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLShininessElement::createInstance(void)
*/

SoGLShininessElement::SoGLShininessElement()
{
  setTypeId(classTypeId);
  setStackIndex(classStackIndex);
}

/*!
  The destructor.
*/

SoGLShininessElement::~SoGLShininessElement()
{
}

//! FIXME: write doc.

void 
SoGLShininessElement::init(SoState *state)
{
  inherited::init(state);
  this->current = 0.2f;
  this->updategl();
}

//! FIXME: write doc.

void
SoGLShininessElement::push(SoState *state)
{
  inherited::push(state);
  SoGLShininessElement * const element =
    (SoGLShininessElement *)this->next;
  element->current = this->current;
}

//! FIXME: write doc.

void 
SoGLShininessElement::pop(SoState *state, const SoElement *prevTopElement)
{
  inherited::pop(state, prevTopElement);
  ((SoGLShininessElement*)prevTopElement)->current = this->current;
}

//! FIXME: write doc.

void 
SoGLShininessElement::send(const int index)
{
  int realindex = index < this->numValues ? index : this->numValues - 1;
  if (this->values && this->values[realindex] != this->current) {
    this->current = this->values[realindex];
    this->updategl();
  }
}

//! FIXME: write doc.

void 
SoGLShininessElement::setElt(const int32_t numValues, 
			     const float * const values)
{
  inherited::setElt(numValues, values);
}

//! FIXME: write doc.

void 
SoGLShininessElement::updategl()
{
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 
	      this->current*128);
}

