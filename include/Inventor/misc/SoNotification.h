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

#ifndef __SONOTIFICATION_H__
#define __SONOTIFICATION_H__

#include <Inventor/system/inttypes.h>

#include <stdio.h>

class SoBase;

class SoNotRec {
public:
  enum Type {
    CONTAINER,
    PARENT,
    SENSOR,
    FIELD,
    ENGINE,
    INTERP,
  };

  SoNotRec(SoBase * const base);
  void setType(const SoNotRec::Type type);
  SoBase * getBase(void) const;
  SoNotRec::Type getType(void) const;
  const SoNotRec * getPrevious(void) const;
  void setPrevious(const SoNotRec * const prev);
  void print(FILE * const file) const;
  
private:
  Type type;
  SoBase * base;
  const SoNotRec * prev;
};

class SoEngineOutput;
class SoField;
class SoVRMLInterpOutput;

class SoNotList {
public:
  SoNotList(void);
  SoNotList(const SoNotList * const copyFrom);
  
  void append(SoNotRec * const rec);
  void append(SoNotRec * const rec, SoField * const field);
  void append(SoNotRec * const rec, SoVRMLInterpOutput * const interpout);
  void append(SoNotRec * const rec, SoEngineOutput * const engineout);
  void setLastType(const SoNotRec::Type type);
  SoNotRec * getFirstRec(void) const;
  SoNotRec * getLastRec(void) const;
  SoNotRec * getFirstRecAtNode(void) const;
  SoField * getLastField(void) const;
  SoVRMLInterpOutput * getLastInterpOutput(void) const;
  SoEngineOutput * getLastEngineOutput(void) const;
  uint32_t getTimeStamp(void) const;

  void print(FILE * const file = stdout) const;

private:
  SoNotRec * head;
  SoNotRec * tail;
  SoNotRec * firstnoderec;
  SoField * lastfield;
  SoEngineOutput * lastengine;
  SoVRMLInterpOutput * lastinterp;
  uint32_t stamp;
};

#endif // !__SONOTIFICATION_H__
