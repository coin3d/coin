/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef __SOTEXTURE2_H__
#define __SOTEXTURE2_H__

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFImage.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFColor.h>

#if defined(COIN_EXCLUDE_SOTEXTURE2)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOTEXTURE2

// *************************************************************************

class SoTexture2 : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoTexture2);

public:
  static void initClass(void);
  SoTexture2(void);


  SbBool readImage();

  // Definition must match 100% with the enum in SoTextureImageElement.
  enum Model {
    MODULATE,
    DECAL,
    BLEND
  };

  // Definition must match 100% with the enum in SoTextureImageElement.
  enum Wrap {
    REPEAT,
    CLAMP
  };

  SoSFString filename;
  SoSFImage image;
  SoSFEnum wrapS;
  SoSFEnum wrapT;
  SoSFEnum model;
  SoSFColor blendColor;

#if !defined(COIN_EXCLUDE_SOACTION)
  virtual void doAction(SoAction * action);
#endif // !COIN_EXCLUDE_SOACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  virtual void GLRender(SoGLRenderAction * action);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
  virtual void callback(SoCallbackAction * action);
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

  static SbBool readImage(const SbString & fname, int & w, int & h, int & nc,
                          unsigned char *& bytes);

protected:
  virtual ~SoTexture2();

  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  int getReadStatus(void);
  void setReadStatus(int s);

private:
  void getImage();
  class SoImageInterface *imageData;
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  class SoGLImage * glImage;
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
};

#endif // !__SOTEXTURE2_H__
