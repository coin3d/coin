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
  on and off. Cycling can be turned off using the SoBlinker::on field,
  and the node then behaves like a normal SoSwitch node.
*/

#include <Inventor/nodes/SoBlinker.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/sensors/SoOneShotSensor.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/engines/SoTimeCounter.h>
#include <Inventor/misc/SoChildList.h>

#include <coindefs.h> // COIN_STUB()

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

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
SoBlinker::SoBlinker(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoBlinker);

  SO_NODE_ADD_FIELD(speed, (1));
  SO_NODE_ADD_FIELD(on, (TRUE));

  this->counter = new SoTimeCounter;
  this->counter->ref();
  this->counter->min = SO_SWITCH_NONE;
  this->counter->max = SO_SWITCH_NONE;
  this->counter->frequency.connectFrom(&this->speed);
  this->counter->on.connectFrom(&this->on);
  this->whichChild.connectFrom(&this->counter->output);
}

/*!
  Destructor.
*/
SoBlinker::~SoBlinker()
{
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
SoBlinker::getBoundingBox(SoGetBoundingBoxAction * action)
{
  this->whichChild.getValue();
  SoGroup::getBoundingBox(action);
}

/*!
  Overloaded not to write internal engine connections.
*/
void
SoBlinker::write(SoWriteAction * action)
{
  // Decouple connections to/from internal engine to avoid it being
  // written.
  this->whichChild.disconnect(&this->counter->output);
  this->counter->on.disconnect(&this->on);
  this->counter->on = FALSE;
  this->counter->frequency.disconnect(&this->speed);

  inherited::write(action);

  // Reenable all connections to/from internal engine.
  this->counter->frequency.connectFrom(&this->speed);
  this->counter->on.connectFrom(&this->on);
  this->whichChild.connectFrom(&this->counter->output);

  // Make sure "on" field of engine get synchronized with the "on"
  // field of the blinker.
  this->on.touch();
}

/*!
  Overloaded to detect "external" changes (i.e. not caused by the
  internal timer engine).
*/
void
SoBlinker::notify(SoNotList * nl)
{
  // See if the whichChild field was "manually" set.
  if (nl->getFirstRec()->getBase() == this &&
      nl->getLastField() == &this->whichChild) {
    short counterval = ((SoSFShort *)(this->counter->output)[0])->getValue();
    if (this->whichChild.getValue() != counterval) {
      this->counter->enableNotify(FALSE); // Wrap to avoid recursive invocation.
      this->counter->reset.setValue(this->whichChild.getValue());
      this->counter->enableNotify(TRUE);
    }
  }

  // Check if a child was added or removed.
  int lastchildidx = this->getNumChildren() - 1;
  if (this->counter->max.getValue() != lastchildidx) {
    // Wrap to avoid recursive invocation.
    this->counter->enableNotify(FALSE);

    // Note that if we have one child, the counting should go from -1
    // to 0 (so the child is toggled on and off).
    this->counter->min.setValue(lastchildidx > 0 ? 0 : SO_SWITCH_NONE);
    this->counter->max.setValue(lastchildidx >= 0 ? lastchildidx : SO_SWITCH_NONE);

    // To avoid SoSwitch getting an out-of-range whichChild value, in
    // case whichChild was at the end.
    if (lastchildidx < this->whichChild.getValue()) {
      this->counter->reset.setValue(lastchildidx);
      this->whichChild.setDirty(TRUE); // Force evaluate() on the field.
    }

    this->counter->enableNotify(TRUE);
  }

  inherited::notify(nl);
}
