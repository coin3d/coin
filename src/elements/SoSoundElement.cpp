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
  \since 2003-01-31
*/

#include <Inventor/elements/SoSoundElement.h>
#include <Inventor/nodes/SoNode.h>

/*!
  \fn SoSoundElement::sceneGraphHasSoundNode

  The sceneGraphHasSoundNode flag is used to stop the SoAudioRenderAction in SoSeparator and SoVRMLGroup nodes for cases where the sub-graph doesn not contain any SoVRMLSound nodes.
*/

/*!
  \fn SoSoundElement::isPartOfActiveSceneGraph

  The isPartOfActiveSceneGraph flag is used to make SoVRMLSound that are below inactive parts of the scenegraph (i.e. below a SoSwitch or SoLOD node) stop playing.
*/

/*!
  \fn SoSoundElement::soundNodeIsPlaying

  The soundNodeIsPlaying flag is currently unused.
*/

SO_ELEMENT_SOURCE(SoSoundElement);

/*!
  This static method initializes static data for the
  SoSoundElement class.
*/

void
SoSoundElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoSoundElement, inherited);
}

/*!
  The destructor.
*/

SoSoundElement::~SoSoundElement(void)
{
}

/*! 
  Initializes the element to it's default value.
*/

void
SoSoundElement::init(SoState * state)
{
  inherited::init(state);

  this->setDefaultValues();
}

/*! 
  Sets the flags.
*/

void
SoSoundElement::set(SoState * const state,
                    SoNode * const node,
                    SbBool scenegraphhassoundnode, 
                    SbBool soundnodeisplaying,
                    SbBool ispartofactivescenegraph)
{
  SoSoundElement *elem =
    (SoSoundElement*) SoElement::getElement(state, classStackIndex);
  if (elem) {
    elem->scenegraphhassoundnode = scenegraphhassoundnode;
    elem->soundnodeisplaying = soundnodeisplaying;
    elem->ispartofactivescenegraph = ispartofactivescenegraph;
  }
}

/*!
  Sets the sceneGraphHasSoundNode flag.
 */

SbBool 
SoSoundElement::setSceneGraphHasSoundNode(SoState * const state, SoNode * const node,
                                          SbBool flag)
{
  SoSoundElement *elem = (SoSoundElement *)
    SoElement::getElement(state, classStackIndex);
  if (elem) {
    SbBool old = elem->scenegraphhassoundnode;
    elem->scenegraphhassoundnode = flag;
    return old; 
  } else
    return FALSE;
}

/*!
  Returns the value of the sceneGraphHasSoundNode flag.
 */

SbBool 
SoSoundElement::sceneGraphHasSoundNode(SoState * const state)
{
  const SoSoundElement *elem = (SoSoundElement *)
    SoElement::getConstElement(state, classStackIndex);
  return elem->scenegraphhassoundnode;
}
  

/*!
  Sets the soundNodeIsPlaying flag.
 */

SbBool 
SoSoundElement::setSoundNodeIsPlaying(SoState * const state, SoNode * const node,
                           SbBool flag)
{
  SoSoundElement *elem = (SoSoundElement *)
    SoElement::getElement(state, classStackIndex);
  if (elem) {
    SbBool old = elem->soundnodeisplaying;
    elem->soundnodeisplaying = flag;
    return old; 
  } else
    return FALSE;
}

/*!
  Returns the value of the soundNodeIsPlaying flag.
 */

SbBool 
SoSoundElement::soundNodeIsPlaying(SoState * const state)
{
  const SoSoundElement *elem = (SoSoundElement *)
    SoElement::getConstElement(state, classStackIndex);
  return elem->soundnodeisplaying;
}

/*!
  Sets the isPartOfActiveSceneGraph flag.
 */

SbBool 
SoSoundElement::setIsPartOfActiveSceneGraph(SoState * const state, SoNode * const node,
                                            SbBool flag)
{
  SoSoundElement *elem = (SoSoundElement *)
    SoElement::getElement(state, classStackIndex);
  if (elem) {
    SbBool old = elem->ispartofactivescenegraph;
    elem->ispartofactivescenegraph = flag;
    return old; 
  } else
    return FALSE;
}

/*!
  Returns the value of the isPartOfActiveSceneGraph flag.
*/

SbBool 
SoSoundElement::isPartOfActiveSceneGraph(SoState * const state)
{
  const SoSoundElement *elem = (SoSoundElement *)
    SoElement::getConstElement(state, classStackIndex);
  return elem->ispartofactivescenegraph;
}

/*!  
  Calls the superclass' push method. Initializes the element to the
  default values.
*/

void
SoSoundElement::push(SoState * state)
{
  inherited::push(state);

  this->setDefaultValues();
}

/*!
  Initializes the element to the default values. The default values for the sceneGraphHasSoundNode is FALSE. The default value for the isPartOfActiveSceneGraph flag is TRUE. the default value for the soundNodeIsPlaying flag is FALSE.
 */

void 
SoSoundElement::setDefaultValues()
{
  this->scenegraphhassoundnode = FALSE;
  this->soundnodeisplaying = FALSE;
  this->ispartofactivescenegraph = TRUE;
}


//! Prints contents of element (not implemented)

void
SoSoundElement::print(FILE * /* file */) const
{
}

