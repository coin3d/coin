/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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

#ifndef COIN_SOGROUP_H
#define COIN_SOGROUP_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>


class SoGroup : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoGroup);

public:
  static void initClass(void);
  SoGroup(void);

  SoGroup(int nchildren);

  void addChild(SoNode * const node);
  void insertChild(SoNode * const child, const int newchildindex);
  SoNode * getChild(const int index) const;
  int findChild(const SoNode * const node) const;
  int getNumChildren(void) const;
  void removeChild(const int childindex);
  void removeChild(SoNode * const child);
  void removeAllChildren(void);
  void replaceChild(const int index, SoNode * const newchild);
  void replaceChild(SoNode * const oldchild, SoNode * const newchild);

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void handleEvent(SoHandleEventAction * action);
  virtual void pick(SoPickAction * action);
  virtual void search(SoSearchAction * action);
  virtual void write(SoWriteAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
  SoChildList * getChildren(void) const;

protected:
  virtual ~SoGroup();

  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  virtual SbBool readChildren(SoInput * in);

  virtual void copyContents(const SoFieldContainer * from,
                            SbBool copyconnections);

  SoChildList * children;

  friend class SoUnknownNode; // Let SoUnknownNode access readChildren().
};

#endif // !COIN_SOGROUP_H
