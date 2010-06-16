/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/nodes/SoTexture.h>

/*!
  SoTexture SoTexture.h Inventor/nodes/SoTexture.h
  \brief Common base class for texture nodes.

  \ingroup nodes
  \COIN_CLASS_EXTENSION
  \since Coin 3.0
*/

SO_NODE_ABSTRACT_SOURCE(SoTexture);

void
SoTexture::initClass(void)
{
  SO_NODE_INIT_ABSTRACT_CLASS(SoTexture, SoNode, Node);
}

void
SoTexture::cleanupClass(void)
{
}

SoTexture::SoTexture(void)
{
  SO_NODE_CONSTRUCTOR(SoTexture);
}

SoTexture::~SoTexture(void)
{
}

void
SoTexture::doAction(SoAction * action)
{
  inherited::doAction(action);
}

void
SoTexture::GLRender(SoGLRenderAction * action)
{
  inherited::GLRender(action);
}

void
SoTexture::callback(SoCallbackAction * action)
{
  inherited::callback(action);
}
