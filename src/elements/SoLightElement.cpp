/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoLightElement Inventor/elements/SoLightElement.h
  \brief The SoLightElement class manages the currently active light sources.

*/

#include <Inventor/elements/SoLightElement.h>
#include <Inventor/nodes/SoLight.h>
#include <Inventor/lists/SbList.h>
#include <assert.h>

/*!
  \fn SoLightElement::lights
  List of current light nodes.
*/

/*!
  \fn SoLightElement::matrixlist

  List of matrices to map from world coordinates to view reference
  coordinates. To avoid getting a hugs element (sizeof), this
  list is only allocated in the bottom element, and the pointer
  to this list is passed along to the other elements.
*/

SO_ELEMENT_SOURCE(SoLightElement);


// doc from parent
void
SoLightElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoLightElement, inherited);
}

/*!
  The destructor.
*/
SoLightElement::~SoLightElement(void)
{
  if (this->didalloc.state) delete this->matrixlist;
}


/*!
  Adds \a light to the list of active lights. \a matrix should
  transform the light from the world coordinate system to
  the view reference coordinate system.
*/
void
SoLightElement::add(SoState * const state, SoLight * const light,
                    const SbMatrix & matrix)
{
  SoLightElement * elem = (SoLightElement*)
    SoElement::getElement(state, classStackIndex);

  int i = elem->lights.getLength();
  elem->lights.append(light);
  elem->addNodeId(light);
  if (i >= elem->matrixlist->getLength())
    elem->matrixlist->append(matrix);
  else
    (*elem->matrixlist)[i] = matrix;
}

/*!
  Returns the list of light nodes.
*/
const SoNodeList &
SoLightElement::getLights(SoState * const state)
{
  SoLightElement * elem = (SoLightElement*)
    SoElement::getConstElement(state, classStackIndex);
  return elem->lights;
}

/*!
  Get matrix which transforms light \a index from the world
  coordinate system to the view reference system.
*/
const SbMatrix &
SoLightElement::getMatrix(SoState * const state, const int index)
{
  SoLightElement * elem = (SoLightElement*)
    SoElement::getConstElement(state, classStackIndex);
  assert(index >= 0 && index < elem->matrixlist->getLength());
  return elem->matrixlist->getArrayPtr()[index];
}

// doc from parent
void
SoLightElement::init(SoState * state)
{
  inherited::init(state);
  this->matrixlist = new SbList <SbMatrix>;
  this->didalloc.state = TRUE;
}

/*!
  Overloaded to copy lights to the new top of stack. Also
  copies node ids.
*/
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
  element->matrixlist = this->matrixlist; // just pass pointer to list
  element->copyNodeIds(this);
}
