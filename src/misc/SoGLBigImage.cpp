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

/*!
  \class SoGLBigImage include/Inventor/misc/SoGLBigImage.h
  \brief The SoGLBigImage class is used to handle 2D OpenGL textures of any size.

  This class is currently under heavy development, and is probably
  very buggy.  Don't use it unless you really know what you're doing!
*/

#include <Inventor/misc/SoGLBigImage.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/elements/SoGLTextureImageElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>
#include <Inventor/errors/SoDebugError.h>

#ifndef DOXYGEN_SKIP_THIS

class SoGLBigImageP {
public:
  SoGLBigImageP(void) :
    imagesize(0,0),
    remain(0,0),
    dim(0,0),
    tmpbuf(NULL),
    tmpbufsize(0),
    glimagearray(NULL),
    glimagediv(NULL) {}
  
  ~SoGLBigImageP() {
    assert(this->glimagearray == NULL);
    delete[] this->tmpbuf;
  }
  SbVec2s imagesize;
  SbVec2s remain;
  SbVec2s dim;
  unsigned char * tmpbuf;
  int tmpbufsize;
  SoGLImage ** glimagearray;
  int * glimagediv;
  SbVec2f tcmul;
  
  void copySubImage(const int idx,
                    const unsigned char * src,
                    const SbVec2s & fullsize,
                    const int nc,
                    unsigned char * dst,
                    const int div);


  void reset(SoState * state = NULL) {
    const int n = this->dim[0] * this->dim[1];
    for (int i = 0; i < n; i++) {
      if (this->glimagearray[i]) {
        this->glimagearray[i]->unref(state);
        this->glimagearray[i] = NULL;
      }
    }
    delete[] this->glimagearray;
    delete[] this->glimagediv;
    this->glimagearray = NULL;
    this->glimagediv = NULL;
    this->imagesize.setValue(0,0);
    this->remain.setValue(0,0);
    this->dim.setValue(0,0);
  }
};

#endif // DOXYGEN_SKIP_THIS

#undef THIS
#define THIS this->pimpl

/*!
  Constructor.
*/
SoGLBigImage::SoGLBigImage(void)
{
  THIS = new SoGLBigImageP;
}

/*!
  Destructor.
*/
SoGLBigImage::~SoGLBigImage()
{
  THIS->reset(NULL);
  delete THIS;
}

void
SoGLBigImage::unref(SoState * state)
{
  THIS->reset(state);
  inherited::unref(state);
}

static SoType classTypeId;

/*!
  Returns the type id for this class.
*/
SoType
SoGLBigImage::getClassTypeId(void)
{
  if (classTypeId.isBad()) {
    classTypeId = SoType::createType(SoGLImage::getClassTypeId(),
                                     SbName("GLBigImage"));
  }
  return classTypeId;
}

/*!
  Returns the type id for an SoGLBigImage instance.
*/
SoType
SoGLBigImage::getTypeId(void) const
{
  return SoGLBigImage::getClassTypeId();
}

void
SoGLBigImage::setData(const unsigned char * bytes,
                      const SbVec2s size,
                      const int nc,
                      const Wrap wraps,
                      const Wrap wrapt,
                      const float quality,
                      const int border,
                      SoState * createinstate)
{
  if (createinstate) {
    SoDebugError::postWarning("SoGLBigImage::setData",
                              "createinstate must be NULL for SoGLBigImage");
  }
  THIS->reset(NULL);
  inherited::setData(bytes, size, nc, wraps, wrapt, quality, border, NULL);
}

SoGLDisplayList *
SoGLBigImage::getGLDisplayList(SoState * state)
{
  return NULL;
}

int
SoGLBigImage::initSubImages(SoState * state,
                            const SbVec2s & subimagesize) const
{
  // FIXME: support not using full resolution when projsize is small
  if (subimagesize == THIS->imagesize) return THIS->dim[0] * THIS->dim[1];
  THIS->reset(state);
  THIS->imagesize = subimagesize;

  const unsigned char * bytes = this->getDataPtr();
  const SbVec2s size = this->getSize();
  const int nc = this->getNumComponents();

  THIS->dim[0] = size[0] / subimagesize[0];
  THIS->dim[1] = size[1] / subimagesize[1];

  THIS->remain[0] = size[0] % subimagesize[0];
  if (THIS->remain[0]) THIS->dim[0] += 1;
  THIS->remain[1] = size[1] % subimagesize[1];
  if (THIS->remain[1]) THIS->dim[1] += 1;

  const int numimages = THIS->dim[0] * THIS->dim[1];

  THIS->glimagediv = new int[numimages];
  THIS->glimagearray = new SoGLImage*[numimages];
  for (int i = 0; i < numimages; i++) {
    THIS->glimagearray[i] = NULL;
    THIS->glimagediv[i] = 1;
  }

  THIS->tcmul[0] = float(THIS->dim[0] * subimagesize[0]) / float(size[0]);
  THIS->tcmul[1] = float(THIS->dim[1] * subimagesize[1]) / float(size[1]);

  return THIS->dim[0] * THIS->dim[1];
}

void
SoGLBigImage::handleSubImage(const int idx,
                             SbVec2f & start,
                             SbVec2f & end,
                             SbVec2f & tcmul)
{
  SbVec2s pos(idx % THIS->dim[0], idx / THIS->dim[0]);
  start[0] = float(pos[0]) / float(THIS->dim[0]);
  start[1] = float(pos[1]) / float(THIS->dim[1]);
  end[0] = float(pos[0]+1) / float(THIS->dim[0]);
  end[1] = float(pos[1]+1) / float(THIS->dim[1]);

  start[0] *= THIS->tcmul[0];
  start[1] *= THIS->tcmul[1];
  end[0] *= THIS->tcmul[0];
  end[1] *= THIS->tcmul[1];
  tcmul = THIS->tcmul;
}

void
SoGLBigImage::applySubImage(SoState * state, const int idx, 
                            const float quality,
                            const SbVec2s & projsize)
{
  int div = 2;
  while ((THIS->imagesize[0]/div > projsize[0]) && 
         (THIS->imagesize[1]/div > projsize[1])) div <<= 1;
  div >>= 1;
    
  if (THIS->glimagearray[idx] == NULL || THIS->glimagediv[idx] != div) {
    if (THIS->glimagearray[idx] == NULL) 
      THIS->glimagearray[idx] = new SoGLImage();
    THIS->glimagediv[idx] = div;

    THIS->glimagearray[idx]->setFlags(SoGLImage::NO_MIPMAP|
                                      SoGLImage::LINEAR_MIN_FILTER|
                                      SoGLImage::LINEAR_MAG_FILTER);
    SbVec2s actualsize(THIS->imagesize[0]/div,
                       THIS->imagesize[1]/div);
    
    int numbytes = actualsize[0]*actualsize[1]*this->getNumComponents();
    if (numbytes > THIS->tmpbufsize) {
      delete[] THIS->tmpbuf;
      THIS->tmpbuf = new unsigned char[numbytes];
      THIS->tmpbufsize = numbytes;
    }
    THIS->copySubImage(idx, 
                       this->getDataPtr(),
                       this->getSize(),
                       this->getNumComponents(),
                       THIS->tmpbuf, div);
    THIS->glimagearray[idx]->setData(THIS->tmpbuf,
                                     actualsize,
                                     this->getNumComponents(),
                                     SoGLImage::CLAMP_TO_EDGE,
                                     SoGLImage::CLAMP_TO_EDGE,
                                     quality,
                                     0, state);
  }

  SoGLDisplayList * dl = THIS->glimagearray[idx]->getGLDisplayList(state);
  dl->call(state);
}

#undef THIS

void
SoGLBigImageP::copySubImage(const int idx, 
                            const unsigned char * src,
                            const SbVec2s & fullsize,
                            const int nc,
                            unsigned char * dst,
                            const int div)
{
  SbVec2s pos(idx % this->dim[0], idx / this->dim[0]);

  // FIXME: investigate if it's possible to set the pixel transfer
  // mode so that we don't have to copy the data into a temporary
  // image. This is probably fast enough though.

  SbVec2s origin;
  origin[0] = pos[0] * this->imagesize[0];
  origin[1] = pos[1] * this->imagesize[1];

  for (int y = 0; y < int(this->imagesize[1]); y += div) {
    for (int x = 0; x < int(this->imagesize[0]); x += div) {
      if ((origin[0] + x) < fullsize[0] && (origin[1] + y) < fullsize[1]) {
        const unsigned char * srcptr = 
          src + nc * (fullsize[0] * (origin[1]+y) + origin[0]+x);
        for (int c = 0; c < nc; c++) {
          *dst++ = srcptr[c];
        }
      }
      else {
        for (int c = 0; c < nc; c++) *dst++ = 0xff;
      }
    }
  }
}
