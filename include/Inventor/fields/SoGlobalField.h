/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOGLOBALFIELD_H
#define COIN_SOGLOBALFIELD_H

#include <Inventor/fields/SoFieldContainer.h>


class COIN_DLL_API SoGlobalField : public SoFieldContainer {
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
