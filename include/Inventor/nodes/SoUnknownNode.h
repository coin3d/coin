#ifndef COIN_SOUNKNOWNNODE_H
#define COIN_SOUNKNOWNNODE_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#include <Inventor/nodes/SoSubNode.h>

class SoUnknownNodeP;

class COIN_DLL_API SoUnknownNode : public SoNode {
  typedef SoNode inherited;

  // The following definitions are used instead of SO_NODE_HEADER() to
  // let SoUnknownNodes have dynamic handling of SoFieldData objects.

  PRIVATE_NODE_TYPESYSTEM_HEADER();
protected:
  virtual const SoFieldData * getFieldData(void) const;
private:
  SoFieldData * classfielddata;
  static void * createInstance(void);

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
