/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
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

typedef SbBool VRMLPrequalifyFileCallback(const SbString &, void *,
                                          SoVRMLImageTexture *);

class COIN_DLL_API SoVRMLImageTexture : public SoVRMLTexture
{
  typedef SoVRMLTexture inherited;
  SO_NODE_HEADER(SoVRMLImageTexture);

public:
  static void initClass(void);
  SoVRMLImageTexture(void);

  SoMFString url;

  static void setDelayFetchURL(const SbBool onoff);
  static void setPrequalifyFileCallBack(VRMLPrequalifyFileCallback * cb,
                                        void * closure);
  void allowPrequalifyFile(SbBool enable);

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void rayPick(SoRayPickAction * action);

  void setImage(const SbImage & image);
  const SbImage * getImage(void) const;

  static void setImageDataMaxAge(const uint32_t maxage);

protected:
  virtual ~SoVRMLImageTexture();

  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  int getReadStatus(void) const;
  void setReadStatus(int status);

private:

  SbBool readImage(const SbString & filename);
  SbBool loadUrl(void);
  class SoVRMLImageTextureP * pimpl;
  static void urlSensorCB(void *, SoSensor *);
  static void glimage_callback(void * closure);
  static SbBool image_read_cb(const SbString &, SbImage *, void *);
  static void read_thread(void * closure);
  static SbBool default_prequalify_cb(const SbString & url,  void * closure, 
                                      SoVRMLImageTexture * node);
  static void oneshot_readimage_cb(void *, SoSensor *);

}; // class SoVRMLImageTexture

#endif // ! COIN_SOVRMLIMAGETEXTURE_H
