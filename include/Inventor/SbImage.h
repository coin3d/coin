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

#ifndef COIN_SBIMAGE_H
#define COIN_SBIMAGE_H

#include <Inventor/SbVec2s.h>
#include <stddef.h> // for NULL

class SbString;

class SbImage {
public:
  SbImage(void);
  SbImage(const unsigned char * bytes,
          const SbVec2s & size, const int bytesperpixel);
  ~SbImage(void);

  void setValue(const SbVec2s & size, const int bytesperpixel,
                const unsigned char * bytes);
  unsigned char * getValue(SbVec2s & size, int & bytesperpixel) const;

  SbBool readFile(const SbString & filename,
                const SbString * const * searchdirectories = NULL,
                const int numdirectories = 0);

  int operator==(const SbImage & image) const;
  int operator!=(const SbImage & image) const {
    return ! operator == (image);
  }
private:
  unsigned char * bytes;
  SbVec2s size;
  int bpp;
};

#endif // COIN_SBIMAGE_H
