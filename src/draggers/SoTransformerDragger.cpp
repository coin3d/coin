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
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/nodes/SoAntiSquish.h>
#include <Inventor/nodes/SoLocateHighlight.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/projectors/SbPlaneProjector.h>
#include <Inventor/projectors/SbLineProjector.h>
#include <Inventor/projectors/SbSphereSectionProjector.h>
#include <Inventor/projectors/SbCylinderPlaneProjector.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/lists/SoPathList.h>

#include <coindefs.h> // COIN_STUB()


#define WHATKIND_NONE      0
#define WHATKIND_SCALE     1
#define WHATKIND_TRANSLATE 2
#define WHATKIND_ROTATE    3

#define CONSTRAINT_OFF  0
#define CONSTRAINT_WAIT 1
#define CONSTRAINT_X    2
#define CONSTRAINT_Y    3
#define CONSTRAINT_Z    4

#define KNOB_DISTANCE 1.25f   // distance from center to rotate-knobs

int SoTransformerDragger::colinearThreshold = 3; // FIXME: find default value from somewhere


// FIXME, bugs or missing features (pederb, 20000224):
// o when appending a rotation and scale is not uniform, the cube
//   is sometimes sheared. Seems to be a scaleorientation problem.
// o some feedback is missing (mostly crosshair)
// o detect if disc or cylinder rotator should be used (disc-only right now)
// o not possible to go from free rotate to disc/cylinder rotate.
// o when translating, the positive-y face is always "active". weird...
//
// Also the translation feedback is a bit different from OIV. Coin
// always places the feedback axes at the center of the face being
// translated. OIV places them at the picked point. I think our
// strategy is better, since when switching between constrained
// translations and unconstrained translation, the OIV feedback axes
// can easily be positioned outside the face being dragged.
//
SO_KIT_SOURCE(SoTransformerDragger);


void
SoTransformerDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoTransformerDragger);
}

void
SoTransformerDragger::build_catalog1(void)
{
  SO_KIT_ADD_CATALOG_ENTRY(surroundScale, SoSurroundScale, TRUE, topSeparator, overallStyle, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(overallStyle, SoGroup, TRUE, topSeparator, geomSeparator, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translatorSep, SoSeparator, TRUE, topSeparator, rotatorSep, FALSE);
}

void
SoTransformerDragger::build_catalog2(void)
{
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
}

void
SoTransformerDragger::build_catalog3(void)
{
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
}

void
SoTransformerDragger::build_catalog4(void)
{
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
}

void
SoTransformerDragger::build_catalog5(void)
{
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
}

void
SoTransformerDragger::build_catalog6(void)
{
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
}

SoTransformerDragger::SoTransformerDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoTransformerDragger);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    // split-up to avoid one huge method
    this->build_catalog1();
    this->build_catalog2();
    this->build_catalog3();
    this->build_catalog4();
    this->build_catalog5();
    this->build_catalog6();
  }


  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoInteractionKit::readDefaultParts("transformerDragger.iv", NULL, 0);
  }

  SO_NODE_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));
  SO_NODE_ADD_FIELD(minDiscRotDot, (0.025f));

  SO_KIT_INIT_INSTANCE();

  this->setPartAsDefault("overallStyle", "transformerOverallStyle");
  this->setPartAsDefault("translator1", "transformerTranslator1");
  this->setPartAsDefault("translator2", "transformerTranslator2");
  this->setPartAsDefault("translator3", "transformerTranslator3");
  this->setPartAsDefault("translator4", "transformerTranslator4");
  this->setPartAsDefault("translator5", "transformerTranslator5");
  this->setPartAsDefault("translator6", "transformerTranslator6");
  this->setPartAsDefault("translator1Active", "transformerTranslator1Active");
  this->setPartAsDefault("translator2Active", "transformerTranslator2Active");
  this->setPartAsDefault("translator3Active", "transformerTranslator3Active");
  this->setPartAsDefault("translator4Active", "transformerTranslator4Active");
  this->setPartAsDefault("translator5Active", "transformerTranslator5Active");
  this->setPartAsDefault("translator6Active", "transformerTranslator6Active");
  this->setPartAsDefault("rotator1", "transformerRotator1");
  this->setPartAsDefault("rotator2", "transformerRotator2");
  this->setPartAsDefault("rotator3", "transformerRotator3");
  this->setPartAsDefault("rotator4", "transformerRotator4");
  this->setPartAsDefault("rotator5", "transformerRotator5");
  this->setPartAsDefault("rotator6", "transformerRotator6");
  this->setPartAsDefault("rotator1Active", "transformerRotator1Active");
  this->setPartAsDefault("rotator2Active", "transformerRotator2Active");
  this->setPartAsDefault("rotator3Active", "transformerRotator3Active");
  this->setPartAsDefault("rotator4Active", "transformerRotator4Active");
  this->setPartAsDefault("rotator5Active", "transformerRotator5Active");
  this->setPartAsDefault("rotator6Active", "transformerRotator6Active");
  this->setPartAsDefault("scale1", "transformerScale1");
  this->setPartAsDefault("scale2", "transformerScale2");
  this->setPartAsDefault("scale3", "transformerScale3");
  this->setPartAsDefault("scale4", "transformerScale4");
  this->setPartAsDefault("scale5", "transformerScale5");
  this->setPartAsDefault("scale6", "transformerScale6");
  this->setPartAsDefault("scale7", "transformerScale7");
  this->setPartAsDefault("scale8", "transformerScale8");
  this->setPartAsDefault("scale1Active", "transformerScale1Active");
  this->setPartAsDefault("scale2Active", "transformerScale2Active");
  this->setPartAsDefault("scale3Active", "transformerScale3Active");
  this->setPartAsDefault("scale4Active", "transformerScale4Active");
  this->setPartAsDefault("scale5Active", "transformerScale5Active");
  this->setPartAsDefault("scale6Active", "transformerScale6Active");
  this->setPartAsDefault("scale7Active", "transformerScale7Active");
  this->setPartAsDefault("scale8Active", "transformerScale8Active");
  this->setPartAsDefault("xAxisFeedbackActive", "transformerXAxisFeedbackActive");
  this->setPartAsDefault("xAxisFeedbackSelect", "transformerXAxisFeedbackSelect");
  this->setPartAsDefault("yAxisFeedbackActive", "transformerYAxisFeedbackActive");
  this->setPartAsDefault("yAxisFeedbackSelect", "transformerYAxisFeedbackSelect");
  this->setPartAsDefault("zAxisFeedbackActive", "transformerZAxisFeedbackActive");
  this->setPartAsDefault("zAxisFeedbackSelect", "transformerZAxisFeedbackSelect");
  this->setPartAsDefault("xCrosshairFeedback", "transformerXCrosshairFeedback");
  this->setPartAsDefault("yCrosshairFeedback", "transformerYCrosshairFeedback");
  this->setPartAsDefault("zCrosshairFeedback", "transformerZCrosshairFeedback");
  this->setPartAsDefault("xCircleFeedback", "transformerXCircleFeedback");
  this->setPartAsDefault("yCircleFeedback", "transformerYCircleFeedback");
  this->setPartAsDefault("zCircleFeedback", "transformerZCircleFeedback");
  this->setPartAsDefault("radialFeedback", "transformerRadialFeedback");
  this->setPartAsDefault("translateBoxFeedback", "transformerTranslateBoxFeedback");

  this->setPartAsDefault("scaleBoxFeedback", "transformerScaleBoxFeedback");
  this->setPartAsDefault("posXWallFeedback", "transformerPosXWallFeedback");
  this->setPartAsDefault("posYWallFeedback", "transformerPosYWallFeedback");
  this->setPartAsDefault("posZWallFeedback", "transformerPosZWallFeedback");
  this->setPartAsDefault("negXWallFeedback", "transformerNegXWallFeedback");
  this->setPartAsDefault("negYWallFeedback", "transformerNegYWallFeedback");
  this->setPartAsDefault("negZWallFeedback", "transformerNegZWallFeedback");
  this->setPartAsDefault("posXRoundWallFeedback", "transformerPosXRoundWallFeedback");
  this->setPartAsDefault("posYRoundWallFeedback", "transformerPosYRoundWallFeedback");
  this->setPartAsDefault("posZRoundWallFeedback", "transformerPosZRoundWallFeedback");
  this->setPartAsDefault("negXRoundWallFeedback", "transformerNegXRoundWallFeedback");
  this->setPartAsDefault("negYRoundWallFeedback", "transformerNegYRoundWallFeedback");
  this->setPartAsDefault("negZRoundWallFeedback", "transformerNegZRoundWallFeedback");

  this->state = INACTIVE;
  this->constraintState = CONSTRAINT_OFF;
  this->locateHighlighting = FALSE;
  this->whatkind = WHATKIND_NONE;
  this->whatnum = -1;

  this->setAllPartSwitches(0, 0, 0);

  this->addStartCallback(SoTransformerDragger::startCB);
  this->addMotionCallback(SoTransformerDragger::motionCB);
  this->addFinishCallback(SoTransformerDragger::finishCB);
  this->addValueChangedCallback(SoTransformerDragger::valueChangedCB);
  this->addOtherEventCallback(SoTransformerDragger::metaKeyChangeCB);

  this->planeProj = new SbPlaneProjector;
  this->lineProj = new SbLineProjector;
  this->sphereProj = new SbSphereSectionProjector;
  this->cylProj = new SbCylinderPlaneProjector;

  this->translFieldSensor = new SoFieldSensor(SoTransformerDragger::fieldSensorCB, this);
  this->translFieldSensor->setPriority(0);
  this->scaleFieldSensor = new SoFieldSensor(SoTransformerDragger::fieldSensorCB, this);
  this->scaleFieldSensor->setPriority(0);
  this->rotateFieldSensor = new SoFieldSensor(SoTransformerDragger::fieldSensorCB, this);
  this->rotateFieldSensor->setPriority(0);

  this->setUpConnections(TRUE, TRUE);
}


SoTransformerDragger::~SoTransformerDragger()
{
  delete this->translFieldSensor;
  delete this->scaleFieldSensor;
  delete this->rotateFieldSensor;
}

SbBool
SoTransformerDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff) return onoff;

  if (onoff) {
    inherited::setUpConnections(onoff, doitalways);

    SoTransformerDragger::fieldSensorCB(this, NULL);

    if (this->translFieldSensor->getAttachedField() != &this->translation) {
      this->translFieldSensor->attach(&this->translation);
    }
    if (this->scaleFieldSensor->getAttachedField() != &this->scaleFactor) {
      this->scaleFieldSensor->attach(&this->scaleFactor);
    }
    if (this->rotateFieldSensor->getAttachedField() != &this->rotation) {
      this->rotateFieldSensor->attach(&this->rotation);
    }
  }
  else {
    if (this->translFieldSensor->getAttachedField() != NULL) {
      this->translFieldSensor->detach();
    }
    if (this->scaleFieldSensor->getAttachedField() != NULL) {
      this->scaleFieldSensor->detach();
    }
    if (this->rotateFieldSensor->getAttachedField() != NULL) {
      this->rotateFieldSensor->detach();
    }
    inherited::setUpConnections(onoff, doitalways);
  }
  return !(this->connectionsSetUp = onoff);
}

void
SoTransformerDragger::setDefaultOnNonWritingFields(void)
{
  COIN_STUB();
  inherited::setDefaultOnNonWritingFields();
}

void
SoTransformerDragger::fieldSensorCB(void *d, SoSensor *)
{
  SoTransformerDragger *thisp = (SoTransformerDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();
  thisp->workFieldsIntoTransform(matrix);
  thisp->setMotionMatrix(matrix);
}

void
SoTransformerDragger::valueChangedCB(void *, SoDragger * d)
{
  SoTransformerDragger *thisp = (SoTransformerDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();
  SbVec3f trans, scale;
  SbRotation rot, scaleOrient;
  matrix.getTransform(trans, rot, scale, scaleOrient);

  thisp->translFieldSensor->detach();
  if (thisp->translation.getValue() != trans)
    thisp->translation = trans;
  thisp->translFieldSensor->attach(&thisp->translation);

  thisp->scaleFieldSensor->detach();
  if (thisp->scaleFactor.getValue() != scale)
    thisp->scaleFactor = scale;
  thisp->scaleFieldSensor->attach(&thisp->scaleFactor);

  thisp->rotateFieldSensor->detach();
  if (thisp->rotation.getValue() != rot) {
    thisp->rotation = rot;
  }
  thisp->rotateFieldSensor->attach(&thisp->rotation);
}

SoTransformerDragger::State
SoTransformerDragger::getCurrentState(void)
{
  return this->state;
}

void
SoTransformerDragger::unsquishKnobs(void)
{
  this->updateAntiSquishList();
}

SbBool
SoTransformerDragger::isLocateHighlighting(void)
{
  return this->locateHighlighting;
}

void
SoTransformerDragger::setLocateHighlighting(SbBool onoff)
{
  this->locateHighlighting = onoff;
}

void
SoTransformerDragger::setColinearThreshold(int newval)
{
  SoTransformerDragger::colinearThreshold = newval;
}

int
SoTransformerDragger::getColinearThreshold(void)
{
  return SoTransformerDragger::colinearThreshold;
}

SbVec3f
SoTransformerDragger::getBoxPointInWorldSpace(const SbVec3f & pointonunitbox)
{
  SbMatrix mat, inv;
  this->getSurroundScaleMatrices(mat, inv);
  mat.multRight(this->getLocalToWorldMatrix());
  SbVec3f ret;
  mat.multVecMatrix(pointonunitbox, ret);
  return ret;
}

SbVec3f
SoTransformerDragger::getBoxDirInWorldSpace(const SbVec3f & dironunitbox)
{
  SbMatrix mat, inv;
  this->getSurroundScaleMatrices(mat, inv);
  mat.multRight(this->getLocalToWorldMatrix());
  SbVec3f ret;
  mat.multDirMatrix(dironunitbox, ret);
  return ret;
}

SbVec3f
SoTransformerDragger::getWorldPointInBoxSpace(const SbVec3f & pointinworldspace)
{
  SbMatrix mat, inv;
  this->getSurroundScaleMatrices(mat, inv);
  mat.multLeft(this->getWorldToLocalMatrix());
  SbVec3f ret;
  mat.multVecMatrix(pointinworldspace, ret);
  return ret;
}

SbVec2f
SoTransformerDragger::getWorldPointInPixelSpace(const SbVec3f & thepoint)
{
  SbVec3f screenpt;
  this->getViewVolume().projectToScreen(thepoint, screenpt);
  return SbVec2f(screenpt[0], screenpt[1]);
}

SbVec3f
SoTransformerDragger::getInteractiveCenterInBoxSpace(void)
{
  if (this->ctrlDown) return this->ctrlOffset;
  else return SbVec3f(0.0f, 0.0f, 0.0f);
}

void
SoTransformerDragger::startCB(void *, SoDragger * d)
{
  SoTransformerDragger *thisp = (SoTransformerDragger*)d;
  thisp->dragStart();
}

void
SoTransformerDragger::motionCB(void *, SoDragger * d)
{
  SoTransformerDragger *thisp = (SoTransformerDragger*)d;
  thisp->drag();
}

void
SoTransformerDragger::finishCB(void *, SoDragger * d)
{
  SoTransformerDragger *thisp = (SoTransformerDragger*)d;
  thisp->dragFinish();
}

void
SoTransformerDragger::metaKeyChangeCB(void *, SoDragger *d)
{
  SoTransformerDragger *thisp = (SoTransformerDragger*)d;
  if (!thisp->isActive.getValue()) return;

  const SoEvent *event = thisp->getEvent();
  if (thisp->shiftDown != event->wasShiftDown()) {
    thisp->drag();
  }
  if (thisp->ctrlDown != event->wasCtrlDown()) {
    thisp->drag();
  }
}

void
SoTransformerDragger::dragStart(void)
{
  int i;
  const SoPath *pickpath = this->getPickPath();
  const SoEvent *event = this->getEvent();

  SbBool found = FALSE;
  this->state = INACTIVE;

  SbVec3f startpt = this->getLocalStartingPoint();
  startpt = this->localToWorking(startpt);

  SbString str;
  if (!found) {
    for (i = 1; i <= 6; i++) {
      str.sprintf("translator%d", i);
      if (pickpath->findNode(this->getNodeFieldNode(str.getString())) >= 0) break;
    }
    if (i <= 6) {
      found = TRUE;
      this->state = (State)(int(RIT_TRANSLATE) + (i-1));
      this->whatkind = WHATKIND_TRANSLATE;
      this->whatnum = i;
      if (i <= 2) this->dimension = 1;
      else if (i <= 4) this->dimension = 0;
      else this->dimension = 2;
    }
  }

  if (!found) {
    for (i = 1; i <= 6; i++) {
      str.sprintf("rotator%d", i);
      if (pickpath->findNode(this->getNodeFieldNode(str.getString()))>= 0) break;
    }
    if (i <= 6) {
      found = TRUE;
      this->state = (State)(int(RIT_X_ROTATE) + (i-1));
      this->whatkind = WHATKIND_ROTATE;
      this->whatnum = i;
      if (i <= 2) this->dimension = 1;
      else if (i <= 4) this->dimension = 0;
      else this->dimension = 2;
    }
  }
  if (!found) {
    for (i = 1; i <= 8; i++) {
      str.sprintf("scale%d", i);
      if (pickpath->findNode(this->getNodeFieldNode(str.getString()))>= 0) break;
    }
    if (i <= 8) {
      found = TRUE;
      this->state = (State) (int(PX_PY_PZ_3D_SCALE) + (i-1));
      this->whatkind = WHATKIND_SCALE;
      this->whatnum = i;
    }
  }
  assert(found);

  this->ctrlDown = event->wasCtrlDown();
  this->shiftDown = event->wasShiftDown();
  this->ctrlOffset = this->calcCtrlOffset(startpt);

  switch(this->whatkind) {
  case WHATKIND_TRANSLATE:
    {
      SbVec3f n(0.0f, 0.0f, 0.0f);
      n[this->dimension] = 1.0f;
      this->planeProj->setPlane(SbPlane(n, startpt));
      this->lineProj->setLine(SbLine(startpt, startpt + n));
      this->constraintState = CONSTRAINT_OFF;
      if (this->shiftDown) {
        this->getWorkingToWorldMatrix().multVecMatrix(startpt, this->worldRestartPt);
        this->constraintState = CONSTRAINT_WAIT;
      }
      SbLine myline(SbVec3f(0.0f, 0.0f, 0.0f), n);
      SoTranslation *t = SO_GET_ANY_PART(this, "axisFeedbackLocation", SoTranslation);
      t->translation = myline.getClosestPoint(startpt);

      this->setAllPartSwitches(SO_SWITCH_NONE, SO_SWITCH_NONE, SO_SWITCH_NONE);
      SbString str;
      str.sprintf("translator%dSwitch", this->whatnum);
      this->setSwitchValue(str.getString(), 1);
      this->setSwitchValue("translateBoxFeedbackSwitch", 0);
      this->setDynamicTranslatorSwitches(event);
    }
    break;
  case WHATKIND_SCALE:
    {
      SoTranslation *t = SO_GET_ANY_PART(this, "axisFeedbackLocation", SoTranslation);
      t->translation = startpt;
      this->lineProj->setLine(SbLine(SbVec3f(0.0f, 0.0f, 0.0f), startpt));
      this->constraintState = CONSTRAINT_OFF;
      if (this->shiftDown) {
        this->getWorkingToWorldMatrix().multVecMatrix(startpt, this->worldRestartPt);
        this->constraintState = CONSTRAINT_WAIT;
      }

      SbString str;
      str.sprintf("scale%dSwitch", this->whatnum);
      this->setAllPartSwitches(0, SO_SWITCH_NONE, SO_SWITCH_NONE);
      this->setSwitchValue(str.getString(), 1);
      this->setDynamicScaleSwitches(event);
    }
    break;
  case WHATKIND_ROTATE:
    {
      SoTranslation *t = SO_GET_ANY_PART(this, "axisFeedbackLocation", SoTranslation);
      t->translation = startpt;
      this->sphereProj->setSphere(SbSphere(SbVec3f(0.0f, 0.0f, 0.0f), startpt.length()));
      this->sphereProj->setViewVolume(this->getViewVolume());
      this->sphereProj->setWorkingSpace(this->getWorkingToWorldMatrix());

      SbVec3f projpt = this->sphereProj->project(this->getNormalizedLocaterPosition());
      this->getWorkingToWorldMatrix().multVecMatrix(projpt, this->prevWorldHitPt);
      this->prevMotionMatrix = this->getMotionMatrix();

      this->constraintState = CONSTRAINT_OFF;
      if (!this->shiftDown) {
        this->constraintState = CONSTRAINT_WAIT;
        // this plane is only used to find constraint direction
        this->planeProj->setPlane(SbPlane(startpt, startpt));
      }
      SoAntiSquish *squish = SO_GET_ANY_PART(this, "circleFeedbackAntiSquish", SoAntiSquish);
      SoAntiSquish::Sizing sizing;
      switch (this->dimension) {
      case 0: sizing = SoAntiSquish::X; break;
      case 1: sizing = SoAntiSquish::Y; break;
      case 2: sizing = SoAntiSquish::Z; break;
      default: assert(FALSE); sizing = SoAntiSquish::Z; // Dummy assignment to avoid compiler warning.
      }
      squish->sizing = sizing;
      squish->recalc();
      this->setAllPartSwitches(SO_SWITCH_NONE, 0, SO_SWITCH_NONE);
      this->setDynamicRotatorSwitches(event);
    }
    break;
  default:
    assert(0 && "unknown whatkind");
    break;
  }
}

void
SoTransformerDragger::drag(void)
{
  switch(this->whatkind) {
  case WHATKIND_SCALE:
    this->dragScale();
    break;
  case WHATKIND_TRANSLATE:
    this->dragTranslate();
    break;
  case WHATKIND_ROTATE:
    this->dragRotate();
    break;
  default:
    assert(0 && "illegal whatkind");
    break;
  }
}

void
SoTransformerDragger::dragTranslate()
{
  SbVec3f startpt = this->getLocalStartingPoint();
  startpt = this->localToWorking(startpt);

  this->planeProj->setViewVolume(this->getViewVolume());
  this->planeProj->setWorkingSpace(this->getWorkingToWorldMatrix());
  SbVec3f projpt = this->planeProj->project(this->getNormalizedLocaterPosition());

  const SoEvent *event = this->getEvent();
  if (event->wasShiftDown() && this->constraintState == CONSTRAINT_OFF) {
    this->constraintState = CONSTRAINT_WAIT;
    this->setStartLocaterPosition(event->getPosition());
    this->getWorkingToWorldMatrix().multVecMatrix(projpt, this->worldRestartPt);
  }
  else if (!event->wasShiftDown() && this->constraintState != CONSTRAINT_OFF) {
    this->constraintState = CONSTRAINT_OFF;
  }

  if (this->setDynamicTranslatorSwitches(event)) {
    this->saveStartParameters();
    SbVec3f n(0.0f, 0.0f, 0.0f);
    n[this->dimension] = 1.0f;
    this->lineProj->setLine(SbLine(projpt, projpt+n));
    SbVec3f worldpt;
    this->getWorkingToWorldMatrix().multVecMatrix(projpt, worldpt);
    this->setStartingPoint(worldpt);
    startpt = projpt;
  }

  SbVec3f motion;
  if (this->ctrlDown) {
    this->lineProj->setViewVolume(this->getViewVolume());
    this->lineProj->setWorkingSpace(this->getWorkingToWorldMatrix());
    projpt = this->lineProj->project(this->getNormalizedLocaterPosition());
    motion = projpt - startpt;
  }
  else {
    SbVec3f localrestartpt;
    if (this->constraintState != CONSTRAINT_OFF) {
      this->getWorldToLocalMatrix().multVecMatrix(this->worldRestartPt,
                                                  localrestartpt);
      motion = localrestartpt - startpt;
    }
    else motion = projpt - startpt;
    switch(this->constraintState) {
    case CONSTRAINT_OFF:
      break;
    case CONSTRAINT_WAIT:
      if (this->isAdequateConstraintMotion()) {
        SbVec3f newmotion = projpt - localrestartpt;
        int biggest = 0;
        double bigval = fabs(newmotion[0]);
        if (fabs(newmotion[1]) > bigval) {
          biggest = 1;
          bigval = fabs(newmotion[1]);
        }
        if (fabs(newmotion[2]) > bigval) {
          biggest = 2;
        }
        motion[biggest] += newmotion[biggest];
        this->constraintState = CONSTRAINT_X + biggest;
        this->setDynamicTranslatorSwitches(event);
      }
      else {
        return;
      }
      break;
    case CONSTRAINT_X:
      motion[0] += projpt[0] - localrestartpt[0];
      break;
    case CONSTRAINT_Y:
      motion[1] += projpt[1] - localrestartpt[1];
      break;
    case CONSTRAINT_Z:
      motion[2] += projpt[2] - localrestartpt[2];
    }
  }
  SbMatrix mat, inv;
  this->getSurroundScaleMatrices(mat, inv);
  this->setMotionMatrix(this->appendTranslation(this->getStartMotionMatrix(), motion, &mat));
  this->unsquishKnobs();
}

void
SoTransformerDragger::dragScale()
{
  SbVec3f startpt = this->getLocalStartingPoint();
  startpt = this->localToWorking(startpt);

  this->lineProj->setViewVolume(this->getViewVolume());
  this->lineProj->setWorkingSpace(this->getWorkingToWorldMatrix());
  SbVec3f projpt = this->lineProj->project(this->getNormalizedLocaterPosition());
  const SoEvent *event = this->getEvent();

  if (event->wasShiftDown() && this->constraintState == CONSTRAINT_OFF) {
    this->constraintState = CONSTRAINT_WAIT;
    this->setStartLocaterPosition(event->getPosition());
    this->getWorkingToWorldMatrix().multVecMatrix(projpt, this->worldRestartPt);
  }
  else if (!event->wasShiftDown() && this->constraintState != CONSTRAINT_OFF) {
    this->saveStartParameters();
    this->constraintState = CONSTRAINT_OFF;
    this->lineProj->setLine(SbLine(SbVec3f(0.0f, 0.0f, 0.0f), projpt));
    this->ctrlOffset = this->calcCtrlOffset(projpt);
    startpt = projpt;
    SbVec3f worldpt;
    this->getWorkingToWorldMatrix().multVecMatrix(projpt, worldpt);
    this->setStartingPoint(worldpt);
  }

  if (this->constraintState == CONSTRAINT_WAIT && this->isAdequateConstraintMotion()) {
    // detect which dimension user has moved mouse the most. Done by projecting
    // mouse positions onto the near plane, finding that world vector, and
    // transforming that world vector into working space.
    const SbViewVolume &vv = this->getViewVolume();
    const SbViewportRegion &vp = this->getViewportRegion();
    SbVec2s move = this->getLocaterPosition() - this->getStartLocaterPosition();
    SbVec2f normmove((float)move[0]/(float)vp.getViewportSizePixels()[0],
                     (float)move[1]/(float)vp.getViewportSizePixels()[1]);
    SbVec3f tmp = vv.getPlanePoint(vv.getNearDist(), SbVec2f(0.5f, 0.5f));
    SbVec3f dir = vv.getPlanePoint(vv.getNearDist(), SbVec2f(0.5f, 0.5f) + normmove);
    dir -= tmp;
    dir.normalize();
    this->getWorldToWorkingMatrix().multDirMatrix(dir, dir);
    int biggest = 0;
    double bigval = fabs(dir[0]);
    if (fabs(dir[1]) > bigval) {
      biggest = 1;
      bigval = fabs(dir[1]);
    }
    if (fabs(dir[2]) > bigval) {
      biggest = 2;
    }
    SbVec3f n(0.0f, 0.0f, 0.0f);
    n[biggest] = 1.0f;

    this->constraintState = CONSTRAINT_X + biggest;

    this->saveStartParameters();
    this->lineProj->setLine(SbLine(projpt, projpt+n));
    startpt = projpt;
    projpt[(biggest+1)%3] = 0.0f;
    projpt[(biggest+2)%3] = 0.0f;
    this->ctrlOffset = this->calcCtrlOffset(projpt);
    projpt = startpt;
    SbVec3f worldpt;
    this->getWorkingToWorldMatrix().multVecMatrix(projpt, worldpt);
    this->setStartingPoint(worldpt);
  }

  this->setDynamicScaleSwitches(event);

  if (this->constraintState == CONSTRAINT_WAIT) return;

  if (this->constraintState >= CONSTRAINT_X) {
    int num = this->constraintState - CONSTRAINT_X;
    projpt[(num+1)%3] = 0.0f;
    projpt[(num+2)%3] = 0.0f;
    startpt[(num+1)%3] = 0.0f;
    startpt[(num+2)%3] = 0.0f;
  }

  SbVec3f center(0.0f, 0.0f, 0.0f);
  if (this->ctrlDown) {
    center -= this->ctrlOffset;
  }

  float orglen = (startpt-center).length();
  float currlen = (projpt-center).length();
  float scale = 0.0f;

  if (orglen > 0.0f) scale = currlen / orglen;
  if (scale > 0.0f && (startpt-center).dot(projpt-center) <= 0.0f) scale = 0.0f;

  SbVec3f scalevec(scale, scale, scale);
  if (this->constraintState >= CONSTRAINT_X) {
    int num = this->constraintState - CONSTRAINT_X;
    scalevec[(num+1)%3] = 1.0f;
    scalevec[(num+2)%3] = 1.0f;
  }

  SbMatrix mat, inv;
  this->getSurroundScaleMatrices(mat, inv);
  this->setMotionMatrix(this->appendScale(this->getStartMotionMatrix(),
                                          scalevec,
                                          center, &mat));
  this->unsquishKnobs();
}

void
SoTransformerDragger::dragRotate(void)
{
  this->sphereProj->setViewVolume(this->getViewVolume());
  this->sphereProj->setWorkingSpace(this->getWorkingToWorldMatrix());

  const SoEvent *event = this->getEvent();

  SbVec3f startpt, projpt;
  startpt = this->getLocalStartingPoint();
  startpt = this->localToWorking(startpt);

  if (event->wasShiftDown() && this->constraintState != CONSTRAINT_OFF) {
    this->constraintState = CONSTRAINT_OFF;
    projpt = this->sphereProj->project(this->getNormalizedLocaterPosition());
    this->getWorkingToWorldMatrix().multVecMatrix(projpt, this->prevWorldHitPt);
    this->prevMotionMatrix = this->getMotionMatrix();
    this->saveStartParameters();
    this->setStartingPoint(this->prevWorldHitPt);
  }
  else if (!event->wasShiftDown() && this->constraintState == CONSTRAINT_OFF) {
    COIN_STUB();
  }

  SbVec3f center(0.0f, 0.0f, 0.0f);
  if (this->ctrlDown) {
    center -= this->ctrlOffset * KNOB_DISTANCE;
  }

  this->setDynamicRotatorSwitches(event);

  if (this->constraintState == CONSTRAINT_OFF) {
    this->getWorldToWorkingMatrix().multVecMatrix(this->prevWorldHitPt, startpt);
    projpt = this->sphereProj->project(this->getNormalizedLocaterPosition());
    this->getWorkingToWorldMatrix().multVecMatrix(projpt, this->prevWorldHitPt);
    SbRotation rot = this->sphereProj->getRotation(startpt, projpt);
    SbMatrix mat, inv;
    this->getSurroundScaleMatrices(mat, inv);
    this->prevMotionMatrix = this->appendRotation(this->prevMotionMatrix, rot,
                                                  center, &mat);
    this->setMotionMatrix(this->prevMotionMatrix);
  }
  else if (this->constraintState == CONSTRAINT_WAIT && this->isAdequateConstraintMotion()) {
    this->planeProj->setViewVolume(this->getViewVolume());
    this->planeProj->setWorkingSpace(this->getWorkingToWorldMatrix());
    projpt = this->planeProj->project(this->getNormalizedLocaterPosition());

    SbVec3f diff = projpt - startpt;
    int biggest = 0;
    double bigval = fabs(diff[0]);
    if (fabs(diff[1]) > bigval) {
      biggest = 1;
      bigval = fabs(diff[1]);
    }
    if (fabs(diff[2]) > bigval) {
      biggest = 2;
    }
    this->constraintState = CONSTRAINT_X + biggest;
    SbVec3f n(0.0f, 0.0f, 0.0f);
    n[biggest] = 1.0f;
    SbVec3f dim(0.0f, 0.0f, 0.0f);
    dim[this->dimension] = 1.0f;
    // set plane to do disc-rotate in
    this->planeProj->setPlane(SbPlane(SbVec3f(0.0f, 0.0f, 0.0f), dim, dim+n));
    this->setDynamicRotatorSwitches(event);
  }
  if (this->constraintState >= CONSTRAINT_X) {
    // project startpt into plane to get nicer disc-rotation
    // SbPlane really should have had a getClosestPoint() method.
    const SbPlane &plane = this->planeProj->getPlane();
    startpt -= plane.getNormal() * plane.getDistance(startpt);

    this->planeProj->setViewVolume(this->getViewVolume());
    this->planeProj->setWorkingSpace(this->getWorkingToWorldMatrix());

    projpt = this->planeProj->project(this->getNormalizedLocaterPosition());
    startpt -= center;
    projpt -= center;
    SbRotation rot(startpt, projpt);
    SbMatrix mat, inv;
    this->getSurroundScaleMatrices(mat, inv);
    this->setMotionMatrix(this->appendRotation(this->getStartMotionMatrix(),
                                               rot,
                                               center, &mat));
  }
  this->unsquishKnobs();
}


void
SoTransformerDragger::dragFinish(void)
{
  switch (this->whatkind) {
  case WHATKIND_TRANSLATE:
    this->setSwitchValue("translateBoxFeedbackSwitch", SO_SWITCH_NONE);
    break;
  case WHATKIND_ROTATE:
  this->setSwitchValue("xCircleFeedbackSwitch", SO_SWITCH_NONE);
  this->setSwitchValue("yCircleFeedbackSwitch", SO_SWITCH_NONE);
  this->setSwitchValue("zCircleFeedbackSwitch", SO_SWITCH_NONE);
    break;
  case WHATKIND_SCALE:
    this->setSwitchValue("radialFeedbackSwitch", SO_SWITCH_NONE);
    this->setSwitchValue("posXWallFeedbackSwitch", SO_SWITCH_NONE);
    this->setSwitchValue("negXWallFeedbackSwitch", SO_SWITCH_NONE);
    this->setSwitchValue("posYWallFeedbackSwitch", SO_SWITCH_NONE);
    this->setSwitchValue("negYWallFeedbackSwitch", SO_SWITCH_NONE);
    this->setSwitchValue("posZWallFeedbackSwitch", SO_SWITCH_NONE);
    this->setSwitchValue("negZWallFeedbackSwitch", SO_SWITCH_NONE);
    this->setSwitchValue("scaleBoxFeedbackSwitch", SO_SWITCH_NONE);
    break;
  default:
    assert(0 && "unknown whatkind");
    break;
  }

  this->whatkind = WHATKIND_NONE;
  this->state = INACTIVE;
  this->constraintState = CONSTRAINT_OFF;
  this->setAllPartSwitches(0,0,0);
  this->setSwitchValue("xAxisFeedbackSwitch", SO_SWITCH_NONE);
  this->setSwitchValue("yAxisFeedbackSwitch", SO_SWITCH_NONE);
  this->setSwitchValue("zAxisFeedbackSwitch", SO_SWITCH_NONE);

#if COIN_DEBUG && 0 // used to debug motion matrix (pederb, 20000225)
  SbRotation r,so;
  SbVec3f t,s;

  fprintf(stderr,"motion matrix:\n");
  m.print(stderr);
  m.getTransform(t, r, s, so);
  SbVec3f rx, sox;
  float ra, soa;
  r.getValue(rx, ra);
  so.getValue(sox, soa);
  fprintf(stderr,
          "\nt: %g %g %g\n"
          "r: %g %g %g, %g\n"
          "s: %g %g %g\n"
          "so: %g %g %g, %g\n\n",
          t[0], t[1], t[2],
          rx[0], rx[1], rx[2], ra,
          s[0], s[1], s[2],
          sox[0], sox[1], sox[2], soa);
#endif // debug code
}

void
SoTransformerDragger::updateAntiSquishList(void)
{
  if (this->antiSquishList.getLength() == 0) {
    SoSeparator *top = SO_GET_ANY_PART(this,"topSeparator", SoSeparator);
    assert(top);

    SoSearchAction sa;
    sa.setInterest(SoSearchAction::ALL);
    sa.setType(SoAntiSquish::getClassTypeId());
    sa.setSearchingAll(TRUE);
    sa.apply(top);

    SoPathList &pl = sa.getPaths();
    for (int i = 0; i < pl.getLength(); i++) {
      SoFullPath *path = (SoFullPath*)pl[i];
      SoNode *tail = path->getTail();
      int j, n = this->antiSquishList.getLength();
      for (j = 0; j < n; j++) {
        if (this->antiSquishList[j] == tail) break;
      }
      if (j == n)
        this->antiSquishList.append(path->getTail());
    }
  }
  int n = this->antiSquishList.getLength();
  for (int i = 0; i < n; i++) {
    SoAntiSquish *squishy = (SoAntiSquish*) this->antiSquishList[i];
    squishy->recalc();
  }
}

void
SoTransformerDragger::setAllPartSwitches(int scalewhich, int rotatewhich, int translatewhich)
{
  int i;
  SoSwitch *sw;
  SbString str;

  for (i = 1; i <= 6; i++) {
    str.sprintf("translator%dSwitch", i);
    sw = SO_GET_ANY_PART(this, str.getString(), SoSwitch);
    SoInteractionKit::setSwitchValue(sw, translatewhich);
  }
  for (i = 1; i <= 6; i++) {
    str.sprintf("rotator%dSwitch", i);
    sw = SO_GET_ANY_PART(this, str.getString(), SoSwitch);
    SoInteractionKit::setSwitchValue(sw, rotatewhich);
  }
  for (i = 1; i <= 8; i++) {
    str.sprintf("scale%dSwitch", i);
    sw = SO_GET_ANY_PART(this, str.getString(), SoSwitch);
    SoInteractionKit::setSwitchValue(sw, scalewhich);
  }
}

/*!
  STUB. Should probably have been private in OIV.
*/
int
SoTransformerDragger::getMouseGestureDirection(SbBool x_ok, SbBool y_ok, SbBool z_ok)
{
  COIN_STUB();
  return -1;
}

/*!
  STUB. Should probably have been private in OIV.
*/
int
SoTransformerDragger::getIgnoreAxis(SbVec2f axis[3][2], SbBool x_ok, SbBool y_ok, SbBool z_ok)
{
  COIN_STUB();
  return -1;
}

/*!
  STUB. Should probably have been private in OIV.
*/
void
SoTransformerDragger::makeMinorAxisPerpendicularIfColinear(SbVec2f origin, SbVec2f axisends[3][2], int index_a, int index_b)
{
  COIN_STUB();
}

/*!
  STUB. Should probably have been private in OIV.
*/
SbBool
SoTransformerDragger::isColinear(SbVec2f a1[2], SbVec2f a2[2], int pixels)
{
  COIN_STUB();
  return FALSE;
}

void
SoTransformerDragger::getSurroundScaleMatrices(SbMatrix &mat, SbMatrix &inv)
{
  //
  // FIXME: implement a getMatrices() method in SoSurroundScale to avoid
  // creating an SoGetMatrixAction here. pederb, 20000224
  //
  SoSurroundScale *ss = SO_CHECK_ANY_PART(this, "surroundScale", SoSurroundScale);
  if (ss) {
    SoGetMatrixAction ma(this->getViewportRegion());
    ma.apply(ss);
    mat = ma.getMatrix();
    inv = ma.getInverse();
  }
  else {
    mat = SbMatrix::identity();
    inv = SbMatrix::identity();
  }
}

SoNode *
SoTransformerDragger::getNodeFieldNode(const char *fieldname)
{
  SoField *field = this->getField(fieldname);
  assert(field != NULL);
  assert(field->isOfType(SoSFNode::getClassTypeId()));
  assert(((SoSFNode*)field)->getValue() != NULL);
  return ((SoSFNode*)field)->getValue();
}

SbMatrix
SoTransformerDragger::getWorkingToWorldMatrix()
{
  SbMatrix mat, inv;
  this->getSurroundScaleMatrices(mat, inv);
  mat.multRight(this->getLocalToWorldMatrix());
  return mat;
}

SbMatrix
SoTransformerDragger::getWorldToWorkingMatrix(void)
{
  SbMatrix mat, inv;
  this->getSurroundScaleMatrices(mat, inv);
  mat.multLeft(this->getWorldToLocalMatrix());
  return mat;
}

SbVec3f
SoTransformerDragger::localToWorking(const SbVec3f &v)
{
  SbMatrix mat, inv;
  this->getSurroundScaleMatrices(mat, inv);
  SbVec3f ret;
  inv.multVecMatrix(v, ret);
  return ret;
}

SbVec3f
SoTransformerDragger::workingToLocal(const SbVec3f &v)
{
  SbMatrix mat, inv;
  this->getSurroundScaleMatrices(mat, inv);
  SbVec3f ret;
  mat.multVecMatrix(v, ret);
  return ret;
}

SbVec3f
SoTransformerDragger::calcCtrlOffset(const SbVec3f &startpt)
{
  SbVec3f v = startpt;
  for (int i = 0; i < 3; i++) {
    if (v[i] < -0.8) v[i] = -1.0f;
    else if (v[i] > 0.8) v[i] = 1.0f;
    else v[i] = 0.0f;
  }
  return v;
}

void
SoTransformerDragger::setSwitchValue(const char *str, const int which)
{
  SoSwitch *sw = SO_GET_ANY_PART(this, str, SoSwitch);
  SoInteractionKit::setSwitchValue(sw, which);
}

SbBool
SoTransformerDragger::setDynamicTranslatorSwitches(const SoEvent *event)
{
  SbBool changed = FALSE;
  if (this->ctrlDown != event->wasCtrlDown()) {
    changed = TRUE;
    this->ctrlDown = !this->ctrlDown;
  }
  if (this->shiftDown != event->wasShiftDown()) {
    changed = TRUE;
    this->shiftDown = !this->shiftDown;
  }

  SbString str;

  if (this->constraintState >= CONSTRAINT_X) {
    int which = this->constraintState - CONSTRAINT_X;
    str.sprintf("%cAxisFeedbackSwitch", 'x' + which);
    this->setSwitchValue(str.getString(), 0);
    str.sprintf("%cAxisFeedbackSwitch", 'x' + (which+1)%3);
    this->setSwitchValue(str.getString(), SO_SWITCH_NONE);
    str.sprintf("%cAxisFeedbackSwitch", 'x' + (which+2)%3);
    this->setSwitchValue(str.getString(), SO_SWITCH_NONE);
  }
  else {
    str.sprintf("%cAxisFeedbackSwitch", 'x' + this->dimension);
    this->setSwitchValue(str.getString(), this->ctrlDown ? 0 : SO_SWITCH_NONE);
    int val = this->shiftDown ? 1 : 0;
    if (this->ctrlDown) val = SO_SWITCH_NONE;
    str.sprintf("%cAxisFeedbackSwitch", 'x' + (this->dimension+1)%3);
    this->setSwitchValue(str.getString(), val);
    str.sprintf("%cAxisFeedbackSwitch", 'x' + (this->dimension+2)%3);
    this->setSwitchValue(str.getString(), val);
  }
  return changed;
}

SbBool
SoTransformerDragger::setDynamicScaleSwitches(const SoEvent *event)
{
  SbBool changed = FALSE;
  if (this->ctrlDown != event->wasCtrlDown()) {
    changed = TRUE;
    this->ctrlDown = !this->ctrlDown;
  }
  if (this->shiftDown != event->wasShiftDown()) {
    changed = TRUE;
    this->shiftDown = !this->shiftDown;
  }
  SbString str;
  if (this->constraintState == CONSTRAINT_WAIT) {
    this->setSwitchValue("xAxisFeedbackSwitch", 1);
    this->setSwitchValue("yAxisFeedbackSwitch", 1);
    this->setSwitchValue("zAxisFeedbackSwitch", 1);
    this->setSwitchValue("radialFeedbackSwitch", SO_SWITCH_NONE);
  }
  else if (this->constraintState >= CONSTRAINT_X) {
    int which = this->constraintState - CONSTRAINT_X;
    str.sprintf("%cAxisFeedbackSwitch", 'x' + which);
    this->setSwitchValue(str.getString(), 0);
    str.sprintf("%cAxisFeedbackSwitch", 'x' + (which+1)%3);
    this->setSwitchValue(str.getString(), SO_SWITCH_NONE);
    str.sprintf("%cAxisFeedbackSwitch", 'x' + (which+2)%3);
    this->setSwitchValue(str.getString(), SO_SWITCH_NONE);
    this->setSwitchValue("radialFeedbackSwitch", SO_SWITCH_NONE);
  }
  else {
    this->setSwitchValue("xAxisFeedbackSwitch", SO_SWITCH_NONE);
    this->setSwitchValue("yAxisFeedbackSwitch", SO_SWITCH_NONE);
    this->setSwitchValue("zAxisFeedbackSwitch", SO_SWITCH_NONE);
    this->setSwitchValue("radialFeedbackSwitch", 0);
  }

  this->setSwitchValue("scaleBoxFeedbackSwitch", this->shiftDown ? 0 : SO_SWITCH_NONE);

  if (this->ctrlDown) {
    SbVec3f pt = this->getLocalStartingPoint();
    if (this->constraintState >= CONSTRAINT_X) {
      int num = this->constraintState - CONSTRAINT_X;
      pt[(num+1)%3] = 0.0f;
      pt[(num+2)%3] = 0.0f;
    }
    SbString str;
    for (int i = 0; i < 3; i++) {
      str.sprintf("pos%cWallFeedbackSwitch", 'X' + i);
      this->setSwitchValue(str.getString(), pt[i] < 0.0f ? 0 : SO_SWITCH_NONE);
      str.sprintf("neg%cWallFeedbackSwitch", 'X' + i);
      this->setSwitchValue(str.getString(), pt[i] > 0.0f ? 0 : SO_SWITCH_NONE);
    }
  }
  else {
    this->setSwitchValue("posXWallFeedbackSwitch", SO_SWITCH_NONE);
    this->setSwitchValue("negXWallFeedbackSwitch", SO_SWITCH_NONE);
    this->setSwitchValue("posYWallFeedbackSwitch", SO_SWITCH_NONE);
    this->setSwitchValue("negYWallFeedbackSwitch", SO_SWITCH_NONE);
    this->setSwitchValue("posZWallFeedbackSwitch", SO_SWITCH_NONE);
    this->setSwitchValue("negZWallFeedbackSwitch", SO_SWITCH_NONE);
  }
  return changed;
}


SbBool
SoTransformerDragger::setDynamicRotatorSwitches(const SoEvent *event)
{
  SbBool changed = FALSE;
  if (this->ctrlDown != event->wasCtrlDown()) {
    changed = TRUE;
    this->ctrlDown = !this->ctrlDown;
  }
  if (this->shiftDown != event->wasShiftDown()) {
    changed = TRUE;
    this->shiftDown = !this->shiftDown;
  }

  SbString str;
  {
    int axis0 = this->whatnum-1;
    int axis1 = (axis0 & 1) ? axis0 - 1 : axis0 + 1;

    str.sprintf("rotator%dSwitch", axis0 + 1);
    this->setSwitchValue(str.getString(), 1);
    str.sprintf("rotator%dSwitch", axis1 + 1);
    this->setSwitchValue(str.getString(), this->ctrlDown ? 0 : 1);
  }

  int axisval[3];
  int circleval[3];
  int dim = this->dimension;

  if (this->constraintState == CONSTRAINT_WAIT) {
    axisval[dim] = SO_SWITCH_NONE;
    axisval[(dim+1)%3] = 1;
    axisval[(dim+2)%3] = 1;
    circleval[dim] = SO_SWITCH_NONE;
    circleval[(dim+1)%3] = 0;
    circleval[(dim+2)%3] = 0;
  }
  else if (this->constraintState >= CONSTRAINT_X) {
    dim = this->constraintState - CONSTRAINT_X;
    axisval[dim] = 0;
    axisval[(dim+1)%3] = SO_SWITCH_NONE;
    axisval[(dim+2)%3] = SO_SWITCH_NONE;

    const SbVec3f &n = this->planeProj->getPlane().getNormal();
    circleval[0] = n[0] != 0.0f ? 0 : SO_SWITCH_NONE;
    circleval[1] = n[1] != 0.0f ? 0 : SO_SWITCH_NONE;
    circleval[2] = n[2] != 0.0f ? 0 : SO_SWITCH_NONE;
  }
  else {
    circleval[0] = 0;
    circleval[1] = 0;
    circleval[2] = 0;
    axisval[0] = SO_SWITCH_NONE;
    axisval[1] = SO_SWITCH_NONE;
    axisval[2] = SO_SWITCH_NONE;
  }
  if (this->ctrlDown) {
    SoTransform *transform = SO_GET_ANY_PART(this, "circleFeedbackTransform", SoTransform);
    SbVec3f offset = -this->ctrlOffset * KNOB_DISTANCE;
    if (transform->translation.getValue() != offset)
      transform->translation = offset;
    if (transform->scaleFactor.getValue() != SbVec3f(2.0f, 2.0f, 2.0f))
      transform->scaleFactor = SbVec3f(2.0f, 2.0f, 2.0f);
    this->setSwitchValue("circleFeedbackTransformSwitch", SO_SWITCH_ALL);
  }
  else {
    this->setSwitchValue("circleFeedbackTransformSwitch", 0);
  }

  this->setSwitchValue("xAxisFeedbackSwitch", axisval[0]);
  this->setSwitchValue("yAxisFeedbackSwitch", axisval[1]);
  this->setSwitchValue("zAxisFeedbackSwitch", axisval[2]);
  this->setSwitchValue("xCircleFeedbackSwitch", circleval[0]);
  this->setSwitchValue("yCircleFeedbackSwitch", circleval[1]);
  this->setSwitchValue("zCircleFeedbackSwitch", circleval[2]);

  return changed;
}


// Undefine these again, as some of them are also used in other
// dragger sourcecode files (which causes trouble when using the
// compact build hack where all .cpp files are included into all.cpp).

#undef WHATKIND_NONE
#undef WHATKIND_SCALE
#undef WHATKIND_TRANSLATE
#undef WHATKIND_ROTATE

#undef CONSTRAINT_OFF
#undef CONSTRAINT_WAIT
#undef CONSTRAINT_X
#undef CONSTRAINT_Y
#undef CONSTRAINT_Z

#undef KNOB_DISTANCE
