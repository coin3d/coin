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
  \class SoPackedColorV20 SoPackedColorV20.h
  \brief The SoPackedColorV20 class is a node is for Inventor V2.0 support only.
  \ingroup nodes

  \sa SoPackedColor
*/

#include "upgraders/SoPackedColorV20.h"

#include <Inventor/nodes/SoPackedColor.h>

#include "nodes/SoSubNodeP.h"

/*!
  \var SoMFUInt32 SoPackedColorV20::rgba

  Set of packed 32-bit RGBA vectors.

  The most significant 8 bits specifies the transparency value, where
  0x00 means completely transparent, and 0xff completely opaque.

  The least significant 24 bits specifies 8 bits each for the blue,
  green and red components.
*/

// *************************************************************************

SO_NODE_SOURCE(SoPackedColorV20);

/*!
  Constructor.
*/
SoPackedColorV20::SoPackedColorV20()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoPackedColorV20);

  SO_NODE_ADD_FIELD(rgba, (0xffcccccc));
}

/*!
  Destructor.
*/
SoPackedColorV20::~SoPackedColorV20()
{
}

// Doc from superclass.
void
SoPackedColorV20::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoPackedColorV20, SoNode::INVENTOR_2_0|SoNode::INVENTOR_1);
}

SoPackedColor * 
SoPackedColorV20::createUpgrade(void) const
{
  SoPackedColor * pp = new SoPackedColor;
  pp->ref();

  const int n = this->rgba.getNum();
  const uint32_t * src = this->rgba.getValues(0);
  pp->orderedRGBA.setNum(n);
  uint32_t * dst = pp->orderedRGBA.startEditing();
  for (int i = 0; i < n; i++) {
    uint32_t val = src[i];
    dst[i] = (val<<24)|((val<<8)&0xff0000)|((val>>8)&0xff00)|(val>>24);
  }
  pp->orderedRGBA.finishEditing();

  pp->unrefNoDelete();
  return pp;
}
