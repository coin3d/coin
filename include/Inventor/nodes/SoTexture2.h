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

#ifndef COIN_SOTEXTURE2_H
#define COIN_SOTEXTURE2_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFImage.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/elements/SoTextureImageElement.h>

class SoFieldSensor;
class SoSensor;

class COIN_DLL_API SoTexture2 : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoTexture2);

public:
  static void initClass(void);
  SoTexture2(void);

  enum Model {
    MODULATE = SoTextureImageElement::MODULATE,
    DECAL = SoTextureImageElement::DECAL,
    BLEND = SoTextureImageElement::BLEND
  };

  enum Wrap {
    REPEAT = SoTextureImageElement::REPEAT,
    CLAMP = SoTextureImageElement::CLAMP
  };

  SoSFString filename;
  SoSFImage image;
  SoSFEnum wrapS;
  SoSFEnum wrapT;
  SoSFEnum model;
  SoSFColor blendColor;

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);

  static SbBool readImage(const SbString & fname, int & w, int & h, int & nc,
                          unsigned char *& bytes);
protected:
  virtual ~SoTexture2();

  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  virtual void notify(SoNotList * list);
  int getReadStatus(void);
  void setReadStatus(int s);

private:
  SbBool loadFilename(void);
  int readstatus;
  class SoGLImage * glimage;
  SbBool glimagevalid;

  class SoFieldSensor * filenamesensor;
  static void filenameSensorCB(void *, SoSensor *);
};

#endif // !COIN_SOTEXTURE2_H
