/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
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
