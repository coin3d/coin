#ifndef COIN_SOPROTO_H
#define COIN_SOPROTO_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

// NB: This is work-in-progress, and the API might change from day to
// day. Do not use this class unless you are prepared for this.
// pederb, 2002-05-28

#include <Inventor/nodes/SoNode.h>

class SoInput;
class SoProtoInstance;

// We need to inherit SoNode to be able to insert the PROTO definition
// into the scene graph.
class COIN_DLL_API SoProto : public SoNode {
public:
  SoProto(const SbBool externproto = FALSE);

  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId(void);

  static SoProto * findProto(const SbName & name);
  static void initClass(void);

  SoProtoInstance * createProtoInstance(void);
  void addISReference(SoNode * container,
                      const SbName & fieldname,
                      const SbName & interfacename);
  SbName findISReference(const SoFieldContainer * container,
                         const SbName & fieldname);

  void addReference(const SbName & name, SoBase * base);
  void removeReference(const SbName & name);
  SoBase * findReference(const SbName & name) const;

  void addRoute(const SbName & fromnode, const SbName & fromfield,
                const SbName & tonode, const SbName & tofield);

  SbName getProtoName(void) const;

  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  virtual void write(SoWriteAction * action);

protected:

  virtual ~SoProto();
  virtual void destroy(void);

private:
  SbBool writeInterface(SoOutput * out);
  SbBool writeDefinition(SoWriteAction * action);

  SbBool readInterface(SoInput * in);
  SbBool readDefinition(SoInput * in);

  SbBool writeURLs(SoOutput * out);
  class SoProtoP * pimpl;

  SoNode * createInstanceRoot(SoProtoInstance * inst) const;
  void connectISRefs(SoProtoInstance * inst, SoNode * src, SoNode * dst) const;
};

#endif // !COIN_SOPROTO_H
