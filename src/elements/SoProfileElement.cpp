/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#include <Inventor/SbName.h>
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

//$ BEGIN TEMPLATE ElementSource(SoProfileElement)

/*!
  \var SoProfileElement::classTypeId

  This is the static class type identifier for the
  SoProfileElement class.
*/

SoType SoProfileElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoProfileElement::getClassTypeId(void)
{
  return SoProfileElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoProfileElement class.
*/
void *
SoProfileElement::createInstance(void)
{
  return (void *) new SoProfileElement;
}

/*!
  \var SoProfileElement::classStackIndex

  This is the static state stack index for the
  SoProfileElement class.
*/
int SoProfileElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoProfileElement class.
*/
int
SoProfileElement::getClassStackIndex(void)
{
  return SoProfileElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the SoProfileElement class.
*/

void
SoProfileElement::initClass(void)
{
//$ BEGIN TEMPLATE InitElementSource(SoProfileElement)
  assert(SoProfileElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoProfileElement::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SoProfileElement",
                       &SoProfileElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoProfileElement::classStackIndex =
      createStackIndex(SoProfileElement::classTypeId);
  }
  else {
    SoProfileElement::classStackIndex = inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoProfileElement::createInstance(void)
*/

SoProfileElement::SoProfileElement(void)
  : profiles()
{
    setTypeId(SoProfileElement::classTypeId);
    setStackIndex(SoProfileElement::classStackIndex);
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
