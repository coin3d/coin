#ifndef COIN_SOFOREIGNFILEKIT_H
#define COIN_SOFOREIGNFILEKIT_H

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

#include <Inventor/SbBasic.h>

#include <Inventor/nodekits/SoSubKit.h>
#include <Inventor/nodekits/SoBaseKit.h>

typedef SbBool SoForeignFileIdentifyFunc(const char *);

class SoForeignFileKitP;

class COIN_DLL_API SoForeignFileKit : public SoBaseKit {
  typedef SoBaseKit inherited;

  SO_KIT_ABSTRACT_HEADER(SoForeignFileKit);
  SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);

public:
  static void initClass(void);

  static SbBool isFileSupported(SoInput * in, SbBool exhaust = FALSE);
  static SbBool isFileSupported(const char * filename, SbBool exhaust = FALSE);

  static SoForeignFileKit * createForeignFileKit(SoInput * in, SbBool exhaust = FALSE);
  static SoForeignFileKit * createForeignFileKit(const char * filename, SbBool exhaust = FALSE);

  // foreign read support
  virtual SbBool canReadFile(const char * filename = NULL) const;
  virtual SbBool readFile(const char * filename);
  // foreign write support
  virtual SbBool canWriteFile(const char * filename = NULL) const;
  virtual SbBool writeFile(const char * filename);

  /*!
    Converts (if necessary) the internal representation of the foreign file to a pure Coin scenegraph.
    Returns the root node with a refcount of 0.
  */
  virtual class SoSeparator *convert() = 0;

protected:
  static void initClasses(void);

  SoForeignFileKit(void);
  virtual ~SoForeignFileKit(void);

  static SbBool registerFileExtension(SoType handler, SbName extension, SoForeignFileIdentifyFunc * identify);

private:
  SoForeignFileKitP * pimpl;

}; // SoForeignFileKit

#endif // !COIN_SOFOREIGNFILEKIT_H
