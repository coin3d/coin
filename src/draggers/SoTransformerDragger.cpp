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

#include <Inventor/draggers/SoTransformerDragger.h>
#include <Inventor/SbVec2f.h>


SO_KIT_SOURCE(SoTransformerDragger);


void
SoTransformerDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoTransformerDragger);
}

SoTransformerDragger::SoTransformerDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoTransformerDragger);

  // FIXME: SO_KIT_ADD_CATALOG_ENTRY for all entries in this dragger's
  // catalog. 20000107 mortene.
  // axisFeedbackLocation
  // axisFeedbackSep
  // circleFeedbackAntiSquish
  // circleFeedbackSep
  // circleFeedbackTransform
  // circleFeedbackTransformSwitch
  // negXRoundWallFeedback
  // negXWallFeedback
  // negXWallFeedbackSwitch
  // negYRoundWallFeedback
  // negYWallFeedback
  // negYWallFeedbackSwitch
  // negZRoundWallFeedback
  // negZWallFeedback
  // negZWallFeedbackSwitch
  // overallStyle
  // posXRoundWallFeedback
  // posXWallFeedback
  // posXWallFeedbackSwitch
  // posYRoundWallFeedback
  // posYWallFeedback
  // posYWallFeedbackSwitch
  // posZRoundWallFeedback
  // posZWallFeedback
  // posZWallFeedbackSwitch
  // radialFeedback
  // radialFeedbackSwitch
  // rotator1
  // rotator1Active
  // rotator1LocateGroup
  // rotator1Switch
  // rotator2
  // rotator2Active
  // rotator2LocateGroup
  // rotator2Switch
  // rotator3
  // rotator3Active
  // rotator3LocateGroup
  // rotator3Switch
  // rotator4
  // rotator4Active
  // rotator4LocateGroup
  // rotator4Switch
  // rotator5
  // rotator5Active
  // rotator5LocateGroup
  // rotator5Switch
  // rotator6
  // rotator6Active
  // rotator6LocateGroup
  // rotator6Switch
  // rotatorSep
  // scale1
  // scale1Active
  // scale1LocateGroup
  // scale1Switch
  // scale2
  // scale2Active
  // scale2LocateGroup
  // scale2Switch
  // scale3
  // scale3Active
  // scale3LocateGroup
  // scale3Switch
  // scale4
  // scale4Active
  // scale4LocateGroup
  // scale4Switch
  // scale5
  // scale5Active
  // scale5LocateGroup
  // scale5Switch
  // scale6
  // scale6Active
  // scale6LocateGroup
  // scale6Switch
  // scale7
  // scale7Active
  // scale7LocateGroup
  // scale7Switch
  // scale8
  // scale8Active
  // scale8LocateGroup
  // scale8Switch
  // scaleBoxFeedback
  // scaleBoxFeedbackSwitch
  // scaleSep
  // surroundScale
  // translateBoxFeedback
  // translateBoxFeedbackRotation
  // translateBoxFeedbackSep
  // translateBoxFeedbackSwitch
  // translator1
  // translator1Active
  // translator1LocateGroup
  // translator1Switch
  // translator2
  // translator2Active
  // translator2LocateGroup
  // translator2Switch
  // translator3
  // translator3Active
  // translator3LocateGroup
  // translator3Switch
  // translator4
  // translator4Active
  // translator4LocateGroup
  // translator4Switch
  // translator5
  // translator5Active
  // translator5LocateGroup
  // translator5Switch
  // translator6
  // translator6Active
  // translator6LocateGroup
  // translator6Switch
  // translatorSep
  // xAxisFeedbackActive
  // xAxisFeedbackSelect
  // xAxisFeedbackSwitch
  // xCircleFeedback
  // xCircleFeedbackSwitch
  // xCrosshairFeedback
  // yAxisFeedbackActive
  // yAxisFeedbackSelect
  // yAxisFeedbackSwitch
  // yCircleFeedback
  // yCircleFeedbackSwitch
  // yCrosshairFeedback
  // zAxisFeedbackActive
  // zAxisFeedbackSelect
  // zAxisFeedbackSwitch
  // zCircleFeedback
  // zCircleFeedbackSwitch
  // zCrosshairFeedback

  SO_NODE_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));
  SO_NODE_ADD_FIELD(minDiscRotDot, (0.025f));

  SO_KIT_INIT_INSTANCE();
}


SoTransformerDragger::~SoTransformerDragger()
{
  COIN_STUB();
}

SbBool
SoTransformerDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  COIN_STUB();
  return FALSE;
}

void
SoTransformerDragger::setDefaultOnNonWritingFields(void)
{
  COIN_STUB();
}

void
SoTransformerDragger::fieldSensorCB(void * f, SoSensor * s)
{
  COIN_STUB();
}

void
SoTransformerDragger::valueChangedCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

SoTransformerDragger::State
SoTransformerDragger::getCurrentState(void)
{
  COIN_STUB();
  return SoTransformerDragger::INACTIVE;
}

void
SoTransformerDragger::unsquishKnobs(void)
{
  COIN_STUB();
}

SbBool
SoTransformerDragger::isLocateHighlighting(void)
{
  COIN_STUB();
  return FALSE;
}

void
SoTransformerDragger::setLocateHighlighting(SbBool onoff)
{
  COIN_STUB();
}

void
SoTransformerDragger::setColinearThreshold(int newval)
{
  COIN_STUB();
}

int
SoTransformerDragger::getColinearThreshold(void)
{
  COIN_STUB();
  return -1;
}

SbVec3f
SoTransformerDragger::getBoxPointInWorldSpace(const SbVec3f & pointonunitbox)
{
  COIN_STUB();
  return SbVec3f(0.0f, 0.0f, 0.0f);
}

SbVec3f
SoTransformerDragger::getBoxDirInWorldSpace(const SbVec3f & dironunitbox)
{
  COIN_STUB();
  return SbVec3f(0.0f, 0.0f, 0.0f);
}

SbVec3f
SoTransformerDragger::getWorldPointInBoxSpace(const SbVec3f & pointinworldspace)
{
  COIN_STUB();
  return SbVec3f(0.0f, 0.0f, 0.0f);
}

SbVec2f
SoTransformerDragger::getWorldPointInPixelSpace(const SbVec3f & thepoint)
{
  COIN_STUB();
  return SbVec2f(0.0f, 0.0f);
}

SbVec3f
SoTransformerDragger::getInteractiveCenterInBoxSpace(void)
{
  COIN_STUB();
  return SbVec3f(0.0f, 0.0f, 0.0f);
}

void
SoTransformerDragger::startCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoTransformerDragger::motionCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoTransformerDragger::finishCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoTransformerDragger::metaKeyChangeCB(void *, SoDragger *)
{
  COIN_STUB();
}

void
SoTransformerDragger::dragStart(void)
{
  COIN_STUB();
}

void
SoTransformerDragger::drag(void)
{
  COIN_STUB();
}

void
SoTransformerDragger::dragFinish(void)
{
  COIN_STUB();
}

void
SoTransformerDragger::updateAntiSquishList(void)
{
  COIN_STUB();
}

void
SoTransformerDragger::setAllPartSwitches(int scalewhich, int rotatewhich, int translatewhich)
{
  COIN_STUB();
}

int
SoTransformerDragger::getMouseGestureDirection(SbBool x_ok, SbBool y_ok, SbBool z_ok)
{
  COIN_STUB();
  return -1;
}

int
SoTransformerDragger::getIgnoreAxis(SbVec2f axis[3][2], SbBool x_ok, SbBool y_ok, SbBool z_ok)
{
  COIN_STUB();
  return -1;
}

void
SoTransformerDragger::makeMinorAxisPerpendicularIfColinear(SbVec2f origin, SbVec2f axisends[3][2], int index_a, int index_b)
{
  COIN_STUB();
}

SbBool
SoTransformerDragger::isColinear(SbVec2f a1[2], SbVec2f a2[2], int pixels)
{
  COIN_STUB();
  return FALSE;
}
