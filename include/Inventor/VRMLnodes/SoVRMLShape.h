/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#ifndef COIN_SOVRMLSHAPE_H
#define COIN_SOVRMLSHAPE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFEnum.h>

class SoVRMLShapeP;

class COIN_DLL_API SoVRMLShape : public SoNode
{
  typedef SoNode inherited;
  SO_NODE_HEADER(SoVRMLShape);

public:
  static void initClass(void);
  SoVRMLShape(void);

  enum CacheEnabled {
    OFF,
    ON,
    AUTO
  };

  SoSFNode appearance;
  SoSFNode geometry;
  SoSFEnum renderCaching;
  SoSFEnum boundingBoxCaching;

  static void setNumRenderCaches(int num);
  static int getNumRenderCaches(void);

  virtual SbBool affectsState(void) const;

  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void rayPick(SoRayPickAction * action);
  virtual void search(SoSearchAction * action);
  virtual void write(SoWriteAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

  virtual SoChildList * getChildren(void) const;

  virtual void notify(SoNotList * list);
  virtual void copyContents(const SoFieldContainer * from, SbBool copyConn);

protected:
  virtual ~SoVRMLShape();

private:
  static int numrendercaches;
  SoVRMLShapeP * pimpl;

}; // class SoVRMLShape

#endif // ! COIN_SOVRMLSHAPE_H
