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

#ifndef COIN_SOFILE_H
#define COIN_SOFILE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoSFString.h>

class SoGroup;


class SoFile : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoFile);

public:
  static void initClass(void);
  SoFile(void);

  SoSFString name;

  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void handleEvent(SoHandleEventAction * action);
  virtual void pick(SoPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

  SoGroup * copyChildren(void) const;
  virtual SoChildList * getChildren(void) const;
  virtual void copyContents(const SoFieldContainer * fromFC,
                            SbBool copyConnections);

protected:
  virtual ~SoFile();

  virtual SbBool readInstance(SoInput * in, unsigned short flags);

private:
  SbString path;
  class SoChildList * children;
  SbBool readState;
};

#endif // !COIN_SOFILE_H
