#ifndef COIN_SOTEXTURE3_H
#define COIN_SOTEXTURE3_H

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
#include <Inventor/fields/SoSFImage3.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/elements/SoMultiTextureImageElement.h>

class SoFieldSensor;
class SoSensor;

class COIN_DLL_API SoTexture3 : public SoTexture {
  typedef SoTexture inherited;

  SO_NODE_HEADER(SoTexture3);

public:
  static void initClass(void);
  SoTexture3(void);

  enum Model {
    MODULATE = SoMultiTextureImageElement::MODULATE,
    DECAL = SoMultiTextureImageElement::DECAL,
    BLEND = SoMultiTextureImageElement::BLEND
  };

  enum Wrap {
    REPEAT = SoMultiTextureImageElement::REPEAT,
    CLAMP = SoMultiTextureImageElement::CLAMP
  };

  SoMFString filenames;
  SoSFImage3 images;
  SoSFEnum wrapR;
  SoSFEnum wrapS;
  SoSFEnum wrapT;
  SoSFEnum model;
  SoSFColor blendColor;
  SoSFBool enableCompressedTexture;

  virtual void doAction(SoAction *action);
  virtual void GLRender(SoGLRenderAction *action);
  virtual void callback(SoCallbackAction *action);

protected:
  virtual ~SoTexture3();

  virtual SbBool readInstance(SoInput *in, unsigned short flags);
  virtual void notify(SoNotList *list);
  int getReadStatus(void);
  void setReadStatus(int s);

private:
  SbBool loadFilenames(SoInput * in = NULL);
  int readstatus;
  class SoGLImage *glimage;
  SbBool glimagevalid;

  class SoFieldSensor *filenamesensor;
  static void filenameSensorCB(void *, SoSensor *);
};

#endif // !COIN_SOTEXTURE3_H
