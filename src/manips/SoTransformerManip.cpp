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

#include <Inventor/manips/SoTransformerManip.h>
#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/draggers/SoTransformerDragger.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

SO_NODE_SOURCE(SoTransformerManip);


void
SoTransformerManip::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTransformerManip);
}

SoTransformerManip::SoTransformerManip(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTransformerManip);

  SoTransformerDragger *dragger = new SoTransformerDragger;
  this->setDragger(dragger);
  
  SoSurroundScale *ss = (SoSurroundScale*) dragger->getPart("surroundScale", TRUE);
  ss->numNodesUpToContainer = 4;
  ss->numNodesUpToReset = 3;
}


SoTransformerManip::~SoTransformerManip()
{
}

SbBool
SoTransformerManip::isLocateHighlighting(void)
{
  SoDragger *dragger = this->getDragger();
  if (dragger && dragger->isOfType(SoTransformerDragger::getClassTypeId())) {
    return ((SoTransformerDragger*)dragger)->isLocateHighlighting();
  }
#if COIN_DEBUG
  SoDebugError::postWarning("SoTransformerManip::isLocateHighlighting",
                            "Not a valid dragger in manipulator");
#endif // debug
  return FALSE;
}

void
SoTransformerManip::setLocateHighlighting(SbBool onoff)
{
  SoDragger *dragger = this->getDragger();
  if (dragger && dragger->isOfType(SoTransformerDragger::getClassTypeId())) {
    ((SoTransformerDragger*)dragger)->setLocateHighlighting(onoff);
  }
  else {
#if COIN_DEBUG
    SoDebugError::postWarning("SoTransformerManip::setLocateHighlighting",
                              "Not a valid dragger in manipulator");
#endif // debug
  }
}

void
SoTransformerManip::unsquishKnobs(void)
{
  SoDragger *dragger = this->getDragger();
  if (dragger && dragger->isOfType(SoTransformerDragger::getClassTypeId())) {
    ((SoTransformerDragger*)dragger)->unsquishKnobs();
  }
  else {
#if COIN_DEBUG
    SoDebugError::postWarning("SoTransformerManip::setLocateHighlighting",
                              "Not a valid dragger in manipulator");
#endif // debug
  }
}
