#ifndef COIN_SOSHADOWGROUP_H
#define COIN_SOSHADOWGROUP_H

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
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFInt32.h>

class SoShadowGroupP;

class COIN_DLL_API SoShadowGroup : public SoSeparator {
  typedef SoSeparator inherited;

  SO_NODE_HEADER(SoShadowGroup);

public:
  static void initClass(void);
  static void init(void);

  static SbBool isSupported(void);

  SoShadowGroup(void);

  enum VisibilityFlag {
    ABSOLUTE_RADIUS,
    LONGEST_BBOX_EDGE_FACTOR,
    PROJECTED_BBOX_DEPTH_FACTOR
  };

  SoSFBool isActive;
  SoSFFloat intensity;
  SoSFFloat precision;
  SoSFFloat quality;
  SoSFFloat smoothBorder;
  SoSFBool shadowCachingEnabled;
  SoSFFloat visibilityNearRadius;
  SoSFFloat visibilityRadius;
  SoSFEnum visibilityFlag;
  
  SoSFFloat epsilon;
  SoSFFloat threshold;

  virtual void GLRenderBelowPath(SoGLRenderAction * action);
  virtual void GLRenderInPath(SoGLRenderAction * action);

  virtual void notify(SoNotList * nl);

  void enableSubgraphSearchOnNotify(const SbBool onoff);

protected:
  virtual ~SoShadowGroup();

private:
  SoShadowGroupP * pimpl;

};

#endif // COIN_SOSHADOWGROUP_H
