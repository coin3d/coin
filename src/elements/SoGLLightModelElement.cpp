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
  \class SoGLLightModelElement Inventor/elements/SoGLLightModelElement.h
  \brief The SoGLLightModelElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLLightModelElement.h>

#include <Inventor/SbName.h>

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <assert.h>

//$ BEGIN TEMPLATE ElementSource(SoGLLightModelElement)

/*!
  \var SoGLLightModelElement::classTypeId

  This is the static class type identifier for the
  SoGLLightModelElement class.
*/

SoType SoGLLightModelElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoGLLightModelElement::getClassTypeId(void)
{
  return SoGLLightModelElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoGLLightModelElement class.
*/
void *
SoGLLightModelElement::createInstance(void)
{
  return (void *) new SoGLLightModelElement;
}

/*!
  \var SoGLLightModelElement::classStackIndex

  This is the static state stack index for the
  SoGLLightModelElement class.
*/
int SoGLLightModelElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoGLLightModelElement class.
*/
int
SoGLLightModelElement::getClassStackIndex(void)
{
  return SoGLLightModelElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoGLLightModelElement class.
*/

void
SoGLLightModelElement::initClass(void)
{
//$ BEGIN TEMPLATE InitElementSource(SoGLLightModelElement)
  assert(SoGLLightModelElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGLLightModelElement::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SoGLLightModelElement",
                       &SoGLLightModelElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoGLLightModelElement::classStackIndex =
      createStackIndex(SoGLLightModelElement::classTypeId);
  }
  else {
    SoGLLightModelElement::classStackIndex = inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the
  SoGLLightModelElement class.
*/

void
SoGLLightModelElement::cleanClass(void)
{
//$ BEGIN TEMPLATE CleanElementSource(SoGLLightModelElement)
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLLightModelElement::createInstance(void)
*/

SoGLLightModelElement::SoGLLightModelElement()
{
  setTypeId(SoGLLightModelElement::classTypeId);
  setStackIndex(SoGLLightModelElement::classStackIndex);
}

/*!
  The destructor.
*/

SoGLLightModelElement::~SoGLLightModelElement()
{
}

//! FIXME: write doc.

void
SoGLLightModelElement::init(SoState * state)
{
  inherited::init(state);
  glEnable(GL_LIGHTING);
  this->current = PHONG;
}

//! FIXME: write doc.

void
SoGLLightModelElement::push(SoState * state)
{
  inherited::push(state);
  SoGLLightModelElement *top = (SoGLLightModelElement*)this->next;
  top->current = this->current;  
}

//! FIXME: write doc.

void
SoGLLightModelElement::pop(SoState * state,
			  const SoElement * prevTopElement)
{
  SoGLLightModelElement *prev = (SoGLLightModelElement*)prevTopElement;
  prev->current = this->current;
  inherited::pop(state, prevTopElement);
}

//! FIXME: write doc.

void
SoGLLightModelElement::setElt(int32_t val)
{
  // this is a lazy element
  this->data = val;
}

//! FIXME: write doc.

void
SoGLLightModelElement::evaluate() const
{
  if (this->data != this->current) {
    ((SoGLLightModelElement*)this)->current = (Model)this->data;
    ((SoGLLightModelElement*)this)->updategl();
  }
}

//! FIXME: write doc.

void
SoGLLightModelElement::forceSend(const Model model) const
{
  if (model != this->current) {
    ((SoGLLightModelElement*)this)->current = model;
    ((SoGLLightModelElement*)this)->updategl();
  }
}

//! FIXME: write doc.

void
SoGLLightModelElement::updategl()
{
  if (current == PHONG) glEnable(GL_LIGHTING);
  else glDisable(GL_LIGHTING);
}

const SoGLLightModelElement *
SoGLLightModelElement::getInstance(SoState *state)
{
  return (const SoGLLightModelElement *)
    SoElement::getConstElement(state, classStackIndex);
}
