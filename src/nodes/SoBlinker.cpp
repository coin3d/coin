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
  \class SoBlinker SoBlinker.h Inventor/nodes/SoBlinker.h
  \brief The SoBlinker class is a cycling switch node.
  \ingroup nodes

  This switch node cycles its children SoBlinker::speed number of
  times per second. If the node has only one child, it will be cycled
  on and off. Cycling can be turned off using the SoBlinker::on
  field, and the node then behaves like a normal SoSwitch node.
*/

#include <Inventor/nodes/SoBlinker.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/sensors/SoOneShotSensor.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/engines/SoTimeCounter.h>
#include <Inventor/misc/SoChildList.h>

#include <coindefs.h> // COIN_STUB()

/*!
  \var SoSFFloat SoBlinker::speed
  Number of cycles per second.
*/
/*!
  \var SoSFBool SoBlinker::on
  Controls whether cycling is on or off.
*/


// *************************************************************************

SO_NODE_SOURCE(SoBlinker);

/*!
  Constructor.
*/
SoBlinker::SoBlinker()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoBlinker);

  SO_NODE_ADD_FIELD(speed, (1));
  SO_NODE_ADD_FIELD(on, (TRUE));

  // OneShot sensors needed to avoid recursive notify
  this->childrenSensor = new SoOneShotSensor(SoBlinker::childrenCB, this);
  this->whichSensor = new SoOneShotSensor(SoBlinker::whichCB, this);

  this->counter = new SoTimeCounter;
  this->counter->ref();
  this->setCounterLimits();
  this->counter->frequency.connectFrom(&this->speed);
  this->counter->on.connectFrom(&this->on);
  this->whichChild.connectFrom(&this->counter->output);
  this->whichChild.setDirty(TRUE);
}

/*!
  Destructor.
*/
SoBlinker::~SoBlinker()
{
  delete this->childrenSensor;
  delete this->whichSensor;
  this->counter->unref();
}

// doc in parent
void
SoBlinker::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoBlinker);
}

/*!
  Overloaded to calculate bbox of all children.
*/
void
SoBlinker::getBoundingBox(SoGetBoundingBoxAction *action)
{
  this->whichChild.getValue();
  SoGroup::getBoundingBox(action);
}

/*!
  Overloaded not to write internal engine connections.
*/
void
SoBlinker::write(SoWriteAction *action)
{
  COIN_STUB();
  inherited::write(action);
}

/*!
  Overloaded to detect field/children changes.
*/
void
SoBlinker::notify(SoNotList *list)
{
  SoField *f = list->getLastField();
  if (f == &this->whichChild) {
    if (list->getFirstRec()->getBase() == this) this->whichSensor->schedule();
  }
  else this->childrenSensor->schedule();
  SoNode::notify(list);
}

// sets the counter min/max values
void
SoBlinker::setCounterLimits(void)
{
  int lastchild = this->getChildren()->getLength() - 1;
  this->counter->max.setValue(lastchild);
  this->counter->min.setValue(lastchild > 0 ? 0 : -1);
}

// OneShot callback when children change
void
SoBlinker::childrenCB(void *d, SoSensor *s)
{
  ((SoBlinker*)d)->setCounterLimits();
}

// OneShot callback when whichChild is manually set
void
SoBlinker::whichCB(void *d, SoSensor *s)
{
  SoBlinker *thisp = (SoBlinker*)d;
  thisp->counter->reset.setValue(thisp->whichChild.getValue());
  thisp->whichChild.touch(); // notify scene graph
  thisp->whichChild.setDirty(TRUE); // force engine to evaluate
}
