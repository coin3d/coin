#ifndef COIN_SOASCIITEXT_H
#define COIN_SOASCIITEXT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/lists/SbList.h>

class COIN_DLL_API SoAsciiText : public SoShape {
  typedef SoShape inherited;

  SO_NODE_HEADER(SoAsciiText);

public:
  static void initClass(void);
  SoAsciiText(void);

  enum Justification {
    LEFT = 1,
    RIGHT,
    CENTER
  };

  SoMFString string;
  SoSFFloat spacing;
  SoSFEnum justification;
  SoMFFloat width;

  virtual void GLRender(SoGLRenderAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

protected:
  virtual ~SoAsciiText();

  virtual void computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center);
  virtual void generatePrimitives(SoAction *);

  virtual SoDetail * createTriangleDetail(SoRayPickAction * action,
                                          const SoPrimitiveVertex * v1,
                                          const SoPrimitiveVertex * v2,
                                          const SoPrimitiveVertex * v3,
                                          SoPickedPoint * pp);
  virtual void notify(SoNotList * list);
  
private:
  class SoAsciiTextP * pimpl;
  friend class SoAsciiTextP;

  float getWidth(const int idx, const float fontsize);
};

#endif // !COIN_SOASCIITEXT_H
