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
  \class SoLightElement Inventor/elements/SoLightElement.h
  \brief The SoLightElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoLightElement.h>

#include <Inventor/SbName.h>

#include <assert.h>

/*!
  \fn SoLightElement::lights

  FIXME: write doc.
*/

/*!
  \fn SoLightElement::WCToVRCMatrices

  FIXME: write doc.
*/

/*!
  \fn SoLightElement::startIndex

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoLightElement);

/*!
  This static method initializes static data for the SoLightElement class.
*/

void
SoLightElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoLightElement, inherited);
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoLightElement::createInstance(void)
*/

SoLightElement::SoLightElement(void)
  : startIndex(-1)
{
    setTypeId(SoLightElement::classTypeId);
    setStackIndex(SoLightElement::classStackIndex);
}

/*!
  The destructor.
*/

SoLightElement::~SoLightElement(void)
{
}

//! FIXME: write doc.

void
SoLightElement::add(SoState * const /* state */, SoLight * const /* light */,
                    const SbMatrix & /* WCToVRCMatrix */)
{
}

//! FIXME: write doc.

const SoNodeList &
SoLightElement::getLights(SoState * const /* state */)
{
  COIN_STUB();
  static SoNodeList nl;
  return nl;
}

//! FIXME: write doc.

const SbMatrix &
SoLightElement::getMatrix(SoState * const /* state */, const int /* index */)
{
  COIN_STUB();
  static SbMatrix m = SbMatrix::identity();
  return m;
}

//! FIXME: write doc.

void
SoLightElement::print(FILE * file) const
{
  fprintf(file, "SoLightElement[%p]: lights = %dn",
          this, this->lights.getLength());
}

//! FIXME: write doc.

void
SoLightElement::init(SoState * state)
{
    inherited::init(state);
}

//! FIXME: write doc.

void
SoLightElement::push(SoState * state)
{
    inherited::push(state);

    SoLightElement * const element =
        (SoLightElement *)(this->next);
    element->lights.truncate(0);
    const int numLights = this->lights.getLength();
    int i;
    for (i = 0; i < numLights; i++)
        element->lights.append(this->lights[ i ]);
    element->WCToVRCMatrices.truncate(0);
    const int numMatrices = WCToVRCMatrices.getLength();
    for (i = 0; i < numMatrices; i++)
        element->WCToVRCMatrices.append(this->WCToVRCMatrices[ i ]);
    element->startIndex = this->startIndex;
}

//! FIXME: write doc.

void
SoLightElement::pop(SoState * state, const SoElement * prevTopElement)
{
    inherited::pop(state, prevTopElement);
}
