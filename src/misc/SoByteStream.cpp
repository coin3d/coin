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

#include <Inventor/misc/SoByteStream.h>
#include <coindefs.h> // COIN_STUB
#include <stdlib.h>


SoByteStream::SoByteStream(void)
{
  COIN_STUB();
}

SoByteStream::~SoByteStream()
{
  COIN_STUB();
}

void
SoByteStream::convert(SoNode * node, SbBool binary)
{
  COIN_STUB();
}

void
SoByteStream::convert(SoPath * path, SbBool binary)
{
  COIN_STUB();
}

void
SoByteStream::convert(SoPathList * pl, SbBool binary)
{
  COIN_STUB();
}

void *
SoByteStream::getData(void)
{
  COIN_STUB();
  return NULL;
}

uint32_t
SoByteStream::getNumBytes(void)
{
  COIN_STUB();
  return 0;
}

SoPathList *
SoByteStream::unconvert(SoByteStream * stream)
{
  COIN_STUB();
  return NULL;
}

SoPathList *
SoByteStream::unconvert(void * data, uint32_t bytesinstream)
{
  COIN_STUB();
  return NULL;
}

void
SoByteStream::copy(void * d, size_t len)
{
  COIN_STUB();
}

SbBool
SoByteStream::isRawData(void) const
{
  COIN_STUB();
  return FALSE;
}
