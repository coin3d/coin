#ifndef COIN_SOUNKNOWNNODE_H
#define COIN_SOUNKNOWNNODE_H

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
#endif /* !CoiN_INTERNAL */

#include <Inventor/nodes/SoSubNode.h>

class SoUnknownNodeP;

/*
  This class is internal, but since the incorrect COIN_DLL_API tag wasn't
  discovered before the 2.0.0 release, it has to stay for the 2.x branch.
*/

class SoUnknownNode : public SoNode {
  typedef SoNode inherited;

  // The following definitions are used instead of SO_NODE_HEADER() to
  // let SoUnknownNodes have dynamic handling of SoFieldData objects.

  PRIVATE_NODE_TYPESYSTEM_HEADER();
protected:
  virtual const SoFieldData * getFieldData(void) const;
private:
  SoFieldData * classfielddata;
  static void * createInstance(void);
  static void atexit_cleanup(void);

  // Node definition starts "proper".

public:
  static void initClass(void);
  SoUnknownNode(void);

  void setNodeClassName(const SbName & name);
  virtual SoChildList * getChildren(void) const;

  virtual void GLRender(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void pick(SoPickAction * action);
  virtual void search(SoSearchAction * action);
  virtual void write(SoWriteAction * action);

protected:
  virtual ~SoUnknownNode();

  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  virtual const char * getFileFormatName(void) const;
  virtual SoNode * addToCopyDict(void) const;
  virtual void copyContents(const SoFieldContainer * from,
                            SbBool copyconnections);

private:
  
  SoUnknownNodeP * pimpl;
};

#endif // !COIN_SOUNKNOWNNODE_H
