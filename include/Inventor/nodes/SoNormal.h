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

#ifndef __SONORMAL_H__
#define __SONORMAL_H__

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/nodes/SoNode.h>

class SbVec3f;


class SoNormal : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoNormal);

public:
  static void initClass(void);
  SoNormal(void);

  // old stuff from VRMLView, but pretty smart, I think
//   enum VectorType {
//     UNKNOWN,
//     UNITVECTORS,
//     NOT_UNITVECTORS
//   };
//   SbVec3f *getNormalizedVectors();

  SoMFVec3f vector;

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void pick(SoPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

protected:
  virtual ~SoNormal();

private:
// VRMLView stuff, temporaryly disabled
//  SbVec3f *normalizedVectors;
//  VectorType vectorType;         // unit or notunit ?
};

#endif // !__SONORMAL_H__
