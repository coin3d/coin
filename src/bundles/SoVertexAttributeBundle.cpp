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

/*!
  \class SoVertexAttributeBundle include/Inventor/SoVertexAttributeBundle.h
  \brief The SoVertexAttributeBundle class simplifies vertex attribute handling.
  \ingroup bundles

*/

#include <Inventor/bundles/SoVertexAttributeBundle.h>
#include <Inventor/actions/SoAction.h>
#include <cassert>
#include "SbBasicP.h"

/*!
  Constructor.
*/
SoVertexAttributeBundle::SoVertexAttributeBundle(SoAction * action, SbBool forrendering)
  : SoBundle(action)
{
  this->elem = SoVertexAttributeElement::getInstance(this->state);
  this->glelem = NULL;
  if (forrendering) {
    this->glelem = static_cast<const SoGLVertexAttributeElement *> (this->elem);
  }
}

/*!
  Destructor.
*/
SoVertexAttributeBundle::~SoVertexAttributeBundle()
{
}

SbBool 
SoVertexAttributeBundle::doAttributes(void) const
{
  return (this->elem->getNumAttributes() > 0);
}

/*!
  Send the index'th attribute to OpenGL.
*/
void 
SoVertexAttributeBundle::send(int index) const
{
  assert(this->glelem);
  this->glelem->send(index);
}
