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

#include <assert.h>

/*!
  \fn SoProfileElement::Profile

  FIXME: write doc.
*/

/*!
  \fn SoProfileElement::profiles

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoProfileElement);

/*!
  This static method initializes static data for the SoProfileElement class.
*/

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

//! FIXME: write doc.

void
SoProfileElement::add(SoState * const state,
                      SoProfile * const profile)
{
  SoProfileElement * element = (SoProfileElement *)
    getElement(state, classStackIndex);

  // FIXME: this doesn't look correct. Shouldn't we check the
  // SoProfile::linkage field, and then append or replace based on
  // that value? 20000327 mortene.
  element->profiles.append(profile);
}

//! FIXME: write doc.

const SoNodeList &
SoProfileElement::get(SoState * const state)
{
  const SoProfileElement * element = (const SoProfileElement *)
    getConstElement(state, classStackIndex);
  return element->profiles;
}

//! FIXME: write doc.

void
SoProfileElement::print(FILE * file) const
{
    fprintf(file, "SoProfileElement[%p]: num = %d\n", this,
        this->profiles.getLength());
}

//! FIXME: write doc.

void
SoProfileElement::init(SoState * state)
{
  inherited::init(state);
  this->profiles.truncate(0);
}

//! FIXME: write doc.

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
}

//! FIXME: write doc.

void
SoProfileElement::pop(SoState * state, const SoElement * prevTopElement)
{
    inherited::pop(state, prevTopElement);
}
