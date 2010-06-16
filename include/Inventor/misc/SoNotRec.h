#ifndef COIN_SONOTREC_H
#define COIN_SONOTREC_H

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
#include <stdio.h>

class SoBase;


class COIN_DLL_API SoNotRec {
public:
  enum Type {
    CONTAINER,
    PARENT,
    SENSOR,
    FIELD,
    ENGINE
  };

  enum OperationType {
    UNSPECIFIED,
    FIELD_UPDATE,
    GROUP_ADDCHILD,
    GROUP_INSERTCHILD,
    GROUP_REPLACECHILD,
    GROUP_REMOVECHILD,
    GROUP_REMOVEALLCHILDREN
  };

  SoNotRec(SoBase * const notifbase);
  void setType(const SoNotRec::Type type);
  SoBase * getBase(void) const;
  SoNotRec::Type getType(void) const;
  const SoNotRec * getPrevious(void) const;
  void setPrevious(const SoNotRec * const prev);
  void print(FILE * const file) const;
  SoNotRec::OperationType getOperationType(void) const;
  int getIndex(void) const;
  int getFieldNumIndices(void) const;
  const SoBase * getGroupChild(void) const;
  const SoBase * getGroupPrevChild(void) const;
  void setOperationType(const SoNotRec::OperationType opType);
  void setIndex(const int idx);
  void setFieldNumIndices(const int fldnumind);
  void setGroupChild(const SoBase * const gc);
  void setGroupPrevChild(const SoBase * const pc);

private:
  Type type;
  SoBase * base;
  const SoNotRec * prev;
  int index, fieldNumIndices;
  OperationType operationType;
  const SoBase * groupChild;
  const SoBase * groupPrevChild;
};

#endif // !COIN_SONOTREC_H
