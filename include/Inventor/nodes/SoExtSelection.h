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

#ifndef COIN_SOEXTSELECTION_H
#define COIN_SOEXTSELECTION_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/SbColor.h>


class COIN_DLL_EXPORT SoExtSelection : public SoSelection {
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

  void useOverlay(SbBool flg = TRUE);
  SbBool isUsingOverlay(void);
  SoSeparator * getOverlaySceneGraph(void);
  void setOverlayLassoColorIndex(int index);
  int getOverlayLassoColorIndex(void);
  void setLassoColor(SbColor c);
  SbColor getLassoColor(void);
  void setLassoWidth(float width);
  float getLassoWidth(void);
  void setOverlayLassoPattern(unsigned short pattern);
  unsigned short getOverlayLassoPattern(void);
  void animateOverlayLasso(SbBool flg = TRUE);
  SbBool isOverlayLassoAnimated(void);

protected:
  virtual ~SoExtSelection();
};

#endif // !COIN_SOEXTSELECTION_H
