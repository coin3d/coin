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

// FIXME: this class has not been implemented yet. 20000627 mortene.

#include <Inventor/misc/SoTranSender.h>
#include <coindefs.h> // COIN_STUB
#include <stdlib.h>

SoTranSender::SoTranSender(SoOutput * COIN_UNUSED_ARG(output))
{
  COIN_STUB();
}

SoTranSender::~SoTranSender()
{
  COIN_STUB();
}

SoOutput *
SoTranSender::getOutput(void) const
{
  COIN_STUB();
  return NULL;
}

void
SoTranSender::insert(SoNode * COIN_UNUSED_ARG(node))
{
  COIN_STUB();
}

void
SoTranSender::insert(SoNode * COIN_UNUSED_ARG(node), SoNode * COIN_UNUSED_ARG(parent), int COIN_UNUSED_ARG(n))
{
  COIN_STUB();
}

void
SoTranSender::remove(SoNode * COIN_UNUSED_ARG(parent), int COIN_UNUSED_ARG(n))
{
  COIN_STUB();
}

void
SoTranSender::replace(SoNode * COIN_UNUSED_ARG(parent), int COIN_UNUSED_ARG(n), SoNode * COIN_UNUSED_ARG(newnode))
{
  COIN_STUB();
}

void
SoTranSender::modify(SoNode * COIN_UNUSED_ARG(node))
{
  COIN_STUB();
}

void
SoTranSender::prepareToSend(void)
{
  COIN_STUB();
}
