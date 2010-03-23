/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*! \file SoVertexAttributeElement.h */
#include <Inventor/elements/SoVertexAttributeElement.h>

#include "SbBasicP.h"
#include "elements/SoVertexAttributeData.h"
#include "misc/SbHash.h"

#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/C/glue/gl.h>

class SoVertexAttributeElementP {
public:
  typedef SbHash<const char *, SoVertexAttributeData *> AttribDict;
  AttribDict attribdict;
};

#define PRIVATE(obj) ((obj)->pimpl)

SO_ELEMENT_SOURCE(SoVertexAttributeElement);

void
SoVertexAttributeElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoVertexAttributeElement, inherited);
}

void
SoVertexAttributeElement::init(SoState * state)
{
  inherited::init(state);
  this->clearNodeIds();
}

SoVertexAttributeElement::~SoVertexAttributeElement()
{

}

/*!
  Overridden to copy vertex attributes and node ids.
*/
void
SoVertexAttributeElement::push(SoState * state)
{
  inherited::push(state);

  const SoVertexAttributeElement * prev =
    coin_assert_cast<SoVertexAttributeElement *>(this->getNextInStack());

  PRIVATE(this)->attribdict = PRIVATE(prev)->attribdict;
  this->copyNodeIds(prev);
}

void
SoVertexAttributeElement::add(SoState * const state,
                              SoVertexAttributeData * attribdata)
{
  SoVertexAttributeElement * thisp =
    static_cast<SoVertexAttributeElement *>(SoElement::getElement(state, classStackIndex));

  thisp->addElt(attribdata);
  thisp->addNodeId(attribdata->nodeid);
}

void
SoVertexAttributeElement::addElt(SoVertexAttributeData * attribdata)
{
  PRIVATE(this)->attribdict.put(attribdata->name.getString(), attribdata);
}

const SoVertexAttributeElement *
SoVertexAttributeElement::getInstance(SoState * const state)
{
  return coin_assert_cast<const SoVertexAttributeElement *>
    (getConstElement(state, classStackIndex));
}


unsigned int
SoVertexAttributeElement::getNumAttributes(void) const
{
  return PRIVATE(this)->attribdict.getNumElements();
}

void
SoVertexAttributeElement::applyToAttributes(AttributeApplyFunc * func, void * closure) const
{
  for(
      SoVertexAttributeElementP::AttribDict::const_iterator iter =
       PRIVATE(this)->attribdict.const_begin();
      iter!=PRIVATE(this)->attribdict.const_end();
      ++iter
      ) {
    func(iter->key,iter->obj,closure);
  }
}

#undef PRIVATE
