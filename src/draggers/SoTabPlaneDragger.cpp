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

#include <Inventor/draggers/SoTabPlaneDragger.h>


SO_KIT_SOURCE(SoTabPlaneDragger);


void
SoTabPlaneDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoTabPlaneDragger);
}

SoTabPlaneDragger::SoTabPlaneDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoTabPlaneDragger);

  // FIXME: SO_KIT_ADD_CATALOG_ENTRY for all entries in this dragger's
  // catalog. 20000107 mortene.
  // cornerScaleCoords
  // cornerScaleTab0
  // cornerScaleTab1
  // cornerScaleTab2
  // cornerScaleTab3
  // edgeScaleCoords
  // edgeScaleTab0
  // edgeScaleTab1
  // edgeScaleTab2
  // edgeScaleTab3
  // planeSwitch
  // scaleTabHints
  // scaleTabMaterial
  // scaleTabMaterialBinding
  // scaleTabNormal
  // scaleTabNormalBinding
  // scaleTabs
  // translator

  // FIXME: should the fields of this class be added to the field list
  // with SO_NODE_ADD_FIELD()? 20000107 mortene.

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
