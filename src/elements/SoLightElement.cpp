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
  \class SoLightElement Inventor/elements/SoLightElement.h
  \brief The SoLightElement class manages the currently active light sources.
  \ingroup elements
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
  
  if (elem) {
    int i = elem->lights.getLength();
    elem->lights.append(light);
    elem->addNodeId(light);
    if (i >= elem->matrixlist->getLength())
      elem->matrixlist->append(matrix);
    else
      (*elem->matrixlist)[i] = matrix;
  }
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

// Documented in superclass. Overridden to copy lights to the new top
// of stack. Also copies node ids.
void
SoLightElement::push(SoState * state)
{
  inherited::push(state);

  SoLightElement * const prev =
    (SoLightElement *) this->getNextInStack();
  this->lights.truncate(0);
  const int numLights = prev->lights.getLength();
  int i;
  for (i = 0; i < numLights; i++)
    this->lights.append(prev->lights[ i ]);
  this->matrixlist = prev->matrixlist; // just pass pointer to list
  this->copyNodeIds(prev);
}
