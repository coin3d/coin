/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
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

  virtual void doAction( SoAction * action );
  virtual void GLRender( SoGLRenderAction * action );
  virtual void callback( SoCallbackAction * action );

protected:
  virtual ~SoVRMLPixelTexture();

  virtual SbBool readInstance( SoInput * in, unsigned short flags );
  virtual void notify(SoNotList * list);


private:
  SoVRMLPixelTextureP * pimpl;
}; // class SoVRMLPixelTexture

#endif // ! COIN_SOVRMLPIXELTEXTURE_H
