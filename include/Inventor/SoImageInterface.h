/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#ifndef _SO_IMAGE_INTERFACE_H_
#define _SO_IMAGE_INTERFACE_H_

#include <Inventor/SbBasic.h>
#include <Inventor/SbString.h>
#include <Inventor/SbVec2s.h>

class SoImageInterface {
public:
  SoImageInterface(const char * const filename);
  SoImageInterface(const SbVec2s size,
		   const int numComponents,
		   const unsigned char * data);
  ~SoImageInterface();
  
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

  void ref();
  void unref();

private:
  SbString filename;
  SbVec2s orgSize;
  int orgNumComponents;

  SbVec2s size;
  int numComponents;
  unsigned char *dataPtr;
  int refCount;
  unsigned int hasTried : 1;
  unsigned int didAlloc : 1;
};

#endif // _SO_IMAGE_INTERFACE_H_

