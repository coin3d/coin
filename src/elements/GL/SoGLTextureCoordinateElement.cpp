/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoGLTextureCoordinateElement Inventor/elements/SoGLTextureCoordinateElement.h
  \brief The SoGLTextureCoordinateElement class stores the current gltexture coordinates.
  \ingroup elements
*/

// *************************************************************************

#include <cassert>
#include "coindefs.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/elements/SoGLTextureCoordinateElement.h>

#include "coindefs.h"

#include <Inventor/elements/SoGLMultiTextureCoordinateElement.h>
#include <Inventor/elements/SoMultiTextureEnabledElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoTexture3EnabledElement.h>
#include <Inventor/elements/SoTextureEnabledElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/system/gl.h>

// *************************************************************************

class SoGLTextureCoordinateElement::PImpl {
public:
  // switch/case table for faster rendering.
  enum SendLookup {
    NONE,
    FUNCTION,
    TEXCOORD2,
    TEXCOORD3,
    TEXCOORD4
  };
  SendLookup sendlookup;

  void * texgenData;
  const SoGLMultiTextureCoordinateElement * multielem;
  const SbBool * multienabled;
  int multimax;
};

#define PRIVATE(obj) ((obj)->pimpl)

// *************************************************************************

SO_ELEMENT_CUSTOM_CONSTRUCTOR_SOURCE(SoGLTextureCoordinateElement);

// *************************************************************************

SoGLTextureCoordinateElement::SoGLTextureCoordinateElement(void)
{
  PRIVATE(this) = new SoGLTextureCoordinateElement::PImpl;

  this->setTypeId(SoGLTextureCoordinateElement::classTypeId);
  this->setStackIndex(SoGLTextureCoordinateElement::classStackIndex);
}

/*!
  This static method initializes static data for the
  SoGLTextureCoordinateElement class.
*/

void
SoGLTextureCoordinateElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLTextureCoordinateElement, inherited);
}

/*!
  The destructor.
*/

SoGLTextureCoordinateElement::~SoGLTextureCoordinateElement()
{
  delete PRIVATE(this);
}

//!  FIXME: write doc.

void
SoGLTextureCoordinateElement::init(SoState * state)
{
  inherited::init(state);
  this->texgenCB = NULL;
  PRIVATE(this)->texgenData = NULL;
  PRIVATE(this)->multielem = NULL;
  PRIVATE(this)->sendlookup = SoGLTextureCoordinateElement::PImpl::NONE;
}

// *************************************************************************

//!  FIXME: write doc.

void
SoGLTextureCoordinateElement::push(SoState * state)
{
  SoGLTextureCoordinateElement * prev = (SoGLTextureCoordinateElement*)this->getNextInStack();
  this->texgenCB = prev->texgenCB;
  PRIVATE(this)->texgenData = PRIVATE(prev)->texgenData;
  PRIVATE(this)->multielem = NULL;
  PRIVATE(this)->sendlookup = SoGLTextureCoordinateElement::PImpl::NONE;
  // capture previous element since we might or might not change the
  // GL state in set/pop
  prev->capture(state);
}

//!  FIXME: write doc.

void
SoGLTextureCoordinateElement::pop(SoState * state,
                                  const SoElement * prevTopElement)
{
  SoGLTextureCoordinateElement * prev = (SoGLTextureCoordinateElement*) prevTopElement;

  // only send to GL if something has changed
  if (this->texgenCB && !prev->texgenCB) {
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);
    glEnable(GL_TEXTURE_GEN_Q);
  }
  else if (!this->texgenCB && prev->texgenCB) {
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);
    glDisable(GL_TEXTURE_GEN_Q);
  }
  this->doCallback();
}

//!  FIXME: write doc.

void
SoGLTextureCoordinateElement::setTexGen(SoState * const state,
                                        SoNode * const node,
                                        SoTexCoordTexgenCB * const texgenFunc,
                                        void * const texgenData,
                                        SoTextureCoordinateFunctionCB * const func,
                                        void * const funcData)
{
  SoShapeStyleElement::setTextureFunction(state, texgenFunc && !func);

  SoTextureCoordinateElement::setFunction(state, node, func, funcData);

  SoGLTextureCoordinateElement *element = (SoGLTextureCoordinateElement *)
    SoReplacedElement::getElement(state, classStackIndex, node);
  if (element) {
    element->setElt(texgenFunc, texgenData);
  }
}

//!  FIXME: write doc.

SoGLTextureCoordinateElement::CoordType
SoGLTextureCoordinateElement::getType() const
{
  if (this->texgenCB) return NONE;
  return inherited::getType();
}

//!  FIXME: write doc.

const SoGLTextureCoordinateElement *
SoGLTextureCoordinateElement::getInstance(SoState * const state)
{
  return (SoGLTextureCoordinateElement*)
    SoElement::getConstElement(state, classStackIndex);
}

//!  FIXME: write doc.

void
SoGLTextureCoordinateElement::send(const int index) const
{
  switch (PRIVATE(this)->sendlookup) {
  case SoGLTextureCoordinateElement::PImpl::NONE:
    break;
  case SoGLTextureCoordinateElement::PImpl::FUNCTION:
    assert(0 && "should not happen");
    break;
  case SoGLTextureCoordinateElement::PImpl::TEXCOORD2:
    assert(index < this->numCoords);
    glTexCoord2fv(coords2[index].getValue());
    break;
  case SoGLTextureCoordinateElement::PImpl::TEXCOORD3:
    assert(index < this->numCoords);
    glTexCoord3fv(coords3[index].getValue());
    break;
  case SoGLTextureCoordinateElement::PImpl::TEXCOORD4:
    assert(index < this->numCoords);
    glTexCoord4fv(coords4[index].getValue());
    break;
  default:
    assert(0 && "should not happen");
    break;
  }

  const SoGLMultiTextureCoordinateElement * multielem =
    PRIVATE(this)->multielem;

  if (multielem) {
    // FIXME: make an optimized loop in SoGLMulteTextureCoordinateElement?
    const int num = PRIVATE(this)->multimax;
    const SbBool * enabled = PRIVATE(this)->multienabled;
    for (int i = 1; i <= num; i++) {
      if (enabled[i]) multielem->send(i, index);
    }
  }
}

//!  FIXME: write doc.

void
SoGLTextureCoordinateElement::send(const int index,
                                   const SbVec3f &c,
                                   const SbVec3f &n) const
{
  // Check for out-of-array-bounds errors.
  switch (PRIVATE(this)->sendlookup) {
  case SoGLTextureCoordinateElement::PImpl::TEXCOORD2:
  case SoGLTextureCoordinateElement::PImpl::TEXCOORD3:
  case SoGLTextureCoordinateElement::PImpl::TEXCOORD4:
    assert((index >= 0) && "negative array index, something is seriously wrong");
    if (index >= this->numCoords) {
      static SbBool first = TRUE;
      if (first) {
        first = FALSE;
        SoDebugError::post("SoGLTextureCoordinateElement::send",
                           "Index value %d into texture coordinate array "
                           "of size %d is out of bounds. This is usually an "
                           "indication that too few texture coordinates "
                           "were supplied in the scene graph.",
                           index, this->numCoords);
      }
      return;
    }
    break;

  default: break;
  }

  switch (PRIVATE(this)->sendlookup) {
  case SoGLTextureCoordinateElement::PImpl::NONE:
    break;
  case SoGLTextureCoordinateElement::PImpl::FUNCTION:
    assert(this->funcCB);
    glTexCoord4fv(this->funcCB(this->funcCBData, c, n).getValue());
    break;
  case SoGLTextureCoordinateElement::PImpl::TEXCOORD2:
    glTexCoord2fv(coords2[index].getValue());
    break;
  case SoGLTextureCoordinateElement::PImpl::TEXCOORD3:
    glTexCoord3fv(coords3[index].getValue());
    break;
  case SoGLTextureCoordinateElement::PImpl::TEXCOORD4:
    glTexCoord4fv(coords4[index].getValue());
    break;
  default:
    assert(0 && "should not happen");
    break;
  }
  const SoGLMultiTextureCoordinateElement * multielem =
    PRIVATE(this)->multielem;

  if (multielem) {
    // FIXME: make am optimized loop in SoGLMulteTextureCoordinateElement?
    const int num = PRIVATE(this)->multimax;
    for (int i = 1; i <= num; i++) {
      multielem->send(i, index, c, n);
    }
  }
}

/*!
  Called from SoTextureCoordinateBundle to initialize multi texturing.

  \internal
*/
void
SoGLTextureCoordinateElement::initMulti(SoState * state) const
{
  PRIVATE(this)->sendlookup = SoGLTextureCoordinateElement::PImpl::NONE;
  // init the sendloopup variable
  if (SoTextureEnabledElement::get(state) ||
      SoTexture3EnabledElement::get(state)) {
    switch (this->whatKind) {
    case SoTextureCoordinateElement::DEFAULT:
      assert(0 && "should not happen");
      break;
    case SoTextureCoordinateElement::FUNCTION:
      PRIVATE(this)->sendlookup = SoGLTextureCoordinateElement::PImpl::FUNCTION;
      break;
    case SoTextureCoordinateElement::NONE:
      break;
    case SoTextureCoordinateElement::EXPLICIT:
      {
        switch (this->coordsDimension) {
        case 2:
          PRIVATE(this)->sendlookup = SoGLTextureCoordinateElement::PImpl::TEXCOORD2;
          break;
        case 3:
          PRIVATE(this)->sendlookup = SoGLTextureCoordinateElement::PImpl::TEXCOORD3;
          break;
        case 4:
          PRIVATE(this)->sendlookup = SoGLTextureCoordinateElement::PImpl::TEXCOORD4;
          break;
        default:
          assert(0 && "should not happen");
          break;
        }
      }
      break;
    default:
      assert(0 && "should not happen");
      break;
    }
  }

  PRIVATE(this)->multielem = NULL;
  PRIVATE(this)->multienabled = SoMultiTextureEnabledElement::getEnabledUnits(state,
                                                                              PRIVATE(this)->multimax);
  if (PRIVATE(this)->multienabled && PRIVATE(this)->multimax >= 1) {
    PRIVATE(this)->multielem = SoGLMultiTextureCoordinateElement::getInstance(state);
    PRIVATE(this)->multielem->initRender(PRIVATE(this)->multienabled, PRIVATE(this)->multimax);
  }
}


//!  FIXME: write doc.

void
SoGLTextureCoordinateElement::setElt(SoTexCoordTexgenCB * func,
                                     void *data)
{
  if (func && !this->texgenCB) {
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);
    glEnable(GL_TEXTURE_GEN_Q);
  }
  else if (!func && this->texgenCB) {
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);
    glDisable(GL_TEXTURE_GEN_Q);
  }
  if (func) {
    this->whatKind = FUNCTION;
  }
  this->texgenCB = func;
  PRIVATE(this)->texgenData = data;
  this->doCallback();
}

void
SoGLTextureCoordinateElement::doCallback() const
{
  if (this->texgenCB) {
    this->texgenCB(PRIVATE(this)->texgenData);
  }
}

#undef PRIVATE

