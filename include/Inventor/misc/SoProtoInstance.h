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

#ifndef COIN_SOPROTOINSTANCE_H
#define COIN_SOPROTOINSTANCE_H

#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSubNode.h>

class SoProto;
class SoSensor;

class COIN_DLL_API SoProtoInstance : public SoNode {
  typedef SoNode inherited;

  // The following definitions are used instead of SO_NODE_HEADER() to
  // let SoProtoInstance have dynamic handling of SoFieldData objects.

  PRIVATE_NODE_TYPESYSTEM_HEADER();
protected:
  virtual const SoFieldData * getFieldData(void) const;
private:
  SoFieldData * classfielddata;

  // Node definition starts "proper".

public:
  static void initClass(void);

  SoProtoInstance(SoProto * proto,
                  const SoFieldData * deffielddata);

  void setRootNode(SoNode * root);
  SoNode * getRootNode(void);

  SoProto * getPROTODefinition(void) const;
  SbName getProtoName(void) const;

protected:
  virtual ~SoProtoInstance();

  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  virtual void writeInstance(SoOutput * out);

private:

  static void sensorCB(void * data, SoSensor * sensor);
  void copyFieldData(const SoFieldData * src);
  class SoProtoInstanceP * pimpl;
};

#endif // COIN_SOPROTOINSTANCE_H
