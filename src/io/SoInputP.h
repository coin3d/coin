#ifndef COIN_SOINPUTP_H
#define COIN_SOINPUTP_H

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

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* ! COIN_INTERNAL */

// *************************************************************************

#include "misc/SbHash.h"

class SoInput;
class SoInput_FileInfo;

// *************************************************************************

class SoInputP {
public:
  SoInputP(SoInput * owner) {
    this->owner = owner;
    this->usingstdin = FALSE;
  }

  static SbBool debug(void);
  static SbBool debugBinary(void);

  SoInput_FileInfo * getTopOfStackPopOnEOF(void);

  static SbBool isNameStartChar(unsigned char c, SbBool validIdent);
  static SbBool isNameChar(unsigned char c, SbBool validIdent);
  static SbBool isNameStartCharVRML1(unsigned char c, SbBool validIdent);
  static SbBool isNameCharVRML1(unsigned char c, SbBool validIdent);
  static SbBool isNameStartCharVRML2(unsigned char c, SbBool validIdent);
  static SbBool isNameCharVRML2(unsigned char c, SbBool validIdent);

  SbBool usingstdin;

  SbHash<const char *, SoBase *> copied_references;

private:
  SoInput * owner;
};

// *************************************************************************

#endif // ! COIN_SOINPUTP_H
