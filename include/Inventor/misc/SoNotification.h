#ifndef COIN_SONOTIFICATION_H
#define COIN_SONOTIFICATION_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/misc/SoNotRec.h>
#include <stdio.h>

class SoEngineOutput;
class SoField;


class COIN_DLL_API SoNotList {
public:
  SoNotList(void);
  SoNotList(const SoNotList * nl);

  void append(SoNotRec * const rec);
  void append(SoNotRec * const rec, SoField * const field);
  void append(SoNotRec * const rec, SoEngineOutput * const engineout);
  void setLastType(const SoNotRec::Type type);
  SoNotRec * getFirstRec(void) const;
  SoNotRec * getLastRec(void) const;
  SoNotRec * getFirstRecAtNode(void) const;
  SoField * getLastField(void) const;
  SoEngineOutput * getLastEngineOutput(void) const;
  uint32_t getTimeStamp(void) const;

  void print(FILE * const file = stdout) const;

private:
  SoNotRec * head;
  SoNotRec * tail;
  SoNotRec * firstnoderec;
  SoField * lastfield;
  SoEngineOutput * lastengine;
  uint32_t stamp;
};

#endif // !COIN_SONOTIFICATION_H
