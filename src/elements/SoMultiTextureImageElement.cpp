/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2009 by Kongsberg SIM.  All rights reserved.
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
  \class SoMultiTextureImageElement Inventor/elements/SoMultiTextureImageElement.h
  \brief The SoMultiTextureImageElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.

  \COIN_CLASS_EXTENSION

  \since Coin 2.2
*/

#include "coindefs.h"
#include "SbBasicP.h"

#include <Inventor/elements/SoMultiTextureImageElement.h>
#include <Inventor/elements/SoGLMultiTextureImageElement.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/misc/SoGLImage.h>
#include <Inventor/SbImage.h>
#include <cassert>

#define MAX_UNITS 16 // FIXME: make dynamic?????

class SoMultiTextureImageElementP {
public:
  SoMultiTextureImageElement::UnitData unitdata[MAX_UNITS];
};

SO_ELEMENT_CUSTOM_CONSTRUCTOR_SOURCE(SoMultiTextureImageElement);

#define PRIVATE(obj) obj->pimpl

SoMultiTextureImageElement::SoMultiTextureImageElement(void)
{
  PRIVATE(this) = new SoMultiTextureImageElementP;

  this->setTypeId(SoMultiTextureImageElement::classTypeId);
  this->setStackIndex(SoMultiTextureImageElement::classStackIndex);
}

/*!
  This static method initializes static data for the
  SoMultiTextureImageElement class.
*/

void
SoMultiTextureImageElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoMultiTextureImageElement, inherited);
}

/*!
  The destructor.
*/

SoMultiTextureImageElement::~SoMultiTextureImageElement()
{
  delete PRIVATE(this);
}

//! FIXME: write doc.

void
SoMultiTextureImageElement::init(SoState * state)
{
  inherited::init(state);
  for (int i = 0; i < MAX_UNITS; i++) {
    this->setDefaultValues(i);
  }
}

//
// private
//
void
SoMultiTextureImageElement::setDefaultValues(const int unit)
{
  assert(unit >=0 && unit < MAX_UNITS);

  UnitData & ud = PRIVATE(this)->unitdata[unit];
  ud.bytes = getDefault(ud.size, ud.numComponents);
  ud.wrapS = REPEAT;
  ud.wrapT = REPEAT;
  ud.wrapR = REPEAT;
  ud.model = MODULATE;
  ud.blendColor.setValue(0.0f, 0.0f, 0.0f);
  ud.nodeid = 0;
}

/*!
  Resets this element to its original values.
*/
void
SoMultiTextureImageElement::setDefault(SoState * const state, SoNode * const COIN_UNUSED_ARG(node), const int unit)
{
  SoMultiTextureImageElement * elem =
    coin_safe_cast<SoMultiTextureImageElement *>
    (
     state->getElement(classStackIndex)
     );

  if (elem) {
    elem->setDefaultValues(unit);
  }
}

//! FIXME: write doc.
void
SoMultiTextureImageElement::set(SoState * const state, SoNode * const node,
                                const int unit,
                                const SbVec2s & size, const int numComponents,
                                const unsigned char * bytes,
                                const Wrap wrapS,
                                const Wrap wrapT,
                                const Model model,
                                const SbColor & blendColor)
{
  SoMultiTextureImageElement * elem =
    coin_safe_cast<SoMultiTextureImageElement *>
    (
     state->getElement(classStackIndex)
     );
  if (elem) {
    elem->setElt(unit, node->getNodeId(), size, numComponents, bytes, wrapS, wrapT,
                 model, blendColor);
  }
}

/*!
  FIXME: write doc.

  \COIN_FUNCTION_EXTENSION
*/
void
SoMultiTextureImageElement::set(SoState * const state, SoNode * const node,
                                const int unit,
                                const SbVec3s & size, const int numComponents,
                                const unsigned char * bytes,
                                const Wrap wrapS,
                                const Wrap wrapT,
                                const Wrap wrapR,
                                const Model model,
                                const SbColor & blendColor)
{
  SoMultiTextureImageElement * elem = coin_safe_cast<SoMultiTextureImageElement *>
    (
     state->getElement(classStackIndex)
     );

  if (elem) {
    elem->setElt(unit, node->getNodeId(), size, numComponents, bytes, wrapS, wrapT, wrapR,
                 model, blendColor);
  }
}

//! FIXME: write doc.

const unsigned char *
SoMultiTextureImageElement::get(SoState * const state,
                                const int unit,
                                SbVec2s & size,
                                int & numComponents,
                                Wrap & wrapS,
                                Wrap & wrapT,
                                Model & model,
                                SbColor &blendColor)
{
  const SoMultiTextureImageElement * elem =
    coin_assert_cast<const SoMultiTextureImageElement *>
    (
     getConstElement(state, classStackIndex)
     );

  assert(unit >= 0 && unit < MAX_UNITS);
  const UnitData & ud = PRIVATE(elem)->unitdata[unit];

  wrapS = ud.wrapS;
  wrapT = ud.wrapT;
  model = ud.model;
  blendColor = ud.blendColor;

  return getImage(state, unit, size, numComponents);
}

/*!
  FIXME: write doc.

  \COIN_FUNCTION_EXTENSION

*/
const unsigned char *
SoMultiTextureImageElement::get(SoState * const state,
                                const int unit,
                                SbVec3s & size,
                                int & numComponents,
                                Wrap & wrapS,
                                Wrap & wrapT,
                                Wrap & wrapR,
                                Model & model,
                                SbColor &blendColor)
{
  const SoMultiTextureImageElement * elem =
    coin_assert_cast<const SoMultiTextureImageElement *>
    (
     getConstElement(state, classStackIndex)
     );

  assert(unit >= 0 && unit < MAX_UNITS);
  const UnitData & ud = PRIVATE(elem)->unitdata[unit];

  wrapS = ud.wrapS;
  wrapT = ud.wrapT;
  wrapR = ud.wrapR;
  model = ud.model;
  blendColor = ud.blendColor;

  return getImage(state, unit, size, numComponents);
}

/*!
  FIXME: write doc
*/
const unsigned char *
SoMultiTextureImageElement::getImage(SoState * const state,
                                     const int unit,
                                     SbVec2s &size,
                                     int & numComponents)
{
  const SoMultiTextureImageElement * elem =
    coin_assert_cast<const SoMultiTextureImageElement *>
    (
     getConstElement(state, classStackIndex)
     );

#if 0 // FIXME: update when SoGLMultiTextureImageElement is implemented
  if (elem->getTypeId().isDerivedFrom(SoGLMultiTextureImageElement::getClassTypeId())) {
    Model dummy1;
    SbColor dummy2;
    SoGLImage * image = SoGLMultiTextureImageElement::get(state, unit, dummy1, dummy2);
    unsigned char * bytes = NULL;
    size = SbVec2s(0,0);
    numComponents = 0;
    if (image && image->getImage()) {
      bytes = image->getImage()->getValue(size, numComponents);
    }
    return bytes;
  }
#endif
  assert(unit >= 0 && unit < MAX_UNITS);
  const UnitData & ud = PRIVATE(elem)->unitdata[unit];

  size.setValue(ud.size[0], ud.size[1]);
  numComponents = ud.numComponents;
  return ud.bytes;
}

/*!
  FIXME: write doc
*/
const unsigned char *
SoMultiTextureImageElement::getImage(SoState * const state,
                                     const int unit,
                                     SbVec3s & size,
                                     int & numComponents)
{
  const SoMultiTextureImageElement * elem =
    coin_assert_cast<const SoMultiTextureImageElement *>
    (
     getConstElement(state, classStackIndex)
     );

#if 0 // FIXME: update when SoGLMultiTextureImageElement is implemented
  if (elem->getTypeId().isDerivedFrom(SoGLMultiTextureImageElement::getClassTypeId())) {
    Model dummy1;
    SbColor dummy2;
    SoGLImage * image = SoGLMultiTextureImageElement::get(state, dummy1, dummy2);
    unsigned char * bytes = NULL;
    size = SbVec3s(0,0,0);
    numComponents = 0;
    if (image && image->getImage()) {
      bytes = image->getImage()->getValue(size, numComponents);
    }
    return bytes;
  }
#endif
  assert(unit >= 0 && unit < MAX_UNITS);
  const UnitData & ud = PRIVATE(elem)->unitdata[unit];

  size = ud.size;
  numComponents = ud.numComponents;
  return ud.bytes;
}


//! FIXME: write doc.

SbBool
SoMultiTextureImageElement::containsTransparency(SoState * const state)
{
  const SoMultiTextureImageElement * elem =
    coin_assert_cast<const SoMultiTextureImageElement *>
    (
     getConstElement(state, classStackIndex)
     );

  for (int i = 0; i < MAX_UNITS; i++) {
    if (elem->hasTransparency(i)) return TRUE;
  }
  return FALSE;
}

/*!
  Called by containsTransparency(). Returns \e TRUE if image data has
  transparency. Default method does a very poor job of detecting this,
  since it returns \e TRUE when the number of components are 2 or
  4. Override whenever it is important to know this
  (SoGLTextureImageElement overrides it to avoid transparency handling
  where possible).
*/
SbBool
SoMultiTextureImageElement::hasTransparency(const int unit) const
{
  const UnitData & ud = PRIVATE(this)->unitdata[unit];
  return (ud.numComponents==2 || ud.numComponents==4);
}

const SoMultiTextureImageElement::UnitData &
SoMultiTextureImageElement::getUnitData(const int unit) const
{
  assert(unit >= 0 && unit < MAX_UNITS);
  return PRIVATE(this)->unitdata[unit];
}


//! FIXME: write doc.

const unsigned char *
SoMultiTextureImageElement::getDefault(SbVec2s & size, int & numComponents)
{
  size.setValue(0,0);
  numComponents = 0;
  return NULL;
}

/*!
  FIXME: write doc.

*/
const unsigned char *
SoMultiTextureImageElement::getDefault(SbVec3s & size, int & numComponents)
{
  size.setValue(0,0,0);
  numComponents = 0;
  return NULL;
}

void
SoMultiTextureImageElement::push(SoState * COIN_UNUSED_ARG(state))
{
  const SoMultiTextureImageElement * prev =
    coin_assert_cast<SoMultiTextureImageElement *>
    (
     this->getNextInStack()
     );

  for (int i = 0; i < MAX_UNITS; i++) {
    PRIVATE(this)->unitdata[i] = PRIVATE(prev)->unitdata[i];
  }
}

SbBool
SoMultiTextureImageElement::matches(const SoElement * elem) const
{
  const SoMultiTextureImageElement * e =
    coin_assert_cast<const SoMultiTextureImageElement *>
    (elem);
  for (int i = 0; i < MAX_UNITS; i++) {
    if (PRIVATE(e)->unitdata[i].nodeid != PRIVATE(this)->unitdata[i].nodeid) {
      return FALSE;
    }
  }
  return TRUE;
}

SoElement *
SoMultiTextureImageElement::copyMatchInfo(void) const
{
  SoMultiTextureImageElement * elem = static_cast<SoMultiTextureImageElement *>
    (getTypeId().createInstance());
  for (int i = 0; i < MAX_UNITS; i++) {
    PRIVATE(elem)->unitdata[i].nodeid = PRIVATE(this)->unitdata[i].nodeid;
  }
  return elem;
}


//! FIXME: write doc.

void
SoMultiTextureImageElement::setElt(const int unit,
                                   const uint32_t nodeid,
                                   const SbVec2s &size, const int numComponents,
                                   const unsigned char * bytes,
                                   const Wrap wrapS,
                                   const Wrap wrapT,
                                   const Model model,
                                   const SbColor &blendColor)
{
  assert(unit >= 0 && unit < MAX_UNITS);
  UnitData & ud = PRIVATE(this)->unitdata[unit];

  ud.nodeid = nodeid;
  ud.size.setValue(size[0],size[1],1);
  ud.numComponents = numComponents;
  ud.bytes = bytes;
  ud.wrapS = wrapS;
  ud.wrapT = wrapT;
  ud.wrapR = REPEAT;
  ud.model = model;
  ud.blendColor = blendColor;
}

/*!
  FIXME: write doc.
*/
void
SoMultiTextureImageElement::setElt(const int unit,
                                   const uint32_t nodeid,
                                   const SbVec3s & size, const int numComponents,
                                   const unsigned char * bytes,
                                   const Wrap wrapS,
                                   const Wrap wrapT,
                                   const Wrap wrapR,
                                   const Model model,
                                   const SbColor &blendColor)
{
  assert(unit >= 0 && unit < MAX_UNITS);
  UnitData & ud = PRIVATE(this)->unitdata[unit];

  ud.nodeid = nodeid;
  ud.size = size;
  ud.numComponents = numComponents;
  ud.bytes = bytes;
  ud.wrapS = wrapS;
  ud.wrapT = wrapT;
  ud.wrapR = wrapR;
  ud.model = model;
  ud.blendColor = blendColor;
}

/*!
  FIXME: write doc.
*/
const SbColor &
SoMultiTextureImageElement::getBlendColor(SoState * const state, const int unit)
{
  const SoMultiTextureImageElement * elem =
    coin_assert_cast<const SoMultiTextureImageElement *>
    (
     getConstElement(state, classStackIndex)
     );
  assert(unit >= 0 && unit < MAX_UNITS);
  return PRIVATE(elem)->unitdata[unit].blendColor;
}

/*!
  FIXME: write doc.
*/
SoMultiTextureImageElement::Wrap
SoMultiTextureImageElement::getWrapS(SoState * const state, const int unit)
{
  const SoMultiTextureImageElement * elem =
    coin_assert_cast<const SoMultiTextureImageElement *>
    (
     getConstElement(state, classStackIndex)
     );
  assert(unit >= 0 && unit < MAX_UNITS);
  return PRIVATE(elem)->unitdata[unit].wrapT;
}

/*!
  FIXME: write doc.
*/
SoMultiTextureImageElement::Wrap
SoMultiTextureImageElement::getWrapT(SoState * const state, const int unit)
{
  const SoMultiTextureImageElement * elem =
    coin_assert_cast<const SoMultiTextureImageElement *>
    (
     getConstElement(state, classStackIndex)
     );
  assert(unit >= 0 && unit < MAX_UNITS);
  return PRIVATE(elem)->unitdata[unit].wrapS;
}

/*!
  FIXME: write doc.
*/
SoMultiTextureImageElement::Wrap
SoMultiTextureImageElement::getWrapR(SoState * const state, const int unit)
{
  const SoMultiTextureImageElement * elem =
    coin_assert_cast<const SoMultiTextureImageElement *>
    (
     getConstElement(state, classStackIndex)
     );
  assert(unit >= 0 && unit < MAX_UNITS);
  return PRIVATE(elem)->unitdata[unit].wrapR;
}

/*!
  FIXME: write doc.
*/
SoMultiTextureImageElement::Model
SoMultiTextureImageElement::getModel(SoState * const state, const int unit)
{
  const SoMultiTextureImageElement * elem =
    coin_assert_cast<const SoMultiTextureImageElement *>
    (
     getConstElement(state, classStackIndex)
     );
  assert(unit >= 0 && unit < MAX_UNITS);
  return PRIVATE(elem)->unitdata[unit].model;
}

#undef MAX_UNITS
#undef PRIVATE
