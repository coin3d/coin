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
  \class SoHandleEventAction Inventor/actions/SoHandleEventAction.h
  \brief The SoHandleEventAction class is for traversing a scene graph for
  nodes of interest to a particular event.

  FIXME: write doc.
  TODO:
  - use raypick action (when it's ready)
  - handle grabs
*/

#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/SbName.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/lists/SoPickedPointList.h>
#include <Inventor/elements/SoSwitchElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoWindowElement.h>
#include <Inventor/nodes/SoNode.h>

// *************************************************************************

//$ BEGIN TEMPLATE ActionSource(SoHandleEventAction)

SoType SoHandleEventAction::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the classname class.
*/
SoType
SoHandleEventAction::getClassTypeId(void)
{
  return classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoHandleEventAction::getTypeId(void) const
{
  return classTypeId;
}

// static variables
SoEnabledElementsList * SoHandleEventAction::enabledElements;
SoActionMethodList * SoHandleEventAction::methods;

/*!
  \fn SoHandleEventAction::enabledElements
  FIXME: write doc.
*/

/*!
  \fn SoHandleEventAction::methods
  FIXME: write doc.
*/

/*!
  This method returns the list of enabled elements for the given action class.
*/
const SoEnabledElementsList &
SoHandleEventAction::getEnabledElements(void) const
{
  return *enabledElements;
}

/*!
  This method adds a method to be perfomed by the action class on the given
  node type.
*/
void 
SoHandleEventAction::addMethod(const SoType type, SoActionMethod method)
{
  methods->addMethod(type, method);
}

/*!
  This method enables an element in the state stack for the action class.
*/
void 
SoHandleEventAction::enableElement(const SoType type, const int stackIndex)
{
  enabledElements->enable(type, stackIndex);
}
//$ END TEMPLATE ActionSource

// *************************************************************************

/*!
  This static method initializes the static data for the
  SoHandleEventAction class.
*/

void
SoHandleEventAction::initClass(void)
{
//$ BEGIN TEMPLATE InitActionSource(SoHandleEventAction)
  assert(SoHandleEventAction::getClassTypeId() == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoHandleEventAction::classTypeId = 
      SoType::createType(inherited::getClassTypeId(),
                         "SoHandleEventAction");
  enabledElements = new SoEnabledElementsList(inherited::enabledElements);
  methods = new SoActionMethodList(inherited::methods);
//$ END TEMPLATE InitActionSource

  // FIXME: aren't these supposed to be enabled from node classes?
  // 19990214 mortene.
  ENABLE_ELEMENT(SoSwitchElement);
  ENABLE_ELEMENT(SoViewVolumeElement);
  ENABLE_ELEMENT(SoViewportRegionElement);
  ENABLE_ELEMENT(SoWindowElement);

//    SO_ACTION_ADD_METHOD(SoNode, SoNode::handleEventS);
  methods->setDefault((void *)SoNode::handleEventS);
}

/*!
  This static method cleans up the static data for the
  SoHandleEventAction class.
*/
void
SoHandleEventAction::cleanClass(void)
{
}

// *************************************************************************

/*!
  A constructor.
*/
SoHandleEventAction::SoHandleEventAction(const SbViewportRegion &
					 viewportRegion)
{
  SO_ACTION_CONSTRUCTOR(SoHandleEventAction);  

  this->vpregion = viewportRegion;
  this->event = NULL;
  this->grabbernode = NULL;
}

/*!
  The destructor.
*/
SoHandleEventAction::~SoHandleEventAction()
{
}

/*!
  This method sets the viewport region.
*/
void
SoHandleEventAction::setViewportRegion(const SbViewportRegion & newRegion)
{
  this->vpregion = newRegion;
}

/*!
  This method returns the viewport region.
*/
const SbViewportRegion &
SoHandleEventAction::getViewportRegion(void) const
{
  return this->vpregion;
}

/*!
  This method sets the event.
*/
void
SoHandleEventAction::setEvent(const SoEvent * ev)
{
  this->event = ev;
}

/*!
  Ths method returns the event.
*/
const SoEvent *
SoHandleEventAction::getEvent(void) const
{
  return this->event;
}

/*!
  This method marks the action instance as handled, hence terminates the
  action.
*/
void
SoHandleEventAction::setHandled(void)
{
  this->setTerminated(TRUE);
}

/*!
  This method returns wether the event has been handled or not.
*/
SbBool
SoHandleEventAction::isHandled(void) const
{
  return this->hasTerminated();
}

/*!
  This method sets the grabber node.
*/
void
SoHandleEventAction::setGrabber(SoNode * node)
{
  this->grabbernode = node;
}

/*!
  This method unsets the grabber node.
*/
void
SoHandleEventAction::releaseGrabber(void)
{
  this->setGrabber(NULL);
}

/*!
  This method returns the grabber node.
*/
SoNode *
SoHandleEventAction::getGrabber(void) const
{
  return this->grabbernode;
}

/*!
  This method sets the root node that is used for a pick action used by
  nodes that tracks the cursor.
*/
void
SoHandleEventAction::setPickRoot(SoNode * /* node */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  This method returns the root node that is used by nodes that tracks the
  cursor.
*/
SoNode *
SoHandleEventAction::getPickRoot(void) const
{
  assert(0 && "FIXME: not implemented");
  return NULL;
}

/*!
  This method sets the pick radius.
*/
void
SoHandleEventAction::setPickRadius(float /* radiusInPixels */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  This method returns the picked point.
*/
const SoPickedPoint *
SoHandleEventAction::getPickedPoint(void)
{
  assert(0 && "FIXME: not implemented");
  return NULL;
}

/*!
  This method returns a list of picked points.
*/
const SoPickedPointList &
SoHandleEventAction::getPickedPointList(void)
{
  assert(0 && "FIXME: not implemented");
  static SoPickedPointList pl;
  return pl;
}

/*!
  This method is called before the graph is traversed.
*/
void
SoHandleEventAction::beginTraversal(SoNode * node)
{
  this->traverse(node);
}
