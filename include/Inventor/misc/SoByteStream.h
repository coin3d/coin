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

#ifndef COIN_SOBYTESTREAM_H
#define COIN_SOBYTESTREAM_H

#include <Inventor/system/inttypes.h>
#include <Inventor/SbBasic.h>

class SoNode;
class SoPath;
class SoPathList;


class COIN_DLL_EXPORT SoByteStream {

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
