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

#ifndef COIN_SOSEPARATOR_H
#define COIN_SOSEPARATOR_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/nodes/SoGroup.h>

class SoState;

class COIN_DLL_EXPORT SoSeparator : public SoGroup {
  typedef SoGroup inherited;

  SO_NODE_HEADER(SoSeparator);

public:
  static void initClass(void);
  SoSeparator(void);
  SoSeparator(const int nchildren);

  enum CacheEnabled {
    OFF, ON, AUTO
  };

  SoSFEnum renderCaching;
  SoSFEnum boundingBoxCaching;
  SoSFEnum renderCulling;
  SoSFEnum pickCulling;


  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void GLRenderBelowPath(SoGLRenderAction * action);
  virtual void GLRenderInPath(SoGLRenderAction * action);
  virtual void GLRenderOffPath(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void handleEvent(SoHandleEventAction * action);
  virtual void rayPick(SoRayPickAction * action);
  virtual void search(SoSearchAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

  virtual void notify(SoNotList * nl);

  static void setNumRenderCaches(const int howmany);
  static int getNumRenderCaches(void);
  virtual SbBool affectsState(void) const;


protected:
  virtual ~SoSeparator();

  virtual SbBool cullTest(SoState * state);
  virtual SbBool cullTest(SoGLRenderAction * action, int & cullresults);
  virtual SbBool readInstance(SoInput * in, unsigned short flags);


private:
  void commonConstructor(void);

  class SoBoundingBoxCache * bboxcache;
  static int numrendercaches;
};

#endif // !COIN_SOSEPARATOR_H
