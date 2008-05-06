#ifndef COIN_SOFOREIGNFILEKIT_H
#define COIN_SOFOREIGNFILEKIT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/SbBasic.h>

#include <Inventor/nodekits/SoSubKit.h>
#include <Inventor/nodekits/SoBaseKit.h>

#include <Inventor/fields/SoSFString.h>

typedef SbBool SoForeignFileIdentifyFunc(const char *);

class SoForeignFileKitP;

class COIN_DLL_API SoForeignFileKit : public SoBaseKit {
  typedef SoBaseKit inherited;

  SO_KIT_ABSTRACT_HEADER(SoForeignFileKit);
  SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);

public:
  static void initClass(void);

  SoSFString filename;

  static SbBool isFileSupported(SoInput * in, SbBool exhaust = FALSE);
  static SoForeignFileKit * createForeignFileKit(SoInput * in, SbBool exhaust = FALSE);

  static SbBool isFileSupported(const char * filename, SbBool exhaust = FALSE);
  static SoForeignFileKit * createForeignFileKit(const char * filename, SbBool exhaust = FALSE);

  // read support
  virtual SbBool canReadFile(const char * filename = NULL) const;
  virtual SbBool readFile(const char * filename);
  virtual SbBool canReadScene(void) const;
  virtual SbBool readScene(SoNode * scene);
  // write support
  virtual SbBool canWriteFile(const char * filename = NULL) const;
  virtual SbBool writeFile(const char * filename);
  virtual SbBool canWriteScene(const char * format = NULL) const;
  virtual SbBool writeScene(SoNode *& root, const char * format = NULL);

protected:
  static void initClasses(void);

  SoForeignFileKit(void);
  virtual ~SoForeignFileKit(void);

  static SbBool registerFileExtension(SoType handler, SbName extension, SoForeignFileIdentifyFunc * identify);

private:
  SoForeignFileKitP * pimpl;

}; // SoForeignFileKit

#endif // !COIN_SOFOREIGNFILEKIT_H
