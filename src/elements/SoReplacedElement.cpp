/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoReplacedElement Inventor/elements/SoReplacedElement.h
  \brief The SoReplacedElement class is an abstract element superclass.
  \ingroup elements

  This is the superclass of all elements where the new element value
  \e replaces the old value. Most elements in the Coin library is in
  this category.

  Apart from this conceptual difference from it's superclass, the
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
#include <Inventor/nodes/SoNode.h>
#include <assert.h>

/*!
  \var uint32_t SoReplacedElement::nodeId
  \internal
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
  if (((const SoReplacedElement *)(element))->nodeId ==
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
    (SoReplacedElement *)(getTypeId().createInstance());
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
    (SoReplacedElement *)SoElement::getElement(state, stackIndex);
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
