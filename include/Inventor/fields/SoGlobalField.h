#ifndef COIN_SOGLOBALFIELD_H
#define COIN_SOGLOBALFIELD_H

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

#include <Inventor/fields/SoFieldContainer.h>

class SoGlobalField : public SoFieldContainer {
  typedef SoFieldContainer inherited;

public:
  SoGlobalField(const SbName & name, SoField * field);

  static void initClass(void);
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  SoField * getGlobalField(void) const;
  virtual const SoFieldData * getFieldData(void) const;

  virtual void setName(const SbName & newname);

  virtual void addWriteReference(SoOutput * out, SbBool isfromfield = FALSE);
  virtual void writeInstance(SoOutput * out);

  static SoGlobalField * getGlobalFieldContainer(const SbName & name);
  static void removeGlobalFieldContainer(SoGlobalField * fieldcontainer);
  static SoBaseList * getGlobalFieldContainers(void);

protected:
  ~SoGlobalField();

  virtual SbBool readInstance(SoInput * in, unsigned short flags);

private:
  static void * createInstance(void);
  static void clean(void);
  static int getGlobalFieldIndex(const SbName & name);

  static SoType classTypeId;
  SoFieldData * classfielddata;
  static SoBaseList * allcontainers;
};

#endif // !COIN_SOGLOBALFIELD_H
