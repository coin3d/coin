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

#include <Inventor/draggers/SoTabPlaneDragger.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoSwitch.h>


SO_KIT_SOURCE(SoTabPlaneDragger);


void
SoTabPlaneDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoTabPlaneDragger);
}

SoTabPlaneDragger::SoTabPlaneDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoTabPlaneDragger);

  SO_KIT_ADD_CATALOG_ENTRY(planeSwitch, SoSwitch, TRUE, geomSeparator, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator, SoSeparator, TRUE, planeSwitch, scaleTabs, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scaleTabs, SoSeparator, TRUE, planeSwitch, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scaleTabMaterial, SoMaterial, TRUE, scaleTabs, scaleTabHints, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scaleTabHints, SoShapeHints, TRUE, scaleTabs, scaleTabMaterialBinding, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scaleTabMaterialBinding, SoMaterialBinding, TRUE, scaleTabs, scaleTabNormalBinding, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scaleTabNormalBinding, SoNormalBinding, TRUE, scaleTabs, scaleTabNormal, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scaleTabNormal, SoNormal, TRUE, scaleTabs, edgeScaleCoords, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(edgeScaleCoords, SoCoordinate3, TRUE, scaleTabs, edgeScaleTab0, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(edgeScaleTab0, SoIndexedFaceSet, TRUE, scaleTabs, edgeScaleTab1, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(edgeScaleTab1, SoIndexedFaceSet, TRUE, scaleTabs, edgeScaleTab2, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(edgeScaleTab2, SoIndexedFaceSet, TRUE, scaleTabs, edgeScaleTab3, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(edgeScaleTab3, SoIndexedFaceSet, TRUE, scaleTabs, cornerScaleCoords, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(cornerScaleCoords, SoCoordinate3, TRUE, scaleTabs, cornerScaleTab0, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(cornerScaleTab0, SoIndexedFaceSet, TRUE, scaleTabs, cornerScaleTab1, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(cornerScaleTab1, SoIndexedFaceSet, TRUE, scaleTabs, cornerScaleTab2, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(cornerScaleTab2, SoIndexedFaceSet, TRUE, scaleTabs, cornerScaleTab3, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(cornerScaleTab3, SoIndexedFaceSet, TRUE, scaleTabs, "", FALSE);

  SO_NODE_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));

  SO_KIT_INIT_INSTANCE();
}


SoTabPlaneDragger::~SoTabPlaneDragger()
{
  COIN_STUB();
}

SbBool
SoTabPlaneDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  COIN_STUB();
  return FALSE;
}

void
SoTabPlaneDragger::setDefaultOnNonWritingFields(void)
{
  COIN_STUB();
}

void
SoTabPlaneDragger::fieldSensorCB(void * f, SoSensor * s)
{
  COIN_STUB();
}

void
SoTabPlaneDragger::valueChangedCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoTabPlaneDragger::GLRender(SoGLRenderAction * action)
{
  COIN_STUB();
}

void
SoTabPlaneDragger::adjustScaleTabSize(void)
{
  COIN_STUB();
}

void
SoTabPlaneDragger::reallyAdjustScaleTabSize(SoGLRenderAction * action)
{
  COIN_STUB();
}

void
SoTabPlaneDragger::getXYScreenLengths(SbVec2f & lengths, const SbMatrix & localtoscreen, const SbVec2s & winsize)
{
  COIN_STUB();
}

void
SoTabPlaneDragger::dragStart(void)
{
  COIN_STUB();
}

void
SoTabPlaneDragger::drag(void)
{
  COIN_STUB();
}

void
SoTabPlaneDragger::dragFinish(void)
{
  COIN_STUB();
}

void
SoTabPlaneDragger::translateStart(void)
{
  COIN_STUB();
}

void
SoTabPlaneDragger::translateDrag(void)
{
  COIN_STUB();
}

void
SoTabPlaneDragger::edgeScaleStart(void)
{
  COIN_STUB();
}

void
SoTabPlaneDragger::edgeScaleDrag(void)
{
  COIN_STUB();
}

void
SoTabPlaneDragger::cornerScaleStart(void)
{
  COIN_STUB();
}

void
SoTabPlaneDragger::cornerScaleDrag(void)
{
  COIN_STUB();
}

void
SoTabPlaneDragger::scaleUniformStart(void)
{
  COIN_STUB();
}

void
SoTabPlaneDragger::scaleUniformDrag(void)
{
  COIN_STUB();
}

void
SoTabPlaneDragger::startCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoTabPlaneDragger::motionCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoTabPlaneDragger::finishCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoTabPlaneDragger::metaKeyChangeCB(void * f, SoDragger * d)
{
  COIN_STUB();
}
