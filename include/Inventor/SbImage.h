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

#ifndef COIN_SBIMAGE_H
#define COIN_SBIMAGE_H

#include <Inventor/SbVec2s.h>
#include <Inventor/SbString.h>
#include <stddef.h> // for NULL

class SbImage;

typedef void SbImageScheduleReadCB(const SbString &, SbImage *, void *);

class COIN_DLL_API SbImage {
public:
  SbImage(void);
  SbImage(const unsigned char * bytes,
          const SbVec2s & size, const int bytesperpixel);
  ~SbImage(void);

  void setValue(const SbVec2s & size, const int bytesperpixel,
                const unsigned char * bytes);
  void setValuePtr(const SbVec2s & size, const int bytesperpixel,
                   const unsigned char * bytes);
  unsigned char * getValue(SbVec2s & size, int & bytesperpixel) const;

  SbBool readFile(const SbString & filename,
                  const SbString * const * searchdirectories = NULL,
                  const int numdirectories = 0);

  int operator==(const SbImage & image) const;
  int operator!=(const SbImage & image) const {
    return ! operator == (image);
  }

  static SbString searchForFile(const SbString & basename,
                                const SbString * const * dirlist,
                                const int numdirs);

private:

  // FIXME: remove these and replace with SbImageP * pimpl when it's
  // ok to break ABI compatibility. pederb, 2001-11-06
  unsigned char * bytes_obsoleted;
  SbVec2s size_obsoleted;
  int bpp_obsoleted;

public:

  // methods for delaying image loading until it is actually needed.
  void readLock(void) const;
  void readUnlock(void) const;
  
  SbBool scheduleReadFile(SbImageScheduleReadCB * cb,
                          void * closure,
                          const SbString & filename,
                          const SbString * const * searchdirectories = NULL,
                          const int numdirectories = 0);
};

#endif // COIN_SBIMAGE_H
