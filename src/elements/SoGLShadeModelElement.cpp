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
  \class SoGLShadeModelElement Inventor/elements/SoGLShadeModelElement.h
  \brief The SoGLShadeModelElement class is for GL optimization only.

  It keeps track of the current shade model (flat or smooth) and
  evaluates lazily when this should be changed. Flat-shaded
  triangles are much faster to draw (at least in SW) than smooth
  shaded triangles.
*/

#include <Inventor/elements/SoGLShadeModelElement.h>

#include <coindefs.h> // COIN_STUB()

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <assert.h>

SO_ELEMENT_SOURCE(SoGLShadeModelElement);

/*!
  This static method initializes static data for the
  SoGLShadeModelElement class.
*/

void
SoGLShadeModelElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoGLShadeModelElement, inherited);
}

/*!
  The destructor.
*/

SoGLShadeModelElement::~SoGLShadeModelElement()
{
}

//! FIXME: write doc.

void
SoGLShadeModelElement::init(SoState * state)
{
  inherited::init(state);
  this->matPerVertex = FALSE;
  this->normPerVertex = TRUE;
  this->glflat = TRUE;
  this->updategl(FALSE);
}

//! FIXME: write doc.

void
SoGLShadeModelElement::push(SoState * state)
{
  inherited::push(state);
  ((SoGLShadeModelElement*)this->next)->glflat = this->glflat;
  ((SoGLShadeModelElement*)this->next)->matPerVertex = this->matPerVertex;
  ((SoGLShadeModelElement*)this->next)->normPerVertex = this->normPerVertex;
}

//! FIXME: write doc.

void
SoGLShadeModelElement::pop(SoState * state,
                           const SoElement * prevTopElement)
{
  ((SoGLShadeModelElement*)prevTopElement)->glflat = this->glflat;
  inherited::pop(state, prevTopElement);
}

//! FIXME: write doc.

SbBool
SoGLShadeModelElement::matches(const SoElement * /* element */) const
{
  COIN_STUB();
  return FALSE;
}

//! FIXME: write doc.

SoElement *
SoGLShadeModelElement::copyMatchInfo() const
{
  COIN_STUB();
  return NULL;
}

//! FIXME: write doc.

void
SoGLShadeModelElement::setMaterial(SoState * const state,
                                   const SbBool perVertex)
{
  SoGLShadeModelElement * e = (SoGLShadeModelElement *)
    inherited::getElement(state, SoGLShadeModelElement::classStackIndex);
  e->matPerVertex = perVertex;
}

//! FIXME: write doc.

void
SoGLShadeModelElement::setNormal(SoState * const state,
                                 const SbBool perVertex)
{
  SoGLShadeModelElement * e = (SoGLShadeModelElement *)
    inherited::getElement(state, SoGLShadeModelElement::classStackIndex);
  e->matPerVertex = perVertex;
}

//! FIXME: write doc.

void
SoGLShadeModelElement::print(FILE *) const
{
}

//! FIXME: write doc.

void
SoGLShadeModelElement::evaluate() const
{
  SbBool flat = !(this->matPerVertex || this->normPerVertex);
  ((SoGLShadeModelElement*)this)->updategl(flat);
}

//! FIXME: write doc.

void
SoGLShadeModelElement::forceSend(const SbBool flat) const
{
  ((SoGLShadeModelElement*)this)->updategl(flat);
}

//! FIXME: write doc.

void
SoGLShadeModelElement::updategl(const SbBool flat)
{
  if (flat != this->glflat) {
    this->glflat = flat;
    if (flat) glShadeModel(GL_FLAT);
    else glShadeModel(GL_SMOOTH);
  }
}
