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

#ifndef COIN_SOBYTESTREAM_H
#define COIN_SOBYTESTREAM_H

#include <Inventor/system/inttypes.h>
#include <Inventor/SbBasic.h>

class SoNode;
class SoPath;
class SoPathList;


class COIN_DLL_API SoByteStream {

public:
  SoByteStream(void);
  ~SoByteStream();

  void convert(SoNode * node, SbBool binary = TRUE);
  void convert(SoPath * path, SbBool binary = TRUE);
  void convert(SoPathList * pl, SbBool binary = TRUE);

  void * getData(void);
  uint32_t getNumBytes(void);

  static SoPathList * unconvert(SoByteStream * stream);
  static SoPathList * unconvert(void * data, uint32_t bytesinstream);

  void copy(void * d, size_t len);
  SbBool isRawData(void) const;

private:
  // Placeholder for any data for the instance. Just added for the
  // sake of making it possible to implement this class after release
  // without breaking ABI compatibility.
  void * pimpl;
};

#endif // COIN_SOBYTESTREAM_H
