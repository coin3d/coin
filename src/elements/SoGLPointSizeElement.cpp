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
  \class SoGLPointSizeElement Inventor/elements/SoGLPointSizeElement.h
  \brief The SoGLPointSizeElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLPointSizeElement.h>

#include <Inventor/SbName.h>

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <assert.h>

float SoGLPointSizeElement::sizerange[2] = {-1.0f, -1.0f};

//$ BEGIN TEMPLATE ElementSource(SoGLPointSizeElement)

/*!
  \var SoGLPointSizeElement::classTypeId

  This is the static class type identifier for the
  SoGLPointSizeElement class.
*/

SoType SoGLPointSizeElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoGLPointSizeElement::getClassTypeId(void)
{
  return SoGLPointSizeElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoGLPointSizeElement class.
*/
void *
SoGLPointSizeElement::createInstance(void)
{
  return (void *) new SoGLPointSizeElement;
}

/*!
  \var SoGLPointSizeElement::classStackIndex

  This is the static state stack index for the
  SoGLPointSizeElement class.
*/
int SoGLPointSizeElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoGLPointSizeElement class.
*/
int
SoGLPointSizeElement::getClassStackIndex(void)
{
  return SoGLPointSizeElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the SoGLPointSizeElement
  class.
*/

void
SoGLPointSizeElement::initClass(void)
{
//$ BEGIN TEMPLATE InitElementSource(SoGLPointSizeElement)
  assert(SoGLPointSizeElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGLPointSizeElement::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SoGLPointSizeElement",
                       &SoGLPointSizeElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoGLPointSizeElement::classStackIndex =
      createStackIndex(SoGLPointSizeElement::classTypeId);
  }
  else {
    SoGLPointSizeElement::classStackIndex = inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLPointSizeElement::createInstance(void)
*/

SoGLPointSizeElement::SoGLPointSizeElement()
{
  setTypeId(SoGLPointSizeElement::classTypeId);
  setStackIndex(SoGLPointSizeElement::classStackIndex);
}

/*!
  The destructor.
*/

SoGLPointSizeElement::~SoGLPointSizeElement(void)
{
}

//! FIXME: write doc.

void
SoGLPointSizeElement::init(SoState * state)
{
  inherited::init(state);
  this->current = -1.0f;
}

//! FIXME: write doc.

void
SoGLPointSizeElement::push(SoState * state)
{
  inherited::push(state);
  SoGLPointSizeElement *top = (SoGLPointSizeElement*)this->next;
  top->current = this->current;  
}

//! FIXME: write doc.

void
SoGLPointSizeElement::pop(SoState * state,
			  const SoElement * prevTopElement)
{
  SoGLPointSizeElement *prev = (SoGLPointSizeElement*)prevTopElement;
  prev->current = this->current;
  inherited::pop(state, prevTopElement);
}

//! FIXME: write doc.

void
SoGLPointSizeElement::setElt(float val)
{
  inherited::setElt(val);
  // this is a lazy element, do nothing
}

//! FIXME: write doc.

void
SoGLPointSizeElement::evaluate() const
{
  if (this->data != current) {
    // cast away constness
    SoGLPointSizeElement *elem = (SoGLPointSizeElement*)this;
    elem->current = this->data;
    elem->updategl();
  }
}

//! FIXME: write doc.

void
SoGLPointSizeElement::updategl()
{
  if (SoGLPointSizeElement::sizerange[0] < 0.0f) {
    GLfloat vals[2];
    glGetFloatv(GL_POINT_SIZE_RANGE, vals);
    SoGLPointSizeElement::sizerange[0] = vals[0];
    SoGLPointSizeElement::sizerange[1] = vals[1];
  }
  
  float useval = this->current;
  // FIXME: spit out a warning? 990314 mortene.
  if (useval < SoGLPointSizeElement::sizerange[0])
    useval = SoGLPointSizeElement::sizerange[0];
  if (useval > SoGLPointSizeElement::sizerange[1])
    useval = SoGLPointSizeElement::sizerange[1];
  
  glPointSize(useval);
}

