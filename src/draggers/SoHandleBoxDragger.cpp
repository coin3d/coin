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
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTranslation.h>


SO_KIT_SOURCE(SoHandleBoxDragger);


void
SoHandleBoxDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoHandleBoxDragger);
}

SoHandleBoxDragger::SoHandleBoxDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoHandleBoxDragger);

  SO_KIT_ADD_CATALOG_ENTRY(surroundScale, SoSurroundScale, TRUE, topSeparator, geomSeparator, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(drawStyle, SoDrawStyle, TRUE, geomSeparator, translator1Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator1Switch, SoSwitch, TRUE, geomSeparator, translator2Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator1, SoSeparator, TRUE, translator1Switch, translator1Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator1Active, SoSeparator, TRUE, translator1Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator2Switch, SoSwitch, TRUE, geomSeparator, translator3Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator2, SoSeparator, TRUE, translator2Switch, translator2Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator2Active, SoSeparator, TRUE, translator2Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator3Switch, SoSwitch, TRUE, geomSeparator, translator4Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator3, SoSeparator, TRUE, translator3Switch, translator3Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator3Active, SoSeparator, TRUE, translator3Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator4Switch, SoSwitch, TRUE, geomSeparator, translator5Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator4, SoSeparator, TRUE, translator4Switch, translator4Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator4Active, SoSeparator, TRUE, translator4Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator5Switch, SoSwitch, TRUE, geomSeparator, translator6Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator5, SoSeparator, TRUE, translator5Switch, translator5Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator5Active, SoSeparator, TRUE, translator5Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator6Switch, SoSwitch, TRUE, geomSeparator, extruder1Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator6, SoSeparator, TRUE, translator6Switch, translator6Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator6Active, SoSeparator, TRUE, translator6Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder1Switch, SoSwitch, TRUE, geomSeparator, extruder2Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder1, SoSeparator, TRUE, extruder1Switch, extruder1Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder1Active, SoSeparator, TRUE, extruder1Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder2Switch, SoSwitch, TRUE, geomSeparator, extruder3Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder2, SoSeparator, TRUE, extruder2Switch, extruder2Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder2Active, SoSeparator, TRUE, extruder2Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder3Switch, SoSwitch, TRUE, geomSeparator, extruder4Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder3, SoSeparator, TRUE, extruder3Switch, extruder3Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder3Active, SoSeparator, TRUE, extruder3Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder4Switch, SoSwitch, TRUE, geomSeparator, extruder5Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder4, SoSeparator, TRUE, extruder4Switch, extruder4Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder4Active, SoSeparator, TRUE, extruder4Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder5Switch, SoSwitch, TRUE, geomSeparator, extruder6Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder5, SoSeparator, TRUE, extruder5Switch, extruder5Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder5Active, SoSeparator, TRUE, extruder5Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder6Switch, SoSwitch, TRUE, geomSeparator, uniform1Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder6, SoSeparator, TRUE, extruder6Switch, extruder6Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(extruder6Active, SoSeparator, TRUE, extruder6Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform1Switch, SoSwitch, TRUE, geomSeparator, uniform2Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform1, SoSeparator, TRUE, uniform1Switch, uniform1Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform1Active, SoSeparator, TRUE, uniform1Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform2Switch, SoSwitch, TRUE, geomSeparator, uniform3Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform2, SoSeparator, TRUE, uniform2Switch, uniform2Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform2Active, SoSeparator, TRUE, uniform2Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform3Switch, SoSwitch, TRUE, geomSeparator, uniform4Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform3, SoSeparator, TRUE, uniform3Switch, uniform3Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform3Active, SoSeparator, TRUE, uniform3Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform4Switch, SoSwitch, TRUE, geomSeparator, uniform5Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform4, SoSeparator, TRUE, uniform4Switch, uniform4Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform4Active, SoSeparator, TRUE, uniform4Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform5Switch, SoSwitch, TRUE, geomSeparator, uniform6Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform5, SoSeparator, TRUE, uniform5Switch, uniform5Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform5Active, SoSeparator, TRUE, uniform5Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform6Switch, SoSwitch, TRUE, geomSeparator, uniform7Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform6, SoSeparator, TRUE, uniform6Switch, uniform6Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform6Active, SoSeparator, TRUE, uniform6Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform7Switch, SoSwitch, TRUE, geomSeparator, uniform8Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform7, SoSeparator, TRUE, uniform7Switch, uniform7Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform7Active, SoSeparator, TRUE, uniform7Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform8Switch, SoSwitch, TRUE, geomSeparator, arrowTranslation, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform8, SoSeparator, TRUE, uniform8Switch, uniform8Active, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(uniform8Active, SoSeparator, TRUE, uniform8Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(arrowTranslation, SoTranslation, TRUE, geomSeparator, arrow1Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(arrow1Switch, SoSwitch, TRUE, geomSeparator, arrow2Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(arrow1, SoSeparator, TRUE, arrow1Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(arrow2Switch, SoSwitch, TRUE, geomSeparator, arrow3Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(arrow2, SoSeparator, TRUE, arrow2Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(arrow3Switch, SoSwitch, TRUE, geomSeparator, arrow4Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(arrow3, SoSeparator, TRUE, arrow3Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(arrow4Switch, SoSwitch, TRUE, geomSeparator, arrow5Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(arrow4, SoSeparator, TRUE, arrow4Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(arrow5Switch, SoSwitch, TRUE, geomSeparator, arrow6Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(arrow5, SoSeparator, TRUE, arrow5Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(arrow6Switch, SoSwitch, TRUE, geomSeparator, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(arrow6, SoSeparator, TRUE, arrow6Switch, "", TRUE);

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
