/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#ifndef COIN_SOVRMLPIXELTEXTURE_H
#define COIN_SOVRMLPIXELTEXTURE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/VRMLnodes/SoVRMLTexture.h>
#include <Inventor/fields/SoSFImage.h>

class SoVRMLPixelTextureP;

class COIN_DLL_API SoVRMLPixelTexture : public SoVRMLTexture
{
  typedef SoVRMLTexture inherited;
  SO_NODE_HEADER(SoVRMLPixelTexture);

public:
  static void initClass(void);
  SoVRMLPixelTexture(void);

  SoSFImage image;

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void rayPick(SoRayPickAction * action);

protected:
  virtual ~SoVRMLPixelTexture();

  virtual SbBool readInstance( SoInput * in, unsigned short flags );
  virtual void notify(SoNotList * list);


private:
  SoVRMLPixelTextureP * pimpl;
}; // class SoVRMLPixelTexture

#endif // ! COIN_SOVRMLPIXELTEXTURE_H
