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

#ifndef COIN_SOPROFILE_H
#define COIN_SOPROFILE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/elements/SoProfileElement.h>

class SbVec2f;
class SoState;


class COIN_DLL_EXPORT SoProfile : public SoNode {
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
