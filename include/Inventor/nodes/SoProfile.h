/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SOPROFILE_H
#define COIN_SOPROFILE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/elements/SoProfileElement.h>

class SbVec2f;
class SoState;


class COIN_DLL_API SoProfile : public SoNode {
  typedef SoNode inherited;

  SO_NODE_ABSTRACT_HEADER(SoProfile);

public:
  static void initClass(void);

  enum Profile {
    START_FIRST = SoProfileElement::START_FIRST,
    START_NEW = SoProfileElement::START_NEW,
    ADD_TO_CURRENT = SoProfileElement::ADD_TO_CURRENT
  };

  SoMFInt32 index;
  SoSFEnum linkage;

  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void pick(SoPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

  virtual void getTrimCurve(SoState * state, int32_t & numpoints,
                            float *& points, int & floatspervec,
                            int32_t & numknots, float *& knotvector) = 0;
  virtual void getVertices(SoState * state, int32_t & numvertices,
                           SbVec2f *& vertices) = 0;

protected:
  SoProfile(void);
  virtual ~SoProfile();
};

#endif // !COIN_SOPROFILE_H
