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

#ifndef COIN_SODIRECTIONALLIGHT_H
#define COIN_SODIRECTIONALLIGHT_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoLight.h>
#include <Inventor/fields/SoSFVec3f.h>


class COIN_DLL_EXPORT SoDirectionalLight : public SoLight {
  typedef SoLight inherited;

  SO_NODE_HEADER(SoDirectionalLight);

public:
  static void initClass(void);
  SoDirectionalLight(void);

  SoSFVec3f direction;

  virtual void GLRender(SoGLRenderAction * action);

protected:
  virtual ~SoDirectionalLight();
};

#endif // !COIN_SODIRECTIONALLIGHT_H
