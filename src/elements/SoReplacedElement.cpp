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

/*!
  \class SoReplacedElement Inventor/elements/SoReplacedElement.h
  \brief The SoReplacedElement class is an abstract element superclass.
  \ingroup elements

  This is the superclass of all elements where the new element data \e
  replaces the old data, and where the data the element stores is not
  just a simple float or integer value.

  Apart from this conceptual difference from its superclass, the
  SoReplacedElement class also overloads the default getElement()
  method to include a node reference. This reference is used to fetch
  the unique node identification number of the node that last changed
  the element.

  The identifier values of nodes in the scenegraph is updated upon \e
  any kind of change to a node, so this technique plays an important
  role in the construction, validation and destruction of internal
  scenegraph caches.

  \sa SoAccumulatedElement
*/

#include <Inventor/elements/SoReplacedElement.h>

#include "SbBasicP.h"

#include <Inventor/nodes/SoNode.h>
#include <cassert>

/*!
  \var uint32_t SoReplacedElement::nodeId
  \COININTERNAL
*/


SO_ELEMENT_ABSTRACT_SOURCE(SoReplacedElement);


// Documented in superclass.
void
SoReplacedElement::initClass(void)
{
  SO_ELEMENT_INIT_ABSTRACT_CLASS(SoReplacedElement, inherited);
}

/*!
  Destructor.
*/
SoReplacedElement::~SoReplacedElement(void)
{
}

// Documented in superclass.
void
SoReplacedElement::init(SoState * state)
{
  inherited::init(state);
  this->nodeId = 0;
}

// Documented in superclass.
SbBool
SoReplacedElement::matches(const SoElement * element) const
{
  if ((coin_assert_cast<const SoReplacedElement *>(element))->nodeId ==
      this->nodeId)
    return TRUE;
  return FALSE;
}

// Documented in superclass.
SoElement *
SoReplacedElement::copyMatchInfo(void) const
{
  assert(getTypeId().canCreateInstance());
  SoReplacedElement * element =
    static_cast<SoReplacedElement *>(getTypeId().createInstance());
  element->nodeId = this->nodeId;
  return element;
}

// Documented in superclass.
void
SoReplacedElement::print(FILE * file) const
{
  const char * typen = this->getTypeId().getName().getString();
  (void)fprintf(file, "%s[%p]\n", typen, this);
}

/*!
  This function overloads SoElement::getElement() with an extra \a
  node parameter, to let us set the SoReplacedElement::nodeId in the
  element instance before returning.

  SoReplacedElement subclasses should use this method to get writable
  instances.

  The identifier values of nodes in the scenegraph is updated upon \e
  any kind of change to a node, so this technique plays an important
  role in the construction, validation and destruction of internal
  scenegraph caches.

  \sa SoElement::getElement()
*/
SoElement *
SoReplacedElement::getElement(SoState * const state, const int stackIndex,
                                     SoNode * const node)
{
  SoReplacedElement * elem =
    coin_safe_cast<SoReplacedElement *>(SoElement::getElement(state, stackIndex));
  if (elem) {
    if (node) { elem->nodeId = node->getNodeId(); }
    else { elem->nodeId = 0; }
    return elem;
  }
  return NULL;
}

/*!
  Returns the node identifier for the node that previously updated the
  SoReplacedElement.
*/
uint32_t
SoReplacedElement::getNodeId(void) const
{
  return this->nodeId;
}
