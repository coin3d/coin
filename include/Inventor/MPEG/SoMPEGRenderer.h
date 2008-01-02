#ifndef COIN_SOMPEGRENDERER_H
#define COIN_SOMPEGRENDERER_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <stdio.h>

#include <Inventor/SbColor.h>
#include <Inventor/SbVec2s.h>

class SoNode;
class SoGLRenderAction;

// *************************************************************************

class COIN_DLL_API SoMPEGRenderer {
public:
  SoMPEGRenderer(void);
  virtual ~SoMPEGRenderer();

  SbBool openFile(const char * filename, unsigned int frames = 10000);
  void closeFile(void);
  void setFilePointer(FILE * fp);
  FILE * getFilePointer(void) const;

  void addFrame(const unsigned char * frame);

  void setCompressionRate(float rate);
  void setBitPerSec(float bps);
  void setNumFramesPerSecond(int fps);

  float getCompressionRate(void) const;
  int getNumFramesPerSecond(void) const;

  void setSize(const SbVec2s & s);
  SbVec2s getSize(void) const;

  virtual void setSceneGraph(SoNode * root);
  SoNode * getSceneGraph(void) const;

  void setBackgroundColor(const SbColor & col);
  SbColor getBackgroundColor(void) const;

  void setGLRenderAction(SoGLRenderAction * action);
  SoGLRenderAction * getGLRenderAction(void) const;

private:
  class SoMPEGRendererP * pimpl;
};

#endif // ! COIN_SOMPEGRENDERER_H
