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


SO_KIT_SOURCE(SoDragPointDragger);


void
SoDragPointDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoDragPointDragger);
}

SoDragPointDragger::SoDragPointDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoDragPointDragger);

  // FIXME:
  // SO_KIT_ADD_CATALOG_ENTRY for:
  // noRotSep
  // planeFeedbackSep
  // planeFeedbackSwitch
  // planeFeedbackTranslation
  // rotX
  // rotXSep
  // rotY
  // rotYSep
  // rotZ
  // rotZSep
  // xFeedback
  // xFeedbackSep
  // xFeedbackSwitch
  // xFeedbackTranslation
  // xTranslator
  // xTranslatorSwitch
  // xyFeedback
  // xyTranslator
  // xyTranslatorSwitch
  // xzFeedback
  // xzTranslator
  // xzTranslatorSwitch
  // yFeedback
  // yFeedbackSep
  // yFeedbackSwitch
  // yFeedbackTranslation
  // yTranslator
  // yTranslatorSwitch
  // yzFeedback
  // yzTranslator
  // yzTranslatorSwitch
  // zFeedback
  // zFeedbackSep
  // zFeedbackSwitch
  // zFeedbackTranslation
  // zTranslator
  // zTranslatorSwitch
  //
  //     20000107 mortene

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
