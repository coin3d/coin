/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
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
