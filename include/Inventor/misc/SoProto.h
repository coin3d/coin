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

#ifndef COIN_SOPROTO_H
#define COIN_SOPROTO_H

#include <Inventor/misc/SoBase.h>

class SoInput;
class SoNode;
class SoProtoInstance;

class COIN_DLL_API SoProto : public SoBase {
public:
  SoProto(void);

  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId(void);

  static SoProto * findProto(const SbName & name);
  static SoProtoInstance * findProtoInstance(SoNode * protoinstanceroot);

  static void initClass(void);
  
  SoProtoInstance * createProtoInstance(void);
  void addISReference(SoField * f, const SbName & interfacename);

  void addReference(const SbName & name, SoBase * base);
  void removeReference(const SbName & name);
  SoBase * findReference(const SbName & name) const;

  void addRoute(const SbName & fromnode, const SbName & fromfield,
                const SbName & tonode, const SbName & tofield);

  SbName getProtoName(void) const;

  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  virtual void writeInstance(SoOutput * out);

protected:

  virtual ~SoProto();
  virtual void destroy(void);

private:
  SbBool readInterface(SoInput * in);
  SbBool readDefinition(SoInput * in);

  class SoProtoP * pimpl;
  SoNode * createInstanceRoot(SoProtoInstance * inst) const;
  void connectIsRefs(SoProtoInstance * inst, SoNode * src, SoNode * dst) const;
};

#endif // COIN_SOPROTO_H
