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

#ifndef COIN_SOVRMLAPPEARANCE_H
#define COIN_SOVRMLAPPEARANCE_H

#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFNode.h>

class COIN_DLL_API SoVRMLAppearance : public SoNode
{
  typedef SoNode inherited;
  SO_NODE_HEADER(SoVRMLAppearance);

public:
  static void initClass(void);

  SoVRMLAppearance(void);

  SoSFNode material;
  SoSFNode texture;
  SoSFNode textureTransform;

  virtual void doAction( SoAction * action );
  virtual void callback( SoCallbackAction * action );
  virtual void GLRender( SoGLRenderAction * action );
  virtual void search( SoSearchAction * action );

  virtual SoChildList * getChildren(void) const;
  virtual void notify(SoNotList * list);
  virtual void copyContents(const SoFieldContainer * from, SbBool copyConn);

protected:
  virtual ~SoVRMLAppearance();
  SoChildList * childlist;
  SbBool childlistvalid;
}; // class SoVRMLAppearance

#endif // ! COIN_SOVRMLAPPEARANCE_H
