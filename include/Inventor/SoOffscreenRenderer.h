/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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

#ifndef COIN_SOOFFSCREENRENDERER_H
#define COIN_SOOFFSCREENRENDERER_H

#include <Inventor/SbViewportRegion.h>
#include <Inventor/SbColor.h>
#include <stdio.h>

class SoBase;
class SoGLRenderAction;
class SoNode;
class SoPath;


class SoOffscreenRenderer {
public:
  enum Components {
    LUMINANCE = 1,
    LUMINANCE_TRANSPARENCY = 2,
    RGB = 3,
    RGB_TRANSPARENCY = 4
  };

  SoOffscreenRenderer(const SbViewportRegion & viewportregion);
  SoOffscreenRenderer(SoGLRenderAction * action);
  ~SoOffscreenRenderer();

  static float getScreenPixelsPerInch(void);
  static SbVec2s getMaximumResolution(void);
  void setComponents(const Components components);
  Components getComponents(void) const;
  void setViewportRegion(const SbViewportRegion & region);
  const SbViewportRegion & getViewportRegion(void) const;
  void setBackgroundColor(const SbColor & color);
  const SbColor & getBackgroundColor(void) const;
  void setGLRenderAction(SoGLRenderAction * action);
  SoGLRenderAction * getGLRenderAction(void) const;
  SbBool render(SoNode * scene);
  SbBool render(SoPath * scene);
  unsigned char * getBuffer(void) const;

  SbBool writeToRGB(FILE * fp) const;
  SbBool writeToPostScript(FILE * fp) const;
  SbBool writeToPostScript(FILE * fp, const SbVec2f & printsize) const;

private:
  SbBool renderFromBase(SoBase * base);
  void convertBuffer(void);

  SbViewportRegion viewport;
  SbColor backgroundcolor;
  Components components;
  SoGLRenderAction * renderaction;
  SbBool didallocaction;
  class SoOffscreenInternalData * internaldata;
  unsigned char * buffer;
};


#endif // !COIN_SOOFFSCREENRENDERER_H
