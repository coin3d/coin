/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoProfileElement Inventor/elements/SoProfileElement.h
  \brief The SoProfileElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoProfileElement.h>
#include <Inventor/nodes/SoProfile.h>

/*!
  \fn SoProfileElement::Profile

  FIXME: write doc.
*/

/*!
  \fn SoProfileElement::profiles

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoProfileElement);

// doc from parent
void
SoProfileElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoProfileElement, inherited);
}

/*!
  The destructor.
*/
SoProfileElement::~SoProfileElement(void)
{
}

/*!
  Adds \a profile to the list of profiles.
*/
void
SoProfileElement::add(SoState * const state,
                      SoProfile * const profile)
{
  SoProfileElement * element = (SoProfileElement *)
    getElement(state, classStackIndex);

  if (element) {
    switch ((SoProfileElement::Profile)profile->linkage.getValue()) {
    case START_FIRST:
      element->profiles.truncate(0);
      element->profiles.append(profile);
      element->setNodeId(profile);
      break;
    case START_NEW:
      element->profiles.append(profile);
      element->addNodeId(profile);
      break;
    case ADD_TO_CURRENT:
      element->profiles.append(profile);
      element->addNodeId(profile);
      break;
    }
  }
}

/*!
  Returns current list of profiles.
*/
const SoNodeList &
SoProfileElement::get(SoState * const state)
{
  const SoProfileElement * element = (const SoProfileElement *)
    getConstElement(state, classStackIndex);
  return element->profiles;
}

// doc from parent
void
SoProfileElement::init(SoState * state)
{
  inherited::init(state);
  this->profiles.truncate(0);
  this->clearNodeIds();
}

// Documented in superclass. Overridden to copy profiles and node ids.
void
SoProfileElement::push(SoState * state)
{
  inherited::push(state);

  SoProfileElement * const prev =
    (SoProfileElement *) this->getNextInStack();
  
  this->profiles.truncate(0);
  const int numprofiles = prev->profiles.getLength();
  for (int i = 0; i < numprofiles; i++)
    this->profiles.append(prev->profiles[i]);
  this->copyNodeIds(prev);
}
