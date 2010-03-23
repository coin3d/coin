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
SoByteStream::convert(SoNode * COIN_UNUSED_ARG(node), SbBool COIN_UNUSED_ARG(binary))
{
  COIN_STUB();
}

void
SoByteStream::convert(SoPath * COIN_UNUSED_ARG(path), SbBool COIN_UNUSED_ARG(binary))
{
  COIN_STUB();
}

void
SoByteStream::convert(SoPathList * COIN_UNUSED_ARG(pl), SbBool COIN_UNUSED_ARG(binary))
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
SoByteStream::unconvert(SoByteStream * COIN_UNUSED_ARG(stream))
{
  COIN_STUB();
  return NULL;
}

SoPathList *
SoByteStream::unconvert(void * COIN_UNUSED_ARG(data), uint32_t COIN_UNUSED_ARG(bytesinstream))
{
  COIN_STUB();
  return NULL;
}

void
SoByteStream::copy(void * COIN_UNUSED_ARG(d), size_t COIN_UNUSED_ARG(len))
{
  COIN_STUB();
}

SbBool
SoByteStream::isRawData(void) const
{
  COIN_STUB();
  return FALSE;
}
