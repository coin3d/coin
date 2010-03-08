#ifndef COIN_SOEXTSELECTION_H
#define COIN_SOEXTSELECTION_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/fields/SoSFEnum.h>
#include <stddef.h> // NULL

#ifndef COIN_INTERNAL
#include <Inventor/actions/SoCallbackAction.h>
#endif // !COIN_INTERNAL

class SbColor;
class SbVec3f;
class SbVec2f;
class SbVec2s;
class SbViewportRegion;
class SoPrimitiveVertex;

// This shouldn't strictly be necessary, but the OSF1/cxx compiler
// complains if this is left out, while using the "friend class
// SoExtSelectionP" statement in the class definition.
class SoExtSelectionP;


typedef SbBool SoExtSelectionTriangleCB(void * userdata,
                                        SoCallbackAction * action,
                                        const SoPrimitiveVertex * v1,
                                        const SoPrimitiveVertex * v2,
                                        const SoPrimitiveVertex * v3);

typedef SbBool SoExtSelectionLineSegmentCB(void * userdata,
                                           SoCallbackAction * action,
                                           const SoPrimitiveVertex * v1,
                                           const SoPrimitiveVertex * v2);

typedef SbBool SoExtSelectionPointCB(void * userdata,
                                     SoCallbackAction * action,
                                     const SoPrimitiveVertex * v1);

typedef SoPath * SoLassoSelectionFilterCB(void * userdata, const SoPath * path);


class COIN_DLL_API SoExtSelection : public SoSelection {
  typedef SoSelection inherited;

  SO_NODE_HEADER(SoExtSelection);

public:
  static void initClass(void);
  SoExtSelection(void);

  enum LassoType {
    NOLASSO, LASSO, RECTANGLE
  };

  enum LassoPolicy {
    FULL_BBOX, PART_BBOX, FULL, PART
  };

  enum LassoMode {
    ALL_SHAPES,
    VISIBLE_SHAPES
  };

  SoSFEnum lassoType;
  SoSFEnum lassoPolicy;
  SoSFEnum lassoMode;

  void useOverlay(SbBool overlay = TRUE);
  SbBool isUsingOverlay(void);
  SoSeparator * getOverlaySceneGraph(void);
  void setOverlayLassoColorIndex(const int index);
  int getOverlayLassoColorIndex(void);
  void setLassoColor(const SbColor & color);
  const SbColor & getLassoColor(void);
  void setLassoWidth(const float width);
  float getLassoWidth(void);
  void setOverlayLassoPattern(const unsigned short pattern);
  unsigned short getOverlayLassoPattern(void);
  void animateOverlayLasso(const SbBool animate = TRUE);
  SbBool isOverlayLassoAnimated(void);

  virtual void handleEvent(SoHandleEventAction * action);
  virtual void GLRenderBelowPath(SoGLRenderAction * action);

  void select(SoNode * root, int numcoords, SbVec2f * lasso, 
              const SbViewportRegion & vp, SbBool shiftpolicy);
  void select(SoNode * root, int numcoords, SbVec3f * lasso,
              const SbViewportRegion & vp, SbBool shiftkeypolicy);
  const SbVec2s * getLassoCoordsDC(int & numCoords);
  const SbVec3f * getLassoCoordsWC(int & numCoords);
  const SoPathList & getSelectionPathList() const;

  void setLassoFilterCallback(SoLassoSelectionFilterCB * f, void * userdata = NULL,
                              const SbBool callonlyifselectable = TRUE);

  void setTriangleFilterCallback(SoExtSelectionTriangleCB * func,
                                 void * userdata = NULL);
  void setLineSegmentFilterCallback(SoExtSelectionLineSegmentCB * func,
                                    void * userdata = NULL);
  void setPointFilterCallback(SoExtSelectionPointCB * func,
                              void * userdata = NULL);
  SbBool wasShiftDown(void) const;

protected:
  virtual ~SoExtSelection();

private:
  void draw(SoGLRenderAction * action);

  friend class SoExtSelectionP;
  class SoExtSelectionP * pimpl;
};

#endif // !COIN_SOEXTSELECTION_H
