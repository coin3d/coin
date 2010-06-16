#ifndef COIN_SOBUMPMAP_H
#define COIN_SOBUMPMAP_H

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
#include <Inventor/fields/SoSFImage.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/elements/SoMultiTextureImageElement.h>

class SoFieldSensor;
class SoSensor;
class SoBumpMapP;

class COIN_DLL_API SoBumpMap : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoBumpMap);

public:
  static void initClass(void);
  SoBumpMap(void);

  enum Wrap {
    REPEAT = SoMultiTextureImageElement::REPEAT,
    CLAMP = SoMultiTextureImageElement::CLAMP
  };

  SoSFString filename;
  SoSFImage image;
  SoSFEnum wrapS;
  SoSFEnum wrapT;

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void rayPick(SoRayPickAction * action);

protected:
  virtual ~SoBumpMap();

  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  virtual void notify(SoNotList * list);

private:
  SbBool loadFilename(void);
  static void filenameSensorCB(void *, SoSensor *);

  SoBumpMapP * pimpl;
};

#endif // !COIN_SOBUMPMAP_H
