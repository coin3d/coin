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

#include <Inventor/manips/SoJackManip.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/draggers/SoJackDragger.h>

SO_NODE_SOURCE(SoJackManip);


void
SoJackManip::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoJackManip);
}

SoJackManip::SoJackManip(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoJackManip);

  SoJackDragger *dragger = new SoJackDragger;
  this->setDragger(dragger);

  SoSurroundScale *ss = (SoSurroundScale*) dragger->getPart("surroundScale", TRUE);
  assert(ss);
  ss->numNodesUpToContainer = 4;
  ss->numNodesUpToReset = 3;
}


SoJackManip::~SoJackManip()
{
}
