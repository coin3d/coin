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

#include <Inventor/draggers/SoHandleBoxDragger.h>


SO_KIT_SOURCE(SoHandleBoxDragger);


void
SoHandleBoxDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoHandleBoxDragger);
}

SoHandleBoxDragger::SoHandleBoxDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoHandleBoxDragger);

  // FIXME: SO_KIT_ADD_CATALOG_ENTRY for all entries in this dragger's
  // catalog. 20000107 mortene.
  // arrow1
  // arrow1Switch
  // arrow2
  // arrow2Switch
  // arrow3
  // arrow3Switch
  // arrow4
  // arrow4Switch
  // arrow5
  // arrow5Switch
  // arrow6
  // arrow6Switch
  // arrowTranslation
  // drawStyle
  // extruder1
  // extruder1Active
  // extruder1Switch
  // extruder2
  // extruder2Active
  // extruder2Switch
  // extruder3
  // extruder3Active
  // extruder3Switch
  // extruder4
  // extruder4Active
  // extruder4Switch
  // extruder5
  // extruder5Active
  // extruder5Switch
  // extruder6
  // extruder6Active
  // extruder6Switch
  // surroundScale
  // translator1
  // translator1Active
  // translator1Switch
  // translator2
  // translator2Active
  // translator2Switch
  // translator3
  // translator3Active
  // translator3Switch
  // translator4
  // translator4Active
  // translator4Switch
  // translator5
  // translator5Active
  // translator5Switch
  // translator6
  // translator6Active
  // translator6Switch
  // uniform1
  // uniform1Active
  // uniform1Switch
  // uniform2
  // uniform2Active
  // uniform2Switch
  // uniform3
  // uniform3Active
  // uniform3Switch
  // uniform4
  // uniform4Active
  // uniform4Switch
  // uniform5
  // uniform5Active
  // uniform5Switch
  // uniform6
  // uniform6Active
  // uniform6Switch
  // uniform7
  // uniform7Active
  // uniform7Switch
  // uniform8
  // uniform8Active
  // uniform8Switch

  SO_NODE_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));

  SO_KIT_INIT_INSTANCE();
}


SoHandleBoxDragger::~SoHandleBoxDragger()
{
  COIN_STUB();
}

SbBool
SoHandleBoxDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  COIN_STUB();
  return FALSE;
}

void
SoHandleBoxDragger::setDefaultOnNonWritingFields(void)
{
  COIN_STUB();
}

void
SoHandleBoxDragger::fieldSensorCB(void * f, SoSensor * s)
{
  COIN_STUB();
}

void
SoHandleBoxDragger::valueChangedCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoHandleBoxDragger::startCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoHandleBoxDragger::motionCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoHandleBoxDragger::finishCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoHandleBoxDragger::metaKeyChangeCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoHandleBoxDragger::dragStart(void)
{
  COIN_STUB();
}

void
SoHandleBoxDragger::drag(void)
{
  COIN_STUB();
}

void
SoHandleBoxDragger::dragFinish(void)
{
  COIN_STUB();
}

void
SoHandleBoxDragger::setAllPartsActive(SbBool onoroff)
{
  COIN_STUB();
}
