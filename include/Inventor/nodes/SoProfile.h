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

#ifndef __SOPROFILE_H__
#define __SOPROFILE_H__

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoSFEnum.h>

class SbVec2f;
class SoState;


class SoProfile : public SoNode {
  typedef SoNode inherited;

  SO_NODE_ABSTRACT_HEADER(SoProfile);

public:
  static void initClass(void);

  enum Profile {
    START_FIRST, START_NEW, ADD_TO_CURRENT
  };

  SoMFInt32 index;
  SoSFEnum linkage;

  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void pick(SoPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

  virtual void getTrimCurve(SoState * state, int32_t & numPoints,
                            float *& points, int & floatsPerVec,
                            int32_t & numKnots, float *& knotVector) = 0;
  virtual void getVertices(SoState * state, int32_t & nVertices,
                           SbVec2f *& vertices) = 0;

protected:
  SoProfile(void);
  virtual ~SoProfile();
};

#endif // !__SOPROFILE_H__
