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

/*!
  \class SoGLBigImage include/Inventor/misc/SoGLBigImage.h
  \brief The SoGLBigImage class is used to handle 2D OpenGL textures of any size.

  (This class is currently under heavy development, and is probably
  somewhat buggy.  Don't use it unless you really know what you're
  doing!)

  The technique used is the following: split the texture into x*y
  equal size blocks. All these subtextures are of size 2^n, and are
  typically quite small (256x256 or smaller).  Each triangle is
  clipped, based on the texture coordinates, into several smaller
  triangles. The triangles will then be guaranteed to use only one
  subtexture. Then I project the triangles onto the screen, and
  calculate the maximum projected size for the
  subtextures. Subtextures outside the viewport will be culled. Each
  subtexture is then sampled down to a 2^n value close to the
  projected size, and a GL texture is created with this size. This GL
  texture is used when rendering triangles that are clipped into that
  subtexture.

  Mipmapping is disabled for SoGLBigImage. Aliasing problems shouldn't
  occur because the projected size of the texture is calculated on the
  fly.  When mipmapping is enabled, the amount of texture memory used
  is doubled, and creating the texture object is much slower, so we
  avoid this for SoGLBigImage.
*/

#include <Inventor/misc/SoGLBigImage.h>
#include <Inventor/SbImage.h>
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

#include <Inventor/system/gl.h>
#include <Inventor/errors/SoDebugError.h>

// the number of subtextures that can be changed (resized) each frame.
// By keeping this number small, we avoid slow updates when zooming in
// on an image, as only few textures are changed each frame.
#define CHANGELIMIT 2

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
    glimagediv(NULL),
    glimageage(NULL),
    averagebuf(NULL) {}

  ~SoGLBigImageP() {
    assert(this->glimagearray == NULL);
    delete[] this->tmpbuf;
    delete[] this->averagebuf;
  }

  static SoType classTypeId;

  SbVec2s imagesize;
  SbVec2s glimagesize;
  SbVec2s remain;
  SbVec2s dim;
  unsigned char * tmpbuf;
  int tmpbufsize;
  SoGLImage ** glimagearray;
  int * glimagediv;
  uint32_t * glimageage;
  SbVec2f tcmul;
  int changecnt;
  SbImage myimage;
  unsigned int * averagebuf;

  void copySubImage(const int idx,
                    const unsigned char * src,
                    const SbVec2s & fullsize,
                    const int nc,
                    unsigned char * dst,
                    const int div);

  void copyResizeSubImage(const int idx,
                          const unsigned char * src,
                          const SbVec2s & fullsize,
                          const int nc,
                          unsigned char * dst,
                          const SbVec2s & targetsize);


  void reset(SoState * state = NULL) {
    const int n = this->dim[0] * this->dim[1];
    for (int i = 0; i < n; i++) {
      if (this->glimagearray[i]) {
        this->glimagearray[i]->unref(state);
        this->glimagearray[i] = NULL;
      }
    }
    delete[] this->glimagearray;
    delete[] this->glimageage;
    delete[] this->glimagediv;
    delete[] this->averagebuf;
    this->glimagearray = NULL;
    this->glimageage = NULL;
    this->glimagediv = NULL;
    this->averagebuf = NULL;
    this->imagesize.setValue(0,0);
    this->glimagesize.setValue(0,0);
    this->remain.setValue(0,0);
    this->dim.setValue(0,0);
  }
};

SoType SoGLBigImageP::classTypeId;

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

// Doc in superclass.
SoType
SoGLBigImage::getClassTypeId(void)
{
  if (SoGLBigImageP::classTypeId.isBad()) {
    SoGLBigImageP::classTypeId =
      SoType::createType(SoGLImage::getClassTypeId(), SbName("GLBigImage"));
  }
  return SoGLBigImageP::classTypeId;
}

// Doc in superclass.
SoType
SoGLBigImage::getTypeId(void) const
{
  return SoGLBigImage::getClassTypeId();
}

void
SoGLBigImage::setData(const SbImage * image,
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
  inherited::setData(image, wraps, wrapt, quality, border, NULL);
}

void
SoGLBigImage::setData(const SbImage * image,
                      const Wrap wraps,
                      const Wrap wrapt,
                      const Wrap wrapr,
                      const float quality,
                      const int border,
                      SoState * createinstate)
{
  if (createinstate) {
    SoDebugError::postWarning("SoGLBigImage::setData",
                              "createinstate must be NULL for SoGLBigImage");
  }
  THIS->reset(NULL);
  inherited::setData(image, wraps, wrapt, wrapr, quality, border, NULL);
}


SoGLDisplayList *
SoGLBigImage::getGLDisplayList(SoState * state)
{
  return NULL;
}

// returns the number of bits set, and ets highbit to
// the highest bit set.
static int
bi_cnt_bits(unsigned long val, int & highbit)
{
  int cnt = 0;
  highbit = 0;
  while (val) {
    if (val & 1) cnt++;
    val>>=1;
    highbit++;
  }
  return cnt;
}

// returns the next power of two greater or equal to val
static short
bi_next_power_of_two(short val)
{
  int highbit;
  if (bi_cnt_bits((unsigned long) val, highbit) > 1) {
    return (short) (1<<highbit);
  }
  return val;
}

int
SoGLBigImage::initSubImages(SoState * state,
                            const SbVec2s & subimagesize) const
{
  THIS->changecnt = 0;
  if (subimagesize == THIS->imagesize &&
      THIS->dim[0] > 0) return THIS->dim[0] * THIS->dim[1];

  THIS->reset(state);

  THIS->imagesize = subimagesize;
  THIS->glimagesize[0] = bi_next_power_of_two(THIS->imagesize[0]);
  THIS->glimagesize[1] = bi_next_power_of_two(THIS->imagesize[1]);

  if (THIS->glimagesize[0] > THIS->imagesize[0] && THIS->glimagesize[0] >= 256) {
    int diff = THIS->imagesize[0] - (THIS->glimagesize[0]>>1);
    float ratio = float(diff) / float(THIS->glimagesize[0]>>1);
    if (ratio < 0.3) THIS->glimagesize[0] >>= 1;
  }

  if (THIS->glimagesize[1] > THIS->imagesize[1] && THIS->glimagesize[1] >= 256) {
    int diff = THIS->imagesize[1] - (THIS->glimagesize[1]>>1);
    float ratio = float(diff) / float(THIS->glimagesize[1]>>1);
    if (ratio < 0.3) THIS->glimagesize[1] >>= 1;
  }

  SbVec2s size(0,0);
  int nc;

  const unsigned char * bytes = this->getImage() ?
    this->getImage()->getValue(size, nc) : NULL;

  int numbytes = subimagesize[0] * subimagesize[1] * nc;
  THIS->averagebuf = 
    new unsigned int[numbytes ? numbytes : 1];

  THIS->dim[0] = size[0] / subimagesize[0];
  THIS->dim[1] = size[1] / subimagesize[1];

  THIS->remain[0] = size[0] % subimagesize[0];
  if (THIS->remain[0]) THIS->dim[0] += 1;
  THIS->remain[1] = size[1] % subimagesize[1];
  if (THIS->remain[1]) THIS->dim[1] += 1;

  const int numimages = THIS->dim[0] * THIS->dim[1];

  THIS->glimagediv = new int[numimages];
  THIS->glimagearray = new SoGLImage*[numimages];
  THIS->glimageage = new uint32_t[numimages];
  for (int i = 0; i < numimages; i++) {
    THIS->glimagearray[i] = NULL;
    THIS->glimagediv[i] = 1;
    THIS->glimageage[i] = 0;
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

  if (THIS->glimagearray[idx] == NULL ||
      (THIS->glimagediv[idx] != div && THIS->changecnt++ < CHANGELIMIT)) {
    if (THIS->glimagearray[idx] == NULL) {
      THIS->glimagearray[idx] = new SoGLImage();
    }
    THIS->glimagediv[idx] = div;

    uint32_t flags = this->getFlags();
    flags |= NO_MIPMAP;

    if (flags & USE_QUALITY_VALUE) {
      flags &= ~USE_QUALITY_VALUE;
      if (quality >= 0.5f) {
        flags |= LINEAR_MIN_FILTER|LINEAR_MAG_FILTER;
      }
    }
    THIS->glimagearray[idx]->setFlags(flags);

    SbVec2s actualsize(THIS->glimagesize[0]/div,
                       THIS->glimagesize[1]/div);
    SbVec2s size;
    int numcomponents;
    unsigned char * bytes = this->getImage() ?
      this->getImage()->getValue(size, numcomponents) : NULL;
    if (bytes) {
      int numbytes = actualsize[0]*actualsize[1]*numcomponents;
      if (numbytes > THIS->tmpbufsize) {
        delete[] THIS->tmpbuf;
        THIS->tmpbuf = new unsigned char[numbytes];
        THIS->tmpbufsize = numbytes;
      }

      if (THIS->glimagesize == THIS->imagesize) {
        THIS->copySubImage(idx,
                           bytes,
                           size,
                           numcomponents,
                           THIS->tmpbuf, div);
      }
      else {
        THIS->copyResizeSubImage(idx,
                                 bytes,
                                 size,
                                 numcomponents,
                                 THIS->tmpbuf,
                                 actualsize);
      }
      THIS->myimage.setValuePtr(actualsize, numcomponents, THIS->tmpbuf);
    }
    else THIS->myimage.setValuePtr(SbVec2s(0,0), 0, NULL);
    THIS->glimagearray[idx]->setData(&THIS->myimage,
                                     SoGLImage::CLAMP_TO_EDGE,
                                     SoGLImage::CLAMP_TO_EDGE,
                                     quality,
                                     0, state);
  }

  SoGLDisplayList * dl = THIS->glimagearray[idx]->getGLDisplayList(state);
  assert(dl);
  THIS->glimageage[idx] = 0;
  SoGLImage::tagImage(state, THIS->glimagearray[idx]);
  dl->call(state);
}

// Documented in superclass. Overridden to handle age on subimages.
void
SoGLBigImage::unrefOldDL(SoState * state, const uint32_t maxage)
{
  const int numimages = THIS->dim[0] * THIS->dim[1];
  for (int i = 0; i < numimages; i++) {
    if (THIS->glimagearray[i]) {
      if (THIS->glimageage[i] >= maxage) {
#if COIN_DEBUG && 0 // debug
        SoDebugError::postInfo("SoGLBigImage::unrefOldDL",
                               "Killed image because of old age.");
#endif // debug
        THIS->glimagearray[i]->unref(state);
        THIS->glimagearray[i] = NULL;
      }
      else THIS->glimageage[i] += 1;
    }
  }
  this->incAge();
}

#undef THIS

#ifndef DOXYGEN_SKIP_THIS

//  The method copySubImage() handles the downsampling, and it's very
//  primitive. If the GL texture should be half the size of the
//  original subtexture, every second pixel is simply used.
// 
//  FIXME: I guess this may cause "patterns" in the textures, which
//  has been observed. For a better result, the downsampling method
//  should be improved by setting each pixel to the average of the
//  pixels being downscaled. 20011219 pederb.
void
SoGLBigImageP::copySubImage(const int idx,
                            const unsigned char * src,
                            const SbVec2s & fsize,
                            const int nc,
                            unsigned char * dst,
                            const int div)
{

  if (div == 1) {
    SbVec2s pos(idx % this->dim[0], idx / this->dim[0]);
    
    // FIXME: investigate if it's possible to set the pixel transfer
    // mode so that we don't have to copy the data into a temporary
    // image. This is probably fast enough though.
    
    int origin[2];
    origin[0] = pos[0] * this->imagesize[0];
    origin[1] = pos[1] * this->imagesize[1];
    
    int fullsize[2];
    fullsize[0] = fsize[0];
    fullsize[1] = fsize[1];

    const int w = this->imagesize[0];
    const int h = this->imagesize[1];
    
    for (int y = 0; y < h; y += div) {
      int tmpyadd = fullsize[0] * (origin[1]+y);
      for (int x = 0; x < w; x += div) {
        if ((origin[0] + x) < fullsize[0] && (origin[1] + y) < fullsize[1]) {
          const unsigned char * srcptr =
            src + nc * (tmpyadd + origin[0]+x);
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
  else {
    SbVec2s pos(idx % this->dim[0], idx / this->dim[0]);
    
    int origin[2];
    origin[0] = pos[0] * this->imagesize[0];
    origin[1] = pos[1] * this->imagesize[1];
    
    int fullsize[2];
    fullsize[0] = fsize[0];
    fullsize[1] = fsize[1];
    
    int w = this->imagesize[0];
    int h = this->imagesize[1];
        
    unsigned int mask = (unsigned int) div-1;

    if ((origin[0] + w) > fullsize[0]) {
      w = fullsize[0] - origin[0];
      if (w & mask) {
        w = w - (w & mask);
      }
    }
    if ((origin[1] + h) > fullsize[1]) {
      h = fullsize[1] - origin[1];
      if (h & mask) {
        h = h - (h & mask);
      }
    }

    memset(this->averagebuf, 0, w*h*nc*sizeof(int)/div);
    unsigned int * aptr = this->averagebuf;
    int y;
    int ax, ay;
    for (y = 0; y < h; y++) {
      unsigned int * tmpaptr = aptr;
      const unsigned char * srcptr = 
        src + (fullsize[0] * (origin[1]+y) + origin[0]) * nc;
      for (int x = 0; x < w; x++) { 
        for (int c = 0; c < nc; c++) {
          aptr[c] += srcptr[c];
        }
        srcptr += nc;
        if (!((x+1) & mask)) aptr += nc;
      }
      if ((y+1) & mask) aptr = tmpaptr;
    }
    
    aptr = this->averagebuf;    
    int mydiv = div * div;

    int lineadd = this->imagesize[0] - w;
    
    lineadd /= div;
    w /= div;
    h /= div;

    for (y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
        for (int c = 0; c < nc; c++) {
          dst[c] = (unsigned char) (aptr[c] / mydiv);
        }
        dst += nc;
        aptr += nc;
      }
      dst += lineadd*nc;
    } 
  }
}

void
SoGLBigImageP::copyResizeSubImage(const int idx,
                                  const unsigned char * src,
                                  const SbVec2s & fullsize,
                                  const int nc,
                                  unsigned char * dst,
                                  const SbVec2s & targetsize)
{
  SbVec2s pos(idx % this->dim[0], idx / this->dim[0]);

  SbVec2s origin;
  origin[0] = pos[0] * this->imagesize[0];
  origin[1] = pos[1] * this->imagesize[1];

  int incy = ((this->imagesize[1]<<8) / targetsize[1]);
  int incx = ((this->imagesize[0]<<8) / targetsize[0]);

  const int w = targetsize[0];
  const int h = targetsize[1];

  int addy = 0;

  for (int y = 0; y < h; y++) {
    int addx = 0;
    int tmpaddy = ((addy>>8)+origin[1])*fullsize[0]*nc;
    for (int x  = 0; x < w; x++) {
      const unsigned char * ptr = src + tmpaddy + ((addx>>8)+origin[0]) * nc;
      for (int c = 0; c < nc; c++) {
        *dst++ = *ptr++;
      }
      addx += incx;
    }
    addy += incy;
  }
}



#endif // DOXYGEN_SKIP_THIS
