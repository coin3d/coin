#if 0 // disabled while debugging
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
  \class SoSoundElement Inventor/elements/SoSoundElement.h
  \brief The SoSoundElement is used for optimizing audio rendering and for turning off inactive SoVRMLSound nodes
  \ingroup elements

  The sceneGraphHasSoundNode flag is used to stop the SoAudioRenderAction in SoSeparator and SoVRMLGroup nodes for cases where the sub-graph doesn not contain any SoVRMLSound nodes.

  FIXME: Document the other flags. 2003-01-31 thammer.

  \since 2003-01-31
*/

#include <Inventor/elements/SoListenerPositionElement.h>
#include <Inventor/nodes/SoNode.h>

/*!
  \fn SoListenerPositionElement::position

  The position of the listener in world space. Can be set by the 
  SoListener class or the SoCamera class.
*/

SO_ELEMENT_SOURCE(SoListenerPositionElement);

/*!
  This static method initializes static data for the
  SoListenerPositionElement class.
*/

void
SoListenerPositionElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoListenerPositionElement, inherited);
}

/*!
  The destructor.
*/

SoListenerPositionElement::~SoListenerPositionElement(void)
{
}

/*! 
  Initializes the element to it's default value. The default 
  value for the position is (0.0f, 0.0f, 0.0f) and the 
  default value for the setByListener flag is FALSE.
*/

void
SoListenerPositionElement::init(SoState * state)
{
  inherited::init(state);

  this->scenegraphhassoundnode = FALSE;
  this->soundnodeisplaying = FALSE;
  this->ispartofactivescenegraph = FALSE;
}

/*! 
  Sets the current listener position, and indicates if it was set
  by a SoListener node or a SoCamera node.
*/

void
SoListenerPositionElement::set(SoState * const state,
                               SoNode * const node,
                               SbBool scenegraphhassoundnode, 
                               SbBool soundnodeplaying,
                               SbBool ispartofactivescenegraph)
{
  SoListenerPositionElement *elem =
    (SoListenerPositionElement*) SoElement::getElement(state, classStackIndex);
  if (elem) {
    elem->scenegraphhassoundnode = scenegraphhassoundnode;
    elem->soundnodeisplaying = soundnodeisplaying;
    elem->ispartofactivescenegraph = ispartofactivescenegraph;
  }
}

SbBool 
setSceneGraphHasSoundNode(SoState * const state, SoNode * const node,
                                          SbBool flag)
{
  const SoListenerPositionElement *elem = (SoListenerPositionElement *)
    SoElement::getConstElement(state, classStackIndex);
  return elem->; 
}
  static SbBool sceneGraphHasSoundNode(SoState * const state);

  static SbBool setSoundNodePlaying(SoState * const state, SoNode * const node,
                                    SbBool flag);
  static SbBool soundNodePlaying(SoState * const state);

  static SbBool setIsPartOfActiveSceneGraph(SoState * const state, SoNode * const node,
                                            SbBool flag);
  static SbBool isPartOfActiveSceneGraph(SoState * const state);


//! Returns the current listener position

const SbVec3f &
SoListenerPositionElement::get(SoState * const state)
{
  const SoListenerPositionElement *elem = (SoListenerPositionElement *)
    SoElement::getConstElement(state, classStackIndex);
  return elem->position;
}

//! Prints contents of element (not implemented)

void
SoListenerPositionElement::print(FILE * /* file */) const
{
}
#endif
