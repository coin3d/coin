/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
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
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/sensors/SoOneShotSensor.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/engines/SoTimeCounter.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/SoOutput.h>

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
  SO_NODE_INTERNAL_INIT_CLASS(SoBlinker, SO_FROM_INVENTOR_1);
}

// Documented in superclass. Overridden to calculate bbox of all
// children.
void
SoBlinker::getBoundingBox(SoGetBoundingBoxAction * action)
{
  this->whichChild.getValue();
  SoGroup::getBoundingBox(action);
}

// Documented in superclass. Overridden to not write internal engine
// connections.
void
SoBlinker::write(SoWriteAction * action)
{
  SoOutput * out = action->getOutput();

  // Decouple connections to/from internal engine to avoid it being
  // written. (Only done at first pass.)
  if (out->getStage() == SoOutput::COUNT_REFS)
    this->deconnectInternalEngine();

  inherited::write(action);

  // Reenable all connections to/from internal engine. (Only done at
  // last pass.)
  if (out->getStage() == SoOutput::WRITE)
    this->reconnectInternalEngine();
}

// Documented in superclass. Overridden to detect "external" changes
// (i.e. not caused by the internal timer engine).
void
SoBlinker::notify(SoNotList * nl)
{
  // See if the whichChild field was "manually" set.
  if (nl->getFirstRec()->getBase() == this &&
      nl->getLastField() == &this->whichChild) {
    this->counter->enableNotify(FALSE); // Wrap to avoid recursive invocation.
    this->counter->reset.setValue(this->whichChild.getValue());
    this->counter->enableNotify(TRUE);
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

// FIXME: I _think_ we made a mistake when overriding SoNode::copy()
// and making it virtual. The special handling we need below could
// just aswell have been done in an overridden copyContents() method,
// which is the recommended mechanism for application programmers. But
// now I think we'll have to support the "virtual-ity" of
// SoNode::copy(), even though it's confusing to have 2 virtual
// copy-methods (both copy() and copyContents()). 20011220 mortene.

// Overridden to decouple and reconnect engine around copy operation.
SoNode *
SoBlinker::copy(SbBool copyconnections) const
{
  // Decouple connections to/from internal engine to avoid it being
  // copied.
  ((SoBlinker *)this)->deconnectInternalEngine();

  SoBlinker * cp = (SoBlinker *)inherited::copy(copyconnections);

  // Reenable all connections to/from internal engine.
  ((SoBlinker *)this)->reconnectInternalEngine();

  // Need to set this explicitly after reconnect, as the internal
  // engine for the copy initially contains incorrect values. The
  // resulting notification on the copy also sets up correct min and
  // max values for the engine.
  cp->whichChild.setValue(this->whichChild.getValue());

  return cp;
}

// Remove connections to and from internal engine.
void
SoBlinker::deconnectInternalEngine(void)
{
  this->whichChild.disconnect(&this->counter->output);
  this->counter->on.disconnect(&this->on);
  this->counter->on = FALSE;
  this->counter->frequency.disconnect(&this->speed);
}


// Reset connections to and from internal engine.
void
SoBlinker::reconnectInternalEngine(void)
{
  this->counter->frequency.connectFrom(&this->speed);
  this->counter->on.connectFrom(&this->on);
  this->whichChild.connectFrom(&this->counter->output);
}
