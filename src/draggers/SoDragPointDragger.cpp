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

#include <Inventor/draggers/SoDragPointDragger.h>
#include <Inventor/draggers/SoTranslate1Dragger.h>
#include <Inventor/draggers/SoTranslate2Dragger.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTranslation.h>


SO_KIT_SOURCE(SoDragPointDragger);


void
SoDragPointDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoDragPointDragger);
}

SoDragPointDragger::SoDragPointDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoDragPointDragger);

  SO_KIT_ADD_CATALOG_ENTRY(noRotSep, SoSeparator, FALSE, topSeparator, rotXSep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(planeFeedbackSep, SoSeparator, FALSE, topSeparator, geomSeparator, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(planeFeedbackSwitch, SoSwitch, FALSE, planeFeedbackSep, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(planeFeedbackTranslation, SoTranslation, FALSE, planeFeedbackSep, planeFeedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotX, SoRotation, TRUE, rotXSep, xzTranslatorSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotXSep, SoSeparator, FALSE, topSeparator, rotYSep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotY, SoRotation, TRUE, rotYSep, zTranslatorSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotYSep, SoSeparator, FALSE, topSeparator, rotZSep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotZ, SoRotation, TRUE, rotZSep, yTranslatorSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotZSep, SoSeparator, FALSE, topSeparator, xFeedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(xFeedback, SoSeparator, TRUE, xFeedbackSep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(xFeedbackSep, SoSeparator, FALSE, xFeedbackSwitch, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(xFeedbackSwitch, SoSwitch, FALSE, topSeparator, yFeedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(xFeedbackTranslation, SoTranslation, FALSE, xFeedbackSep, xFeedback, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(xTranslator, SoTranslate1Dragger, TRUE, xTranslatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(xTranslatorSwitch, SoSwitch, FALSE, noRotSep, xyTranslatorSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(xyFeedback, SoSeparator, TRUE, planeFeedbackSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(xyTranslator, SoTranslate2Dragger, TRUE, xyTranslatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(xyTranslatorSwitch, SoSwitch, FALSE, noRotSep, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(xzFeedback, SoSeparator, TRUE, planeFeedbackSwitch, xyFeedback, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(xzTranslator, SoTranslate2Dragger, TRUE, xzTranslatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(xzTranslatorSwitch, SoSwitch, FALSE, rotXSep, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(yFeedback, SoSeparator, TRUE, yFeedbackSep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(yFeedbackSep, SoSeparator, FALSE, yFeedbackSwitch, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(yFeedbackSwitch, SoSwitch, FALSE, topSeparator, zFeedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(yFeedbackTranslation, SoTranslation, FALSE, yFeedbackSep, yFeedback, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(yTranslator, SoTranslate1Dragger, TRUE, yTranslatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(yTranslatorSwitch, SoSwitch, FALSE, rotZSep, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(yzFeedback, SoSeparator, TRUE, planeFeedbackSwitch, xzFeedback, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(yzTranslator, SoTranslate2Dragger, TRUE, yzTranslatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(yzTranslatorSwitch, SoSwitch, FALSE, rotYSep, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(zFeedback, SoSeparator, TRUE, zFeedbackSep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(zFeedbackSep, SoSeparator, FALSE, zFeedbackSwitch, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(zFeedbackSwitch, SoSwitch, FALSE, topSeparator, planeFeedbackSep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(zFeedbackTranslation, SoTranslation, FALSE, zFeedbackSep, zFeedback, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(zTranslator, SoTranslate1Dragger, TRUE, zTranslatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(zTranslatorSwitch, SoSwitch, FALSE, rotYSep, yzTranslatorSwitch, FALSE);


  SO_NODE_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));

  SO_KIT_INIT_INSTANCE();
}


SoDragPointDragger::~SoDragPointDragger()
{
  COIN_STUB();
}

SbBool
SoDragPointDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  COIN_STUB();
  return FALSE;
}

void
SoDragPointDragger::setDefaultOnNonWritingFields(void)
{
  COIN_STUB();
}

void
SoDragPointDragger::fieldSensorCB(void * f, SoSensor * s)
{
  COIN_STUB();
}

void
SoDragPointDragger::valueChangedCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoDragPointDragger::setJumpLimit(float limit)
{
  COIN_STUB();
}

float
SoDragPointDragger::getJumpLimit(void) const
{
  COIN_STUB();
  return 0.0f;
}

void
SoDragPointDragger::showNextDraggerSet(void)
{
  COIN_STUB();
}

void
SoDragPointDragger::dragStart(void)
{
  COIN_STUB();
}

void
SoDragPointDragger::drag(void)
{
  COIN_STUB();
}

void
SoDragPointDragger::dragFinish(void)
{
  COIN_STUB();
}

void
SoDragPointDragger::startCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoDragPointDragger::motionCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoDragPointDragger::finishCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoDragPointDragger::metaKeyChangeCB(void * f, SoDragger * d)
{
  COIN_STUB();
}
