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
  \class SoProfileElement Inventor/elements/SoProfileElement.h
  \brief The SoProfileElement class is yet to be documented.

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

/*!
  Overloaded to copy profiles and node ids.
*/
void
SoProfileElement::push(SoState * state)
{
  inherited::push(state);

  SoProfileElement * const element =
    (SoProfileElement *)(this->next);
  element->profiles.truncate(0);
  const int numProfiles = this->profiles.getLength();
  for (int i = 0; i < numProfiles; i++)
    element->profiles.append(this->profiles[ i ]);
  element->copyNodeIds(this);
}
