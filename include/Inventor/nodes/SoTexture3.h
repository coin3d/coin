#ifndef COIN_SOTEXTURE3_H
#define COIN_SOTEXTURE3_H

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
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFImage3.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/elements/SoTextureImageElement.h>

class SoFieldSensor;
class SoSensor;

class COIN_DLL_API SoTexture3 : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoTexture3);

public:
  static void initClass(void);
  SoTexture3(void);

  enum Model {
    MODULATE = SoTextureImageElement::MODULATE,
    DECAL = SoTextureImageElement::DECAL,
    BLEND = SoTextureImageElement::BLEND
  };

  enum Wrap {
    REPEAT = SoTextureImageElement::REPEAT,
    CLAMP = SoTextureImageElement::CLAMP
  };

  SoMFString filenames;
  SoSFImage3 images;
  SoSFEnum wrapR;
  SoSFEnum wrapS;
  SoSFEnum wrapT;
  SoSFEnum model;
  SoSFColor blendColor;

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
  SbBool loadFilenames();
  int readstatus;
  class SoGLImage *glimage;
  SbBool glimagevalid;

  class SoFieldSensor *filenamesensor;
  static void filenameSensorCB(void *, SoSensor *);
};

#endif // !COIN_SOTEXTURE3_H
