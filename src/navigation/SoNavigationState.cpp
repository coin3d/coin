/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/lists/SbList.h>
#include <Inventor/events/SoEvent.h>

#include <Inventor/navigation/SoNavigationState.h>
#include "SoEvent.h"

// *************************************************************************

/*!
  \class SoNavigationState Inventor/navigation/SoNavigationState.h
  \brief Internal class.

  This class is only used internally in the SoNavigationSystem class
  to track the current navigation state (current mode).  It isn't
  exposed anywhere.

  \ingroup navigation
*/

// *************************************************************************

class SoNavigationStateP {
public:
  SoNavigationStateP(SoNavigationState * api);

  SbList<SoNavigationMode *> modestack;
  SbList<const SoEvent *> triggerstack;
};

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)

/*!
  Constructor.
*/

SoNavigationState::SoNavigationState(void)
{
  PRIVATE(this) = new SoNavigationStateP(this);
}
/*!
  Destructor.
*/

SoNavigationState::~SoNavigationState(void)
{
  delete PRIVATE(this);
  PRIVATE(this) = NULL;
}

/*!
  Pushes a new mode on the state stack.  \a trigger is the event that
  triggered the mode change.

  \sa pop, getMode, getTrigger
*/

void
SoNavigationState::push(SoNavigationMode * mode, const SoEvent * trigger)
{
  PRIVATE(this)->modestack.append(mode);
  if (trigger) {
    PRIVATE(this)->triggerstack.append(SoEvent_Clone(trigger));
  } else {
    PRIVATE(this)->triggerstack.append(NULL);
  }
}

/*!
  Pops the mode state stack.
*/

void
SoNavigationState::pop(void)
{
  PRIVATE(this)->modestack.pop();
  const SoEvent * trigger = PRIVATE(this)->triggerstack.pop();
  if (trigger != NULL ) {
    delete trigger;
  }
}

/*!
  Resets the mode state stack.
*/

void
SoNavigationState::reset(void)
{
  PRIVATE(this)->modestack.truncate(0);
  int i;
  for ( i = 0; i < PRIVATE(this)->triggerstack.getLength(); i++ ) {
    const SoEvent * trigger = PRIVATE(this)->triggerstack[i];
    if (trigger != NULL) {
      delete trigger;
    }
  }
  PRIVATE(this)->triggerstack.truncate(0);
}

/*!
  Returns the mode at the top of the mode state stack.

  \sa push, pop, getTrigger
*/

SoNavigationMode *
SoNavigationState::getMode(void) const
{
  if (PRIVATE(this)->modestack.getLength() == 0) return NULL;
  return PRIVATE(this)->modestack[PRIVATE(this)->modestack.getLength()-1];
}

/*!
  Returns the event that triggered the current mode.

  NULL is returned in the case of the initial mode and if the mode
  state stack has been popped empty.

  \sa getMode, push, pop
*/

const SoEvent *
SoNavigationState::getTrigger(void) const
{
  if (PRIVATE(this)->modestack.getLength() == 0) return NULL;
  return PRIVATE(this)->triggerstack[PRIVATE(this)->triggerstack.getLength()-1];
}

#undef PRIVATE

// *************************************************************************

SoNavigationStateP::SoNavigationStateP(SoNavigationState * api)
{
}

