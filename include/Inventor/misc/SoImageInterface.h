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

#ifndef COIN_SOIMAGEINTERFACE_H
#define COIN_SOIMAGEINTERFACE_H

#include <Inventor/SbBasic.h>
#include <Inventor/SbString.h>
#include <Inventor/SbVec2s.h>

class SbStringList;


class COIN_DLL_EXPORT SoImageInterface {
public:
  SoImageInterface(const SbVec2s size,
                   const int numComponents,
                   const unsigned char * data);
  SbBool resize(const SbVec2s newsize);
  //void changeDepth(const int newDepth);
  SbBool load(const SbBool forceTry = FALSE);
  SbBool isLoaded() const;
  const unsigned char *getDataPtr() const;
  int getNumComponents() const;
  SbVec2s getSize() const;
  const char *getFilename() const;
  int getOrgNumComponents() const;
  SbVec2s getOriginalSize() const;
  SbBool hasTransparency() const;

  void ref();
  void unref(); // use this to delete

  SoImageInterface *imageCopy() const;
  static SoImageInterface * findOrCreateImage(const char * const filename,
                                              const SbStringList & dirlist);

private:
  friend class dummyClass; // avoid warnings on stupid compilers

  static void unrefImage(SoImageInterface * const image);

  SoImageInterface(const char * const filename);
  ~SoImageInterface();

  void checkTransparency();

  SbString filename;
  SbVec2s orgSize;
  int orgNumComponents;

  SbVec2s size;
  int numComponents;
  unsigned char *dataPtr;
  int refCount;
  SbBool hasTried;
  SbBool didAlloc;
  SbBool transparency;
  SbBool isReuseable;
};

#endif // !COIN_SOIMAGEINTERFACE_H
