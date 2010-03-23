#ifndef COIN_SOTEXTURECUBEMAP_H
#define COIN_SOTEXTURECUBEMAP_H

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
#include <Inventor/nodes/SoTexture.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFImage.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/elements/SoMultiTextureImageElement.h>

class SoFieldSensor;
class SoSensor;
class SoTextureCubeMapP;

class COIN_DLL_API SoTextureCubeMap : public SoTexture {
  typedef SoTexture inherited;

  SO_NODE_HEADER(SoTextureCubeMap);

public:
  static void initClass(void);
  SoTextureCubeMap(void);

  enum Model {
    MODULATE = SoMultiTextureImageElement::MODULATE,
    DECAL = SoMultiTextureImageElement::DECAL,
    BLEND = SoMultiTextureImageElement::BLEND,
    REPLACE = SoMultiTextureImageElement::REPLACE
  };

  enum Wrap {
    REPEAT = SoMultiTextureImageElement::REPEAT,
    CLAMP = SoMultiTextureImageElement::CLAMP
  };

  SoMFString filenames;

  SoSFImage imagePosX;
  SoSFImage imageNegX;
  SoSFImage imagePosY;
  SoSFImage imageNegY;
  SoSFImage imagePosZ;
  SoSFImage imageNegZ;

  SoSFEnum wrapS;
  SoSFEnum wrapT;
  SoSFEnum model;
  SoSFColor blendColor;

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void rayPick(SoRayPickAction * action);

  static SbBool readImage(const SbString & fname, int & w, int & h, int & nc,
                          unsigned char *& bytes);
protected:
  virtual ~SoTextureCubeMap();

  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  virtual void notify(SoNotList * list);
  int getReadStatus(void);
  void setReadStatus(int s);

private:
  SbBool loadFilename(const SbString & filename, SoSFImage * image);
  static void filenameSensorCB(void *, SoSensor *);
  SoSFImage * getImageField(const int idx);

  SoTextureCubeMapP * pimpl;
};

#endif // !COIN_SOTEXTURECUBEMAP_H
