/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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
  \class SoGLDrawStyleElement Inventor/elements/SoGLDrawStyleElement.h
  \brief The SoGLDrawStyleElement updates the current draw style in OpenGL.
*/

#include <Inventor/elements/SoGLDrawStyleElement.h>


#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <assert.h>

SO_ELEMENT_SOURCE(SoGLDrawStyleElement);

/*!
  This static method initializes static data for the
  SoGLDrawStyleElement class.
*/

void
SoGLDrawStyleElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLDrawStyleElement, inherited);
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLDrawStyleElement::createInstance(void)
*/

SoGLDrawStyleElement::SoGLDrawStyleElement(void)
{
    setTypeId(SoGLDrawStyleElement::classTypeId);
    setStackIndex(SoGLDrawStyleElement::classStackIndex);
}

/*!
  The destructor.
*/

SoGLDrawStyleElement::~SoGLDrawStyleElement(void)
{
}

//! FIXME: write doc.

void
SoGLDrawStyleElement::init(SoState * state)
{
  inherited::init(state);
  this->updategl();
}

//! FIXME: write doc.

void
SoGLDrawStyleElement::push(SoState * state)
{
  inherited::push(state);
  // copy data to avoid unessesary GL calls
  ((SoGLDrawStyleElement*)this->next)->data = this->data;
}

//! FIXME: write doc.

void
SoGLDrawStyleElement::pop(SoState * state,
                          const SoElement * prevTopElement)
{
  SoGLDrawStyleElement *prev = (SoGLDrawStyleElement*) prevTopElement;
  if (prev->data != this->data) prev->updategl();
  inherited::pop(state, prevTopElement);
}

//! FIXME: write doc.

void
SoGLDrawStyleElement::setElt(int32_t style)
{
  if (style != this->data) {
    this->data = (int32_t)style;
    this->updategl();
  }
}

//! FIXME: write doc.

void
SoGLDrawStyleElement::updategl()
{
  switch ((Style)this->data) {
  case SoDrawStyleElement::FILLED:
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    break;
  case SoDrawStyleElement::LINES:
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    break;
  case SoDrawStyleElement::POINTS:
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    break;
  case SoDrawStyleElement::INVISIBLE:
    // handled in SoShape::shouldGLRender()
    break;
  default:
    assert(0 && "FIXME: handle unsupported switch case");
    break;
  }
}
