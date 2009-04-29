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
  \class SoTextureCombineElement Inventor/elements/SoTextureCombineElement.h
  \brief The SoTextureCombineElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.

  \COIN_CLASS_EXTENSION

  \since Coin 2.3
*/

#include <Inventor/elements/SoTextureCombineElement.h>

#include <cassert>
#include <cstring>

#include <Inventor/nodes/SoNode.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/system/gl.h>

#include "coindefs.h"
#include "SbBasicP.h"

#ifndef COIN_WORKAROUND_NO_USING_STD_FUNCS
using std::memcpy;
#endif // !COIN_WORKAROUND_NO_USING_STD_FUNCS

#define MAX_UNITS 16 // FIXME: make dynamic?????

class SoTextureCombineElementP {
public:
  SoTextureCombineElement::UnitData unitdata[MAX_UNITS];
};

SO_ELEMENT_CUSTOM_CONSTRUCTOR_SOURCE(SoTextureCombineElement);

#define PRIVATE(obj) obj->pimpl

SoTextureCombineElement::SoTextureCombineElement(void)
{
  PRIVATE(this) = new SoTextureCombineElementP;

  this->setTypeId(SoTextureCombineElement::classTypeId);
  this->setStackIndex(SoTextureCombineElement::classStackIndex);
}

/*!
  This static method initializes static data for the
  SoTextureCombineElement class.
*/

void
SoTextureCombineElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoTextureCombineElement, inherited);
}

/*!
  The destructor.
*/

SoTextureCombineElement::~SoTextureCombineElement()
{
  delete PRIVATE(this);
}

//! FIXME: write doc.

void
SoTextureCombineElement::init(SoState * state)
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
SoTextureCombineElement::setDefaultValues(const int unit)
{
  assert(unit >=0 && unit < MAX_UNITS);

  UnitData & ud = PRIVATE(this)->unitdata[unit];
  ud.nodeid = 0;
}

//! FIXME: write doc.
void
SoTextureCombineElement::set(SoState * const state, SoNode * const node,
                             const int unit,
                             const Operation rgboperation,
                             const Operation alphaoperation,
                             const Source * rgbsource,
                             const Source * alphasource,
                             const Operand * rgboperand,
                             const Operand * alphaoperand,
                             const SbColor4f & constantcolor,
                             const float rgbscale,
                             const float alphascale)
{
  SoTextureCombineElement * elem = coin_safe_cast<SoTextureCombineElement *>
    (
     state->getElement(classStackIndex)
     );
  if (elem) {
    elem->setElt(unit, node->getNodeId(),
                 rgboperation,
                 alphaoperation,
                 rgbsource,
                 alphasource,
                 rgboperand,
                 alphaoperand,
                 constantcolor,
                 rgbscale,
                 alphascale);
  }
}


//! FIXME: write doc.

void
SoTextureCombineElement::get(SoState * const state,
                             const int unit,
                             Operation & rgboperation,
                             Operation & alphaoperation,
                             Source * rgbsource,
                             Source * alphasource,
                             Operand * rgboperand,
                             Operand * alphaoperand,
                             SbColor4f & constantcolor,
                             float & rgbscale,
                             float & alphascale)
{
  const SoTextureCombineElement * elem =
    coin_assert_cast<const SoTextureCombineElement *>
    (
     getConstElement(state, classStackIndex)
     );

  assert(unit >= 0 && unit < MAX_UNITS);
  const UnitData & ud = PRIVATE(elem)->unitdata[unit];

  rgboperation = ud.rgboperation;
  alphaoperation = ud.alphaoperation;
  memcpy(rgbsource, ud.rgbsource, 3*sizeof(Source));
  memcpy(alphasource, ud.alphasource, 3*sizeof(Source));
  memcpy(rgboperand, ud.rgboperand, 3*sizeof(Operand));
  memcpy(alphaoperand, ud.alphaoperand, 3*sizeof(Operand));
  constantcolor = ud.constantcolor;
  rgbscale = ud.rgbscale;
  alphascale = ud.alphascale;
}


SbBool
SoTextureCombineElement::isDefault(SoState * const state,
                                   const int unit)
{
  const SoTextureCombineElement * elem =
    coin_assert_cast<const SoTextureCombineElement *>
    (
     getConstElement(state, classStackIndex)
     );

  assert(unit >= 0 && unit < MAX_UNITS);

  return PRIVATE(elem)->unitdata[unit].nodeid == 0;
}


const SoTextureCombineElement::UnitData &
SoTextureCombineElement::getUnitData(const int unit) const
{
  assert(unit >= 0 && unit < MAX_UNITS);
  return PRIVATE(this)->unitdata[unit];
}

void
SoTextureCombineElement::push(SoState * COIN_UNUSED_ARG(state))
{
  const SoTextureCombineElement * prev = coin_assert_cast<SoTextureCombineElement *>
    (
     this->getNextInStack()
     );

  for (int i = 0; i < MAX_UNITS; i++) {
    PRIVATE(this)->unitdata[i] = PRIVATE(prev)->unitdata[i];
  }
}

SbBool
SoTextureCombineElement::matches(const SoElement * elem) const
{
  const SoTextureCombineElement * e =
    coin_assert_cast<const SoTextureCombineElement *>(elem);
  for (int i = 0; i < MAX_UNITS; i++) {
    if (PRIVATE(e)->unitdata[i].nodeid != PRIVATE(this)->unitdata[i].nodeid) {
      return FALSE;
    }
  }
  return TRUE;
}

SoElement *
SoTextureCombineElement::copyMatchInfo(void) const
{
  SoTextureCombineElement * elem =
    static_cast<SoTextureCombineElement *>(getTypeId().createInstance());
  for (int i = 0; i < MAX_UNITS; i++) {
    PRIVATE(elem)->unitdata[i].nodeid = PRIVATE(this)->unitdata[i].nodeid;
  }
  return elem;
}


//! FIXME: write doc.

void
SoTextureCombineElement::setElt(const int unit,
                                const uint32_t nodeid,
                                const Operation rgboperation,
                                const Operation alphaoperation,
                                const Source * rgbsource,
                                const Source * alphasource,
                                const Operand * rgboperand,
                                const Operand * alphaoperand,
                                const SbColor4f & constantcolor,
                                const float rgbscale,
                                const float alphascale)

{
  assert(unit >= 0 && unit < MAX_UNITS);
  UnitData & ud = PRIVATE(this)->unitdata[unit];

  ud.nodeid = nodeid;
  ud.rgboperation = rgboperation;
  ud.alphaoperation = alphaoperation;
  memcpy(ud.rgbsource, rgbsource, 3*sizeof(Source));
  memcpy(ud.alphasource, alphasource, 3*sizeof(Source));
  memcpy(ud.rgboperand, rgboperand, 3*sizeof(Operand));
  memcpy(ud.alphaoperand, alphaoperand, 3*sizeof(Operand));
  ud.constantcolor = constantcolor;
  ud.rgbscale = rgbscale;
  ud.alphascale = alphascale;
}

void
SoTextureCombineElement::apply(SoState * state, const int unit)
{
  const SoTextureCombineElement * elem =
    coin_assert_cast<const SoTextureCombineElement *>
    (
     getConstElement(state, classStackIndex)
     );

  assert(unit >= 0 && unit < MAX_UNITS);
  const UnitData & ud = PRIVATE(elem)->unitdata[unit];

  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
  glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, static_cast<GLenum>(ud.rgboperation));
  glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, static_cast<GLenum>(ud.alphaoperation));

  glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, static_cast<GLenum>(ud.rgbsource[0]));
  glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, static_cast<GLenum>(ud.rgbsource[1]));
  glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB, static_cast<GLenum>(ud.rgbsource[2]));

  glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, static_cast<GLenum>(ud.alphasource[0]));
  glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA, static_cast<GLenum>(ud.alphasource[1]));
  glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_ALPHA, static_cast<GLenum>(ud.alphasource[2]));

  glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, static_cast<GLenum>(ud.rgboperand[0]));
  glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, static_cast<GLenum>(ud.rgboperand[1]));
  glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB, static_cast<GLenum>(ud.rgboperand[2]));

  glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, static_cast<GLenum>(ud.alphaoperand[0]));
  glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, static_cast<GLenum>(ud.alphaoperand[1]));
  glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_ALPHA, static_cast<GLenum>(ud.alphaoperand[2]));

  glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR,
             ud.constantcolor.getValue());
  glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, ud.rgbscale);
  glTexEnvf(GL_TEXTURE_ENV, GL_ALPHA_SCALE, ud.alphascale);
}


#undef MAX_UNITS
#undef PRIVATE
