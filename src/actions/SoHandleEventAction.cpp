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
#include <Inventor/events/SoEvent.h>
#include <Inventor/SbName.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/lists/SoPickedPointList.h>
#include <Inventor/elements/SoSwitchElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoWindowElement.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/actions/SoRayPickAction.h>

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

#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

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
  assert(enabledElements);
  return *enabledElements;
}

/*!
  This method adds a method to be perfomed by the action class on the given
  node type.
*/
void
SoHandleEventAction::addMethod(const SoType type, SoActionMethod method)
{
  assert(methods);
  methods->addMethod(type, method);
}

/*!
  This method enables an element in the state stack for the action class.
*/
void
SoHandleEventAction::enableElement(const SoType type, const int stackIndex)
{
  assert(enabledElements);
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

}

// *************************************************************************

/*!
  A constructor.
*/
SoHandleEventAction::SoHandleEventAction(const SbViewportRegion &viewportRegion)
  : viewport(viewportRegion),
    event(NULL),
    grabber(NULL),
    pickRoot(NULL),
    pickValid(FALSE),
    pickAction(NULL),
    applyNode(NULL)

{
  SO_ACTION_CONSTRUCTOR(SoHandleEventAction);

  static int first = 1;
  if (first) {
    first = 0;
    SO_ACTION_ADD_METHOD(SoNode, SoNode::handleEventS);
  }
  methods->setUp(); // FIXME: not sure if this should be called here...
}

/*!
  The destructor.
*/
SoHandleEventAction::~SoHandleEventAction()
{
  delete this->pickAction;
}

/*!
  This method sets the viewport region.
*/
void
SoHandleEventAction::setViewportRegion(const SbViewportRegion & newRegion)
{
  this->viewport = newRegion;
  delete this->pickAction;
  this->pickAction = NULL;
}

/*!
  This method returns the viewport region.
*/
const SbViewportRegion &
SoHandleEventAction::getViewportRegion(void) const
{
  return this->viewport;
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

  The action is only marked as handled when a node in the graph "grabs"
  the event this action is carrying, so the handled flag will be
  FALSE after traversal if no nodes wanted the event.

  \sa isHandled()
*/
void
SoHandleEventAction::setHandled(void)
{
  this->setTerminated(TRUE);
}

/*!
  This method returns whether or not the event has been handled by
  a node during scene graph traversal.

  \sa setHandled()
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
  this->grabber = node;
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
  This method returns the grabber node, or NULL if no grabber is active.
*/
SoNode *
SoHandleEventAction::getGrabber(void) const
{
  return this->grabber;
}

/*!
  This method sets the root node that is used for a pick action used by
  nodes that tracks the cursor.
*/
void
SoHandleEventAction::setPickRoot(SoNode *node)
{
  this->pickRoot = node;
}

/*!
  This method returns the root node that is used by nodes that tracks the
  cursor.
*/
SoNode *
SoHandleEventAction::getPickRoot(void) const
{
  return this->pickRoot;
}

/*!
  This method sets the pick radius.
*/
void
SoHandleEventAction::setPickRadius(const float radiusInPixels)
{
  this->getPickAction()->setRadius(radiusInPixels);
}

/*!
  This method returns the picked point.
*/
const SoPickedPoint *
SoHandleEventAction::getPickedPoint(void)
{
  if (this->event && (this->pickRoot || this->applyNode) &&
      (!this->pickValid || this->didPickAll)) {
    this->getPickAction()->setPoint(this->event->getPosition());
    this->getPickAction()->setPickAll(FALSE);
    this->getPickAction()->apply(this->pickRoot ? this->pickRoot : this->applyNode);
    this->pickValid = TRUE;
    this->didPickAll = FALSE;
  }
  return this->getPickAction()->getPickedPoint();
}

/*!
  This method returns a list of picked points.
*/
const SoPickedPointList &
SoHandleEventAction::getPickedPointList(void)
{
  //
  // Maybe it is A Good Thing to make an extension to SoRayPickAction, to always
  // make it store all picked point, but also store the closest picked point?
  // pederb, 19991214
  //

  if (this->event && (this->pickRoot || this->applyNode) &&
      (!this->pickValid || this->didPickAll)) {
    this->getPickAction()->setPickAll(TRUE);
    this->getPickAction()->apply(this->pickRoot ? this->pickRoot : this->applyNode);
    this->pickValid = TRUE;
    this->didPickAll = TRUE;
  }
  return this->getPickAction()->getPickedPointList();
}

/*!
  This method is called before the graph is traversed.
*/
void
SoHandleEventAction::beginTraversal(SoNode * node)
{
  assert(this->event);
  this->pickValid = FALSE;
  this->applyNode = node;

  if (this->grabber) {
    // ?? is this correct ?? pederb, 19991214
    this->traverse(this->grabber);
  }
  else {
    this->traverse(node);
  }
}

SoRayPickAction *
SoHandleEventAction::getPickAction()
{
  if (this->pickAction == NULL) {
    this->pickAction = new SoRayPickAction(this->viewport);
  }
  return this->pickAction;
}
