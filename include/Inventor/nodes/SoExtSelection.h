#ifndef COIN_SOEXTSELECTION_H
#define COIN_SOEXTSELECTION_H

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
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/fields/SoSFEnum.h>
#include <stddef.h> // NULL

class SbColor;
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
