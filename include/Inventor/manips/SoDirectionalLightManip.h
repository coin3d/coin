#ifndef COIN_SODIRECTIONALLIGHTMANIP_H
#define COIN_SODIRECTIONALLIGHTMANIP_H

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

#include <Inventor/nodes/SoDirectionalLight.h>
#ifndef COIN_INTERNAL
#include <Inventor/draggers/SoDragger.h>
#endif

class SoChildList;
class SoDragger;
class SoFieldSensor;
class SoPath;
class SoSensor;
class SbVec3f;
class SbMatrix;

class COIN_DLL_API SoDirectionalLightManip : public SoDirectionalLight {
  typedef SoDirectionalLight inherited;

  SO_NODE_HEADER(SoDirectionalLightManip);

public:
  static void initClass(void);
  SoDirectionalLightManip(void);

  SoDragger * getDragger(void);
  SbBool replaceNode(SoPath * path);
  SbBool replaceManip(SoPath * path, SoDirectionalLight * newone) const;

  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void handleEvent(SoHandleEventAction * action);
  virtual void pick(SoPickAction * action);
  virtual void search(SoSearchAction * action);

  virtual SoChildList * getChildren(void) const;

protected:
  ~SoDirectionalLightManip();

  void setDragger(SoDragger * newdragger);

  virtual void copyContents(const SoFieldContainer * fromfc, SbBool copyconnections);
  static void transferFieldValues(const SoDirectionalLight * from, SoDirectionalLight * to);

  static void valueChangedCB(void * f, SoDragger * d);
  static void fieldSensorCB(void * f, SoSensor * d);

  SoFieldSensor * directionFieldSensor;
  SoFieldSensor * colorFieldSensor;
  SoChildList * children;

private:
  void attachSensors(const SbBool onoff);

};

#endif // !COIN_SODIRECTIONALLIGHTMANIP_H
