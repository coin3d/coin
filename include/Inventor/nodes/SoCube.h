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

#ifndef COIN_SOCUBE_H
#define COIN_SOCUBE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/nodes/SoShape.h>

class SoIndexedFaceSet;


class COIN_DLL_EXPORT SoCube : public SoShape {
    typedef SoShape inherited;

  SO_NODE_HEADER(SoCube);

public:
  static void initClass(void);
  SoCube(void);

  SoSFFloat width;
  SoSFFloat height;
  SoSFFloat depth;

  virtual void GLRender(SoGLRenderAction * action);
  virtual SbBool willUpdateNormalizeElement(SoState *) const;
  virtual void rayPick(SoRayPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

protected:
  virtual ~SoCube();

  virtual void generatePrimitives(SoAction * action);
  virtual void computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center);

private:
  void getHalfSize(float & w, float & h, float & d);
};

#endif // !COIN_SOCUBE_H
