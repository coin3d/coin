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

// FIXME: this class has not been implemented yet. 20000627 mortene.

#include <Inventor/misc/SoTranSender.h>
#include <coindefs.h> // COIN_STUB
#include <stdlib.h>

SoTranSender::SoTranSender(SoOutput * output)
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
SoTranSender::insert(SoNode * node)
{
  COIN_STUB();
}

void
SoTranSender::insert(SoNode * node, SoNode * parent, int n)
{
  COIN_STUB();
}

void
SoTranSender::remove(SoNode * parent, int n)
{
  COIN_STUB();
}

void
SoTranSender::replace(SoNode * parent, int n, SoNode * newnode)
{
  COIN_STUB();
}

void
SoTranSender::modify(SoNode * node)
{
  COIN_STUB();
}

void
SoTranSender::prepareToSend(void)
{
  COIN_STUB();
}
