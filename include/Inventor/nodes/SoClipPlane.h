/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#ifndef __SOCLIPPLANE_H__
#define __SOCLIPPLANE_H__

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoSFPlane.h>
#include <Inventor/fields/SoSFBool.h>


class SoClipPlane : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoClipPlane);

public:
  static void initClass(void);
  SoClipPlane(void);

  SoSFPlane plane;
  SoSFBool on;

  virtual void doAction(SoAction *action);
  virtual void GLRender(SoGLRenderAction *action);
  virtual void callback(SoCallbackAction *action);
  virtual void pick(SoPickAction *action);

protected:
  virtual ~SoClipPlane();
};

#endif // !__SOCLIPPLANE_H__
