/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#ifndef COIN_SBIMAGE_H
#define COIN_SBIMAGE_H

#include <Inventor/SbVec2s.h>
#include <Inventor/SbString.h>
#include <stddef.h> // for NULL

class SbImage;

typedef SbBool SbImageScheduleReadCB(const SbString &, SbImage *, void *);

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

  SbBool hasData(void) const;

private:

  class SbImageP * pimpl;
  
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
