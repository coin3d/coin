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

/*!
  \class SoMPEGRenderer SoMPEGRenderer.h Inventor/MPEG/SoMPEGRenderer.h
  \brief The SoMPEGRenderer class is the base class for MPEG movie generation.
  \ingroup misc

  FIXME: docs

  \since Coin 2.4
  \since TGS Inventor 3.1
*/

// *************************************************************************

#include <Inventor/MPEG/SoMPEGRenderer.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoNode.h>

// *************************************************************************

#ifndef COIN_BETA_VERSION
#error This must either be implemented properly or taken out of the build before the next official release.
#endif // COIN_BETA_VERSION

// *************************************************************************

class SoMPEGRendererP {
public:
  SoMPEGRendererP(void)
  {
    // Default values, as decided by TGS's public API for the
    // SoMPEGRenderer:
    this->compressionrate = 0.3f;
    this->bps = 1205862;
    this->fps = 30;
    this->canvassize.setValue(96, 96);
    // FIXME: check OIV_BACKGROUND_COLOR envvar. 20040823 mortene.
    this->backgroundcol.setValue(0, 0, 0);

    this->renderaction = new SoGLRenderAction(SbViewportRegion(this->canvassize));
    this->allocedaction = TRUE;

    this->root = NULL;
  }

  ~SoMPEGRendererP()
  {
    if (this->allocedaction) { delete this->renderaction; }
    if (this->root) { this->root->unref(); }
  }

  float compressionrate;
  float bps;
  int fps;
  SbVec2s canvassize;
  SbColor backgroundcol;

  SoGLRenderAction * renderaction;
  SbBool allocedaction;

  SoNode * root;
};

#define PRIVATE(p) ((p)->pimpl)

// *************************************************************************

SoMPEGRenderer::SoMPEGRenderer(void)
{
  SoDebugError::postWarning("SoMPEGRenderer::SoMPEGRenderer",
                            "This class not yet properly implemented, just "
                            "stubbed. No MPEG output will be written.");

  PRIVATE(this) = new SoMPEGRendererP;
}

SoMPEGRenderer::~SoMPEGRenderer()
{
  delete PRIVATE(this);
}

// *************************************************************************

SbBool
SoMPEGRenderer::openFile(const char * filename, unsigned int frames)
{
  // FIXME: implement
  return FALSE;
}

void
SoMPEGRenderer::closeFile(void)
{
  // FIXME: implement
}

void
SoMPEGRenderer::setFilePointer(FILE * fp)
{
  // FIXME: implement
}

FILE *
SoMPEGRenderer::getFilePointer(void) const
{
  // FIXME: implement
  return NULL;
}

// *************************************************************************

void
SoMPEGRenderer::addFrame(const unsigned char * frame)
{
  // FIXME: implement
}

// *************************************************************************

void
SoMPEGRenderer::setCompressionRate(float rate)
{
  PRIVATE(this)->compressionrate = rate;
}

void
SoMPEGRenderer::setBitPerSec(float bps)
{
  PRIVATE(this)->bps = bps;
}

void
SoMPEGRenderer::setNumFramesPerSecond(int fps)
{
  PRIVATE(this)->fps = fps;
}

float
SoMPEGRenderer::getCompressionRate(void) const
{
  return PRIVATE(this)->compressionrate;
}

int
SoMPEGRenderer::getNumFramesPerSecond(void) const
{
  return PRIVATE(this)->fps;
}

// *************************************************************************

void
SoMPEGRenderer::setSize(const SbVec2s & s)
{
  PRIVATE(this)->canvassize = s;
}

SbVec2s
SoMPEGRenderer::getSize(void) const
{
  return PRIVATE(this)->canvassize;
}

void
SoMPEGRenderer::setSceneGraph(SoNode * root)
{
  root->ref();

  SoNode * oldroot = PRIVATE(this)->root;
  if (oldroot) { oldroot->unref(); }

  PRIVATE(this)->root = root;
}

SoNode *
SoMPEGRenderer::getSceneGraph(void) const
{
  return PRIVATE(this)->root;
}

void
SoMPEGRenderer::setBackgroundColor(const SbColor & col)
{
  PRIVATE(this)->backgroundcol = col;
}

SbColor
SoMPEGRenderer::getBackgroundColor(void) const
{
  return PRIVATE(this)->backgroundcol;
}

void
SoMPEGRenderer::setGLRenderAction(SoGLRenderAction * action)
{
  PRIVATE(this)->renderaction = action;
  PRIVATE(this)->allocedaction = FALSE;
}

SoGLRenderAction *
SoMPEGRenderer::getGLRenderAction(void) const
{
  return PRIVATE(this)->renderaction;
}

#undef PRIVATE

// *************************************************************************
