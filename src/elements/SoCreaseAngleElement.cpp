/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoCreaseAngleElement Inventor/elements/SoCreaseAngleElement.h
  \brief The SoCreaseAngleElement class stores the crease angle during a scene graph traversal.
  \ingroup elements
*/

#include <Inventor/elements/SoCreaseAngleElement.h>


#include <assert.h>

SO_ELEMENT_SOURCE(SoCreaseAngleElement);

// doc from parent
void
SoCreaseAngleElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoCreaseAngleElement, inherited);
}

/*!
  The destructor.
*/
SoCreaseAngleElement::~SoCreaseAngleElement(void)
{
}

// doc from parent
void
SoCreaseAngleElement::init(SoState * state)
{
  inherited::init(state);

  // a negative value means that the element contains the default
  // value, which is 0.0 for Inventor 2.1 and 0.5 for VRML1.
  this->data = -1.0f;
}

/*!
  Sets the value of this element.
*/
void
SoCreaseAngleElement::set(SoState * const state, SoNode * const node,
                          const float complexity)
{
  SoFloatElement::set(classStackIndex, state, node, complexity);
}


/*!
  \overload
*/
void
SoCreaseAngleElement::set(SoState * const state, const float complexity)
{
  SoCreaseAngleElement::set(state, NULL, complexity);
}

/*!  
  Returns the element value. This method can be used if you know
  that the node that is going to use the crease angle is an Inventor
  node.  
*/
float
SoCreaseAngleElement::get(SoState * const state)
{
  float val = SoFloatElement::get(classStackIndex, state);
  return val < 0.0f ? SoCreaseAngleElement::getDefault() : val;
}

/*!  
  Returns the element value. \a isvrml1 should be TRUE if the node
  requesting the value is a VRML1 node.
  
  This method is an extension versus the Open Inventor API.

  \sa SoNode::getNodeType() 
*/
float 
SoCreaseAngleElement::get(SoState * const state, const SbBool isvrml1)
{
  float val = SoFloatElement::get(classStackIndex, state);
  return val < 0.0f ? SoCreaseAngleElement::getDefault(isvrml1) : val;
}

/*!
  Returns the default value for Inventor scene graphs (0.0).
*/
float
SoCreaseAngleElement::getDefault(void)
{
  return 0.0f;
}

/*!
  Returns the default value for this element. \a isvrml1 should
  be TRUE if the node requesting the value is a VRML1 node.

  This method is an extension versus the Open Inventor API.
  
  \sa SoNode::getNodeType() 
*/
float 
SoCreaseAngleElement::getDefault(const SbBool isvrml1)
{
  return isvrml1 ? 0.5f : 0.0f;
}
