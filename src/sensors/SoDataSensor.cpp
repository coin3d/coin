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
  \class SoDataSensor SoDataSensor.h Inventor/sensors/SoDataSensor.h
  \brief The SoDataSensor class is the abstract base class for sensors monitoring changes in a scene graph.
  \ingroup sensors

  If you need to know when a particular entity (as a field or a node)
  changes, subclasses of SoDataSensor can be used to monitor the
  entity and notify you when it changes.
*/

#include <Inventor/sensors/SoDataSensor.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/misc/SoNotification.h>
#include <Inventor/nodes/SoNode.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


/*!
  \fn void SoDataSensor::dyingReference(void)

  This method is called when the entity we are connected to is about
  to be deleted.
*/


/*!
  Default constructor.
*/
SoDataSensor::SoDataSensor(void)
  : cbfunc(NULL),
    cbdata(NULL),
    findpath(FALSE),
    triggerfield(NULL),
    triggernode(NULL),
    triggerpath(NULL)
{
}

/*!
  Constructor taking as parameters the sensor callback function and
  the userdata which will be passed the callback.

  \sa setFunction(), setData()
*/
SoDataSensor::SoDataSensor(SoSensorCB * func, void * data)
  : inherited(func, data),
    cbfunc(NULL),
    cbdata(NULL),
    findpath(FALSE),
    triggerfield(NULL),
    triggernode(NULL),
    triggerpath(NULL)
{
}

/*!
  Destructor.
*/
SoDataSensor::~SoDataSensor(void)
{
  if (this->triggerpath) this->triggerpath->unref();
}

/*!
  If an object monitored by a data sensor is deleted, the given
  callback function will be called with the given userdata.
 */
void
SoDataSensor::setDeleteCallback(SoSensorCB * function, void * data)
{
  this->cbfunc = function;
  this->cbdata = data;
}

/*!
  Returns a pointer to the node causing the sensor to trigger, or \c
  NULL if there was no such node.

  \c NULL will also be returned for sensors which are not immediate
  sensors (i.e. with priority equal to 0), as the result could
  otherwise be misleading (non-immediate sensors could have been
  scheduled and rescheduled multiple times, so there wouldn't be a
  single node responsible for the sensor triggering).

  The result is only valid within the scope of a trigger(), so if you
  need to use the pointer outside your sensor callback, you must store
  it.

  \sa getTriggerField()
*/
SoNode *
SoDataSensor::getTriggerNode(void) const
{
  return this->triggernode;
}

/*!
  Returns a pointer to the field causing the sensor to trigger, or \c
  NULL if the change didn't start at a field.

  Only valid for immediate sensors (will return \c NULL otherwise),
  for the same reason as described for SoDataSensor::getTriggerNode().

  The result is only valid within the scope of a trigger(), so if you
  need to use the pointer outside your sensor callback, you must store
  it.
*/
SoField *
SoDataSensor::getTriggerField(void) const
{
  return this->triggerfield;
}

/*!
  Returns a pointer to the path from the node under the surveillance
  of this sensor (either directly or indirectly through a field
  watch) down to the node which caused the sensor to be triggered.

  Will only work for immediate mode sensors, for the same reason
  explained under getTriggerNode().

  The resulting path is only valid within the scope of trigger(), so
  if you need to use the path outside your sensor callback, you must
  store the pointer and call SoPath::ref() to avoid its destruction at
  the end of SoDataSensor::trigger().
*/
SoPath *
SoDataSensor::getTriggerPath(void) const
{
  return this->triggerpath;
}

/*!
  This flag indicates whether or not the path should be queried
  whenever a node triggers the data sensor.

  This flag is provided because finding a node path through a scene
  graph is an expensive operation.

  \sa getTriggerPathFlag(), getTriggerPath()
 */
void
SoDataSensor::setTriggerPathFlag(SbBool flag)
{
  this->findpath = flag;
}

/*!
  Returns whether or not any node induced trigger operations will make
  the sensor find the path of the node which caused it.

  \sa setTriggerPathFlag(), getTriggerPath()
 */
SbBool
SoDataSensor::getTriggerPathFlag(void) const
{
  return this->findpath;
}

// Doc from superclass.
void
SoDataSensor::trigger(void)
{
  inherited::trigger();
  this->triggerfield = NULL;
  this->triggernode = NULL;
  if (this->triggerpath) this->triggerpath->unref();
  this->triggerpath = NULL;
}

/*!
  Called from entity we are monitoring when it changes.

  If this is an immediate sensor, the field and node (if any) causing
  the change will be stored and can be fetched by getTriggerField()
  and getTriggerNode(). If the triggerpath flag has been set, the path
  down to the node is also found and stored for later retrieval by
  getTriggerPath().

  \sa setTriggerPathFlag()
*/
void
SoDataSensor::notify(SoNotList * l)
{
  if (this->getPriority() == 0) {
    this->triggerfield = l->getLastField();
    SoNotRec * record = l->getFirstRecAtNode();
    this->triggernode = (SoNode *) (record ? record->getBase() : NULL);

    if (this->findpath && this->triggernode) {
      SoNotRec * lastrec = l->getLastRec();
      SoType t = lastrec->getBase()->getTypeId();
      if (t.isDerivedFrom(SoNode::getClassTypeId())) {
        SoSearchAction search;
        search.setNode(this->triggernode);
        search.setSearchingAll(TRUE);
        search.apply((SoNode *)lastrec->getBase());
        if (search.isFound()) {
          this->triggerpath = search.getPath();
          this->triggerpath->ref();
        }
      }
    }
  }
}

/*!
  Runs the callback set in setDeleteCallback().

  Called from subclasses when the entity we're monitoring is about to
  be deleted.
*/
void
SoDataSensor::invokeDeleteCallback(void)
{
  if (this->cbfunc) this->cbfunc(this->cbdata, this);
}
