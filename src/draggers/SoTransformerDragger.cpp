/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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
#include <Inventor/nodes/SoAntiSquish.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoLocateHighlight.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoTranslation.h>


SO_KIT_SOURCE(SoTransformerDragger);


void
SoTransformerDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoTransformerDragger);
}

SoTransformerDragger::SoTransformerDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoTransformerDragger);

  SO_KIT_ADD_CATALOG_ENTRY(surroundScale, SoSurroundScale, TRUE, topSeparator, overallStyle, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(overallStyle, SoGroup, TRUE, topSeparator, geomSeparator, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translatorSep, SoSeparator, TRUE, topSeparator, rotatorSep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator1Switch, SoSwitch, TRUE, translatorSep, translator2Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator1LocateGroup, SoLocateHighlight, TRUE, translator1Switch, translator1Active, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator1, SoSeparator, TRUE, translator1LocateGroup, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator1Active, SoSeparator, TRUE, translator1Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator2Switch, SoSwitch, TRUE, translatorSep, translator3Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator2LocateGroup, SoLocateHighlight, TRUE, translator2Switch, translator2Active, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator2, SoSeparator, TRUE, translator2LocateGroup, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator2Active, SoSeparator, TRUE, translator2Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator3Switch, SoSwitch, TRUE, translatorSep, translator4Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator3LocateGroup, SoLocateHighlight, TRUE, translator3Switch, translator3Active, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator3, SoSeparator, TRUE, translator3LocateGroup, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator3Active, SoSeparator, TRUE, translator3Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator4Switch, SoSwitch, TRUE, translatorSep, translator5Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator4LocateGroup, SoLocateHighlight, TRUE, translator4Switch, translator4Active, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator4, SoSeparator, TRUE, translator4LocateGroup, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator4Active, SoSeparator, TRUE, translator4Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator5Switch, SoSwitch, TRUE, translatorSep, translator6Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator5LocateGroup, SoLocateHighlight, TRUE, translator5Switch, translator5Active, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator5, SoSeparator, TRUE, translator5LocateGroup, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator5Active, SoSeparator, TRUE, translator5Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator6Switch, SoSwitch, TRUE, translatorSep, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator6LocateGroup, SoLocateHighlight, TRUE, translator6Switch, translator6Active, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator6, SoSeparator, TRUE, translator6LocateGroup, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator6Active, SoSeparator, TRUE, translator6Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotatorSep, SoSeparator, TRUE, topSeparator, scaleSep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator1Switch, SoSwitch, TRUE, rotatorSep, rotator2Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator1LocateGroup, SoLocateHighlight, TRUE, rotator1Switch, rotator1Active, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator1, SoSeparator, TRUE, rotator1LocateGroup, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator1Active, SoSeparator, TRUE, rotator1Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator2Switch, SoSwitch, TRUE, rotatorSep, rotator3Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator2LocateGroup, SoLocateHighlight, TRUE, rotator2Switch, rotator2Active, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator2, SoSeparator, TRUE, rotator2LocateGroup, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator2Active, SoSeparator, TRUE, rotator2Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator3Switch, SoSwitch, TRUE, rotatorSep, rotator4Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator3LocateGroup, SoLocateHighlight, TRUE, rotator3Switch, rotator3Active, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator3, SoSeparator, TRUE, rotator3LocateGroup, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator3Active, SoSeparator, TRUE, rotator3Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator4Switch, SoSwitch, TRUE, rotatorSep, rotator5Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator4LocateGroup, SoLocateHighlight, TRUE, rotator4Switch, rotator4Active, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator4, SoSeparator, TRUE, rotator4LocateGroup, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator4Active, SoSeparator, TRUE, rotator4Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator5Switch, SoSwitch, TRUE, rotatorSep, rotator6Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator5LocateGroup, SoLocateHighlight, TRUE, rotator5Switch, rotator5Active, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator5, SoSeparator, TRUE, rotator5LocateGroup, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator5Active, SoSeparator, TRUE, rotator5Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator6Switch, SoSwitch, TRUE, rotatorSep, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator6LocateGroup, SoLocateHighlight, TRUE, rotator6Switch, rotator6Active, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator6, SoSeparator, TRUE, rotator6LocateGroup, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator6Active, SoSeparator, TRUE, rotator6Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scaleSep, SoSeparator, TRUE, topSeparator, circleFeedbackSep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scale1Switch, SoSwitch, TRUE, scaleSep, scale2Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scale1LocateGroup, SoLocateHighlight, TRUE, scale1Switch, scale1Active, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scale1, SoSeparator, TRUE, scale1LocateGroup, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scale1Active, SoSeparator, TRUE, scale1Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scale2Switch, SoSwitch, TRUE, scaleSep, scale3Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scale2LocateGroup, SoLocateHighlight, TRUE, scale2Switch, scale2Active, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scale2, SoSeparator, TRUE, scale2LocateGroup, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scale2Active, SoSeparator, TRUE, scale2Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scale3Switch, SoSwitch, TRUE, scaleSep, scale4Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scale3LocateGroup, SoLocateHighlight, TRUE, scale3Switch, scale3Active, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scale3, SoSeparator, TRUE, scale3LocateGroup, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scale3Active, SoSeparator, TRUE, scale3Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scale4Switch, SoSwitch, TRUE, scaleSep, scale5Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scale4LocateGroup, SoLocateHighlight, TRUE, scale4Switch, scale4Active, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scale4, SoSeparator, TRUE, scale4LocateGroup, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scale4Active, SoSeparator, TRUE, scale4Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scale5Switch, SoSwitch, TRUE, scaleSep, scale6Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scale5LocateGroup, SoLocateHighlight, TRUE, scale5Switch, scale5Active, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scale5, SoSeparator, TRUE, scale5LocateGroup, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scale5Active, SoSeparator, TRUE, scale5Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scale6Switch, SoSwitch, TRUE, scaleSep, scale7Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scale6LocateGroup, SoLocateHighlight, TRUE, scale6Switch, scale6Active, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scale6, SoSeparator, TRUE, scale6LocateGroup, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scale6Active, SoSeparator, TRUE, scale6Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scale7Switch, SoSwitch, TRUE, scaleSep, scale8Switch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scale7LocateGroup, SoLocateHighlight, TRUE, scale7Switch, scale7Active, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scale7, SoSeparator, TRUE, scale7LocateGroup, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scale7Active, SoSeparator, TRUE, scale7Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scale8Switch, SoSwitch, TRUE, scaleSep, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scale8LocateGroup, SoLocateHighlight, TRUE, scale8Switch, scale8Active, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scale8, SoSeparator, TRUE, scale8LocateGroup, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scale8Active, SoSeparator, TRUE, scale8Switch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(axisFeedbackSep, SoSeparator, TRUE, geomSeparator, translateBoxFeedbackSep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(axisFeedbackLocation, SoTranslation, TRUE, axisFeedbackSep, xAxisFeedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(xAxisFeedbackSwitch, SoSwitch, TRUE, axisFeedbackSep, yAxisFeedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(xAxisFeedbackActive, SoSeparator, TRUE, xAxisFeedbackSwitch, xAxisFeedbackSelect, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(xAxisFeedbackSelect, SoSeparator, TRUE, xAxisFeedbackSwitch, xCrosshairFeedback, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(xCrosshairFeedback, SoSeparator, TRUE, xAxisFeedbackSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(yAxisFeedbackSwitch, SoSwitch, TRUE, axisFeedbackSep, zAxisFeedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(yAxisFeedbackActive, SoSeparator, TRUE, yAxisFeedbackSwitch, yAxisFeedbackSelect, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(yAxisFeedbackSelect, SoSeparator, TRUE, yAxisFeedbackSwitch, yCrosshairFeedback, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(yCrosshairFeedback, SoSeparator, TRUE, yAxisFeedbackSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(zAxisFeedbackSwitch, SoSwitch, TRUE, axisFeedbackSep, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(zAxisFeedbackActive, SoSeparator, TRUE, zAxisFeedbackSwitch, zAxisFeedbackSelect, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(zAxisFeedbackSelect, SoSeparator, TRUE, zAxisFeedbackSwitch, zCrosshairFeedback, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(zCrosshairFeedback, SoSeparator, TRUE, zAxisFeedbackSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translateBoxFeedbackSep, SoSeparator, TRUE, geomSeparator, scaleBoxFeedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translateBoxFeedbackSwitch, SoSwitch, TRUE, translateBoxFeedbackSep, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translateBoxFeedbackRotation, SoRotation, TRUE, translateBoxFeedbackSwitch, translateBoxFeedback, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translateBoxFeedback, SoSeparator, TRUE, translateBoxFeedbackSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scaleBoxFeedbackSwitch, SoSwitch, TRUE, geomSeparator, posXWallFeedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scaleBoxFeedback, SoSeparator, TRUE, scaleBoxFeedbackSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(posXWallFeedbackSwitch, SoSwitch, TRUE, geomSeparator, posYWallFeedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(posXWallFeedback, SoSeparator, TRUE, posXWallFeedbackSwitch, posXRoundWallFeedback, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(posXRoundWallFeedback, SoSeparator, TRUE, posXWallFeedbackSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(posYWallFeedbackSwitch, SoSwitch, TRUE, geomSeparator, posZWallFeedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(posYWallFeedback, SoSeparator, TRUE, posYWallFeedbackSwitch, posYRoundWallFeedback, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(posYRoundWallFeedback, SoSeparator, TRUE, posYWallFeedbackSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(posZWallFeedbackSwitch, SoSwitch, TRUE, geomSeparator, negXWallFeedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(posZWallFeedback, SoSeparator, TRUE, posZWallFeedbackSwitch, posZRoundWallFeedback, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(posZRoundWallFeedback, SoSeparator, TRUE, posZWallFeedbackSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(negXWallFeedbackSwitch, SoSwitch, TRUE, geomSeparator, negYWallFeedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(negXWallFeedback, SoSeparator, TRUE, negXWallFeedbackSwitch, negXRoundWallFeedback, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(negXRoundWallFeedback, SoSeparator, TRUE, negXWallFeedbackSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(negYWallFeedbackSwitch, SoSwitch, TRUE, geomSeparator, negZWallFeedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(negYWallFeedback, SoSeparator, TRUE, negYWallFeedbackSwitch, negYRoundWallFeedback, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(negYRoundWallFeedback, SoSeparator, TRUE, negYWallFeedbackSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(negZWallFeedbackSwitch, SoSwitch, TRUE, geomSeparator, radialFeedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(negZWallFeedback, SoSeparator, TRUE, negZWallFeedbackSwitch, negZRoundWallFeedback, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(negZRoundWallFeedback, SoSeparator, TRUE, negZWallFeedbackSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(radialFeedbackSwitch, SoSwitch, TRUE, geomSeparator, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(radialFeedback, SoSeparator, TRUE, radialFeedbackSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(circleFeedbackSep, SoSeparator, TRUE, topSeparator, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(circleFeedbackTransformSwitch, SoSwitch, TRUE, circleFeedbackSep, xCircleFeedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(circleFeedbackAntiSquish, SoAntiSquish, TRUE, circleFeedbackTransformSwitch, circleFeedbackTransform, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(circleFeedbackTransform, SoTransform, TRUE, circleFeedbackTransformSwitch, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(xCircleFeedbackSwitch, SoSwitch, TRUE, circleFeedbackSep, yCircleFeedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(xCircleFeedback, SoSeparator, TRUE, xCircleFeedbackSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(yCircleFeedbackSwitch, SoSwitch, TRUE, circleFeedbackSep, zCircleFeedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(yCircleFeedback, SoSeparator, TRUE, yCircleFeedbackSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(zCircleFeedbackSwitch, SoSwitch, TRUE, circleFeedbackSep, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(zCircleFeedback, SoSeparator, TRUE, zCircleFeedbackSwitch, "", TRUE);

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
