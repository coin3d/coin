/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SOEXTSELECTION_H
#define COIN_SOEXTSELECTION_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/fields/SoSFEnum.h>

class SbColor;

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

  SoSFEnum lassoType;
  SoSFEnum lassoPolicy;

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

protected:
  virtual ~SoExtSelection();

private:
  void draw(SoGLRenderAction * action);

  friend class SoExtSelectionP;
  class SoExtSelectionP * pimpl;
};

#endif // !COIN_SOEXTSELECTION_H
