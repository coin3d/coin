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

#ifndef COIN_SOVRMLIMAGETEXTURE_H
#define COIN_SOVRMLIMAGETEXTURE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/VRMLnodes/SoVRMLTexture.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/SbImage.h>

class SoVRMLImageTexture;
class SoSensor;
class SbImage;

typedef SbBool VRMLPrequalifyFileCallback( const SbString &, void *,
                                           SoVRMLImageTexture * );

class COIN_DLL_API SoVRMLImageTexture : public SoVRMLTexture
{
  typedef SoVRMLTexture inherited;
  SO_NODE_HEADER(SoVRMLImageTexture);

public:
  static void initClass(void);
  SoVRMLImageTexture(void);

  SoMFString url;

  static void setPrequalifyFileCallBack(VRMLPrequalifyFileCallback * cb,
                                        void * closure);
  void allowPrequalifyFile(SbBool enable);

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);

  const SbImage * getImage(void) const;

  static void setImageDataMaxAge(const uint32_t maxage);

protected:
  virtual ~SoVRMLImageTexture();

  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  int getReadStatus(void) const;
  void setReadStatus(int status);

private:

  SbBool loadUrl(void);
  class SoVRMLImageTextureP * pimpl;
  static void urlSensorCB(void *, SoSensor *);
  static void glimage_callback(void * closure);
  static SbBool image_read_cb(const SbString &, SbImage *, void *);
  static void read_thread(void * closure);
}; // class SoVRMLImageTexture

#endif // ! COIN_SOVRMLIMAGETEXTURE_H
