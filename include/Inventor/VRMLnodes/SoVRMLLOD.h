/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SOVRMLLOD_H
#define COIN_SOVRMLLOD_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoMFNode.h>

class COIN_DLL_API SoVRMLLOD : public SoGroup
{
  typedef SoGroup inherited;
  SO_NODE_HEADER(SoVRMLLOD);

public:
  static void initClass(void);
  SoVRMLLOD(void);
  SoVRMLLOD(int levels);

  SoMFFloat range;
  SoSFVec3f center;
  SoMFNode level;

  virtual SbBool affectsState(void) const;

  void addLevel(SoNode * level);
  void insertLevel(SoNode * level, int idx);
  SoNode * getLevel(int idx) const;
  int findLevel(const SoNode * level) const;
  int getNumLevels(void) const;
  void removeLevel(int idx);
  void removeLevel(SoNode * level);
  void removeAllLevels(void);
  void replaceLevel(int idx, SoNode * level);
  void replaceLevel(SoNode * old, SoNode * level);

  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void rayPick(SoRayPickAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void search(SoSearchAction * action);
  virtual void write(SoWriteAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

  virtual void GLRenderBelowPath(SoGLRenderAction * action);
  virtual void GLRenderInPath(SoGLRenderAction * action);
  virtual void GLRenderOffPath(SoGLRenderAction * action);

  void addChild(SoNode * child);
  void insertChild(SoNode * child, int idx);
  SoNode * getChild(int idx) const;
  int findChild(const SoNode * child) const;
  int getNumChildren(void) const;
  void removeChild(int idx);
  void removeChild(SoNode * child);
  void removeAllChildren(void);
  void replaceChild(int idx, SoNode * child);
  void replaceChild(SoNode * old, SoNode * child);
  virtual SoChildList * getChildren(void) const;

protected:
  virtual ~SoVRMLLOD();

  virtual void notify(SoNotList * list);
  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  virtual void copyContents(const SoFieldContainer * from, SbBool copyConn);

  virtual int whichToTraverse(SoAction * action);

private:
  void commonConstructor(void);
  SbBool childlistvalid;

}; // class SoVRMLLOD

#endif // ! COIN_SOVRMLLOD_H
