#ifndef COIN_SOINTERSECTIONDETECTIONACTION_H
#define COIN_SOINTERSECTIONDETECTIONACTION_H

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

#include <Inventor/actions/SoSubAction.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoCallbackAction.h>

struct SoIntersectingPrimitive {
  SoPath * path;
  enum PrimitiveType {
    SEGMENT = 2,
    LINE_SEGMENT = 2,
    TRIANGLE = 3
  } type;
  SbVec3f vertex[3];
  SbVec3f xf_vertex[3];
};

class SoIntersectionDetectionActionP;

class COIN_DLL_API SoIntersectionDetectionAction : public SoAction {
  typedef SoAction hinherited;
  SO_ACTION_HEADER(SoIntersectionDetectionAction);
public:
  static void initClass(void);
  SoIntersectionDetectionAction(void);
  virtual ~SoIntersectionDetectionAction(void);

  enum Resp {
    NEXT_PRIMITIVE,
    NEXT_SHAPE,
    ABORT
  };

  typedef SoCallbackAction::Response SoIntersectionVisitationCB(void * closure, const SoPath * where);
  typedef SbBool SoIntersectionFilterCB(void * closure, const SoPath * p1, const SoPath * p2);
  typedef Resp SoIntersectionCB(void * closure, const SoIntersectingPrimitive * p1, const SoIntersectingPrimitive * p2);

  void setIntersectionDetectionEpsilon(float epsilon);
  float getIntersectionDetectionEpsilon(void) const;

  static void setIntersectionEpsilon(float epsilon);
  static float getIntersectionEpsilon(void);

  void setTypeEnabled(SoType type, SbBool enable);
  SbBool isTypeEnabled(SoType type, SbBool checkgroups = FALSE) const;

  void setManipsEnabled(SbBool enable);
  SbBool isManipsEnabled(void) const;

  void setDraggersEnabled(SbBool enable);
  SbBool isDraggersEnabled(void) const;

  void setShapeInternalsEnabled(SbBool enable);
  SbBool isShapeInternalsEnabled(void) const;

  void addVisitationCallback(SoType type, SoIntersectionVisitationCB * cb, void * closure);
  void removeVisitationCallback(SoType type, SoIntersectionVisitationCB * cb, void * closure);

  virtual void apply(SoNode * node);
  virtual void apply(SoPath * path);
  virtual void apply(const SoPathList & paths, SbBool obeysRules = FALSE);

  virtual void setFilterCallback(SoIntersectionFilterCB * cb, void * closure = NULL);
  virtual void addIntersectionCallback(SoIntersectionCB * cb, void * closure  = NULL);
  virtual void removeIntersectionCallback(SoIntersectionCB * cb, void * closure  = NULL);

private:
  SoIntersectionDetectionActionP * pimpl;
};

#endif // !COIN_SOINTERSECTIONDETECTIONACTION_H
