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

// FIXME: this class has not been implemented yet. 20000627 mortene.

#include <Inventor/misc/SoTranSender.h>
#include <coindefs.h> // COIN_STUB

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
