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

#ifndef __SOMATERIAL_H__
#define __SOMATERIAL_H__

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoMFColor.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/nodes/SoNode.h>


class SoMaterial : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoMaterial);

public:
  static void initClass(void);
  SoMaterial(void);

  SoMFColor ambientColor;
  SoMFColor diffuseColor;
  SoMFColor specularColor;
  SoMFColor emissiveColor;
  SoMFFloat shininess;
  SoMFFloat transparency;

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);

protected:
  virtual ~SoMaterial();

private:
  // -- FIXME: not in the OIV API. Remove? 19990611 mortene
#if 0 // OBSOLETED 1999-12-06, pederb
  float compareAppearance(int thisIdx,SoMaterial * otherMaterial,int otherIdx);
  float compareAppearanceVRML2(SoMaterial * otherMaterial);
#endif // OBSOLETED
};

#endif // !__SOMATERIAL_H__
