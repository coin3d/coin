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
  subtexture. Then the triangles are projected onto the screen, and
  the maximum projected size for each subtexture is
  calculated. Subtextures outside the viewport will be culled. Each
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

#ifdef COIN_THREADSAFE
#include <Inventor/threads/SbMutex.h>
#include <Inventor/C/threads/storage.h>
#endif // COIN_THREADSAFE

#include <Inventor/system/gl.h>
#include <Inventor/errors/SoDebugError.h>

// the number of subtextures that can be changed (resized) each frame.
// By keeping this number small, we avoid slow updates when zooming in
// on an image, as only few textures are changed each frame.
#define CHANGELIMIT 4

#ifndef DOXYGEN_SKIP_THIS

typedef struct {
  SbVec2s imagesize;
  SbVec2s glimagesize;
  SbVec2s remain;
  SbVec2f tcmul;
  SbVec2s dim;
  SbVec2s currentdim;

  unsigned char * tmpbuf;
  int tmpbufsize;
  SoGLImage ** glimagearray;
  int * glimagediv;
  uint32_t * glimageage;
  int changecnt;
  SbImage * myimage;
  unsigned int * averagebuf;
} SoGLBigImageTls;

class SoGLBigImageP {
public:
  SoGLBigImageP(void);
  ~SoGLBigImageP();

  static SoType classTypeId;

#ifdef COIN_THREADSAFE
  cc_storage * storage;
  SbMutex mutex;
#else // COIN_THREADSAFE
  SoGLBigImageTls storagedata;
#endif // !COIN_THREADSAFE
  unsigned char ** cache;
  SbVec2s * cachesize;
  int numcachelevels;

  // inline for speed
  inline SoGLBigImageTls * getTls(void) {
#ifdef COIN_THREADSAFE
    return (SoGLBigImageTls*) cc_storage_get(this->storage);
#else
    return &this->storagedata;
#endif // !COIN_THREADSAFE
  }

  inline void lock(void) {
#ifdef COIN_THREADSAFE
    this->mutex.lock();
#endif // COIN_THREADSAFE
  }
  inline void unlock(void) {
#ifdef COIN_THREADSAFE
    this->mutex.unlock();
#endif // COIN_THREADSAFE
  }

  void copySubImage(SoGLBigImageTls * tls,
                    const int idx,
                    const unsigned char * src,
                    const SbVec2s & fullsize,
                    const int nc,
                    unsigned char * dst,
                    const int div,
                    const int level);
  void copyResizeSubImage(SoGLBigImageTls * tls,
                          const int idx,
                          const unsigned char * src,
                          const SbVec2s & fullsize,
                          const int nc,
                          unsigned char * dst,
                          const SbVec2s & targetsize);
  void resetAllTls(SoState * state);
  void resetCache(void);
  static void reset(SoGLBigImageTls * tls, SoState * state = NULL);
  static void unrefOldDL(SoGLBigImageTls * tls, SoState * state, const uint32_t maxage);
  void createCache(const unsigned char * bytes, const SbVec2s size, const int nc);
};

SoType SoGLBigImageP::classTypeId;

static void
soglbigimagetls_construct(void * closure)
{
  SoGLBigImageTls * storage = (SoGLBigImageTls*) closure;
  storage->imagesize.setValue(0, 0);
  storage->remain.setValue(0, 0);
  storage->dim.setValue(0, 0);
  storage->currentdim.setValue(0, 0);
  storage->tmpbuf = NULL;
  storage->tmpbufsize = 0;
  storage->glimagearray = NULL;
  storage->glimagediv = NULL;
  storage->glimageage = NULL;
  storage->averagebuf = NULL;
  storage->myimage = new SbImage;
}

static void
soglbigimagetls_destruct(void * closure)
{
  SoGLBigImageTls * tls = (SoGLBigImageTls*) closure;
  SoGLBigImageP::reset(tls, NULL);

  // these are not destructed in reset()
  delete tls->myimage;
  delete[] tls->tmpbuf;
  delete[] tls->averagebuf;
}

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
  THIS->resetCache();
  delete THIS;
}

void
SoGLBigImage::unref(SoState * state)
{
  THIS->resetAllTls(state);
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
  delete THIS;
  THIS = new SoGLBigImageP;
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
  delete THIS;
  THIS = new SoGLBigImageP;
  inherited::setData(image, wraps, wrapt, wrapr, quality, border, NULL);
}


SoGLDisplayList *
SoGLBigImage::getGLDisplayList(SoState * state)
{
  return NULL;
}

// returns the number of bits set, and sets highbit to
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
SoGLBigImage::initSubImages(const SbVec2s & subimagesize) const
{
  SoGLBigImageTls * tls = THIS->getTls();

  tls->changecnt = 0;
  if (subimagesize == tls->imagesize &&
      tls->dim[0] > 0) return tls->dim[0] * tls->dim[1];

  tls->imagesize = subimagesize;
  tls->glimagesize[0] = bi_next_power_of_two(tls->imagesize[0]);
  tls->glimagesize[1] = bi_next_power_of_two(tls->imagesize[1]);

  if (tls->glimagesize[0] > tls->imagesize[0] && tls->glimagesize[0] >= 256) {
    int diff = tls->imagesize[0] - (tls->glimagesize[0]>>1);
    float ratio = float(diff) / float(tls->glimagesize[0]>>1);
    if (ratio < 0.3) tls->glimagesize[0] >>= 1;
  }

  if (tls->glimagesize[1] > tls->imagesize[1] && tls->glimagesize[1] >= 256) {
    int diff = tls->imagesize[1] - (tls->glimagesize[1]>>1);
    float ratio = float(diff) / float(tls->glimagesize[1]>>1);
    if (ratio < 0.3) tls->glimagesize[1] >>= 1;
  }

  SbVec2s size(0,0);
  int nc;

  const unsigned char * bytes = this->getImage() ?
    this->getImage()->getValue(size, nc) : NULL;

  tls->dim[0] = size[0] / subimagesize[0];
  tls->dim[1] = size[1] / subimagesize[1];

  tls->remain[0] = size[0] % subimagesize[0];
  if (tls->remain[0]) tls->dim[0] += 1;
  tls->remain[1] = size[1] % subimagesize[1];
  if (tls->remain[1]) tls->dim[1] += 1;

  tls->tcmul[0] = float(tls->dim[0] * subimagesize[0]) / float(size[0]);
  tls->tcmul[1] = float(tls->dim[1] * subimagesize[1]) / float(size[1]);
  return tls->dim[0] * tls->dim[1];
}

void
SoGLBigImage::handleSubImage(const int idx,
                             SbVec2f & start,
                             SbVec2f & end,
                             SbVec2f & tcmul)
{
  SoGLBigImageTls * tls = THIS->getTls();

  SbVec2s pos(idx % tls->dim[0], idx / tls->dim[0]);
  start[0] = float(pos[0]) / float(tls->dim[0]);
  start[1] = float(pos[1]) / float(tls->dim[1]);
  end[0] = float(pos[0]+1) / float(tls->dim[0]);
  end[1] = float(pos[1]+1) / float(tls->dim[1]);

  start[0] *= tls->tcmul[0];
  start[1] *= tls->tcmul[1];
  end[0] *= tls->tcmul[0];
  end[1] *= tls->tcmul[1];
  tcmul = tls->tcmul;
}

void
SoGLBigImage::applySubImage(SoState * state, const int idx,
                            const float quality,
                            const SbVec2s & projsize)
{
  SbVec2s size;
  int numcomponents;
  unsigned char * bytes = this->getImage() ?
    this->getImage()->getValue(size, numcomponents) : NULL;

  SoGLBigImageTls * tls = THIS->getTls();

  if (tls->currentdim != tls->dim) {
    SoGLBigImageP::reset(tls, state);
    tls->currentdim = tls->dim;
    const int numimages = tls->dim[0] * tls->dim[1];

    tls->glimagediv = new int[numimages];
    tls->glimagearray = new SoGLImage*[numimages];
    tls->glimageage = new uint32_t[numimages];
    for (int i = 0; i < numimages; i++) {
      tls->glimagearray[i] = NULL;
      tls->glimagediv[i] = 1;
      tls->glimageage[i] = 0;
    }

    int numbytes = tls->imagesize[0] * tls->imagesize[1] * numcomponents;
    tls->averagebuf =
      new unsigned int[numbytes ? numbytes : 1];

    // lock before testing/creating cache to avoid race conditions
    THIS->lock();
    if (THIS->cache == NULL) {
      THIS->createCache(bytes, size, numcomponents);
    }
    THIS->unlock();
  }

  int level = 0;
  int div = 2;
  while ((tls->imagesize[0]/div > projsize[0]) &&
         (tls->imagesize[1]/div > projsize[1])) {
    div <<= 1;
    level++;
  }
  div >>= 1;

  if (tls->glimagearray[idx] == NULL ||
      (tls->glimagediv[idx] != div && tls->changecnt < CHANGELIMIT)) {

    if (tls->glimagearray[idx] == NULL) {
      tls->glimagearray[idx] = new SoGLImage();
    }
    else {
      tls->changecnt++;
    }
    tls->glimagediv[idx] = div;

    uint32_t flags = this->getFlags();
    flags |= NO_MIPMAP;

    if (flags & USE_QUALITY_VALUE) {
      flags &= ~USE_QUALITY_VALUE;
      if (quality >= 0.5f) {
        flags |= LINEAR_MIN_FILTER|LINEAR_MAG_FILTER;
      }
    }
    tls->glimagearray[idx]->setFlags(flags);

    SbVec2s actualsize(tls->glimagesize[0]/div,
                       tls->glimagesize[1]/div);
    if (bytes) {
      int numbytes = actualsize[0]*actualsize[1]*numcomponents;
      if (numbytes > tls->tmpbufsize) {
        delete[] tls->tmpbuf;
        tls->tmpbuf = new unsigned char[numbytes];
        tls->tmpbufsize = numbytes;
      }

      if (tls->glimagesize == tls->imagesize) {
        THIS->copySubImage(tls,
                           idx,
                           bytes,
                           size,
                           numcomponents,
                           tls->tmpbuf, div, level);
      }
      else {
        THIS->copyResizeSubImage(tls,
                                 idx,
                                 bytes,
                                 size,
                                 numcomponents,
                                 tls->tmpbuf,
                                 actualsize);
      }
      tls->myimage->setValuePtr(actualsize, numcomponents, tls->tmpbuf);
    }
    else tls->myimage->setValuePtr(SbVec2s(0,0), 0, NULL);
    tls->glimagearray[idx]->setData(tls->myimage,
                                    SoGLImage::CLAMP_TO_EDGE,
                                    SoGLImage::CLAMP_TO_EDGE,
                                    quality,
                                    0, state);
  }

  SoGLDisplayList * dl = tls->glimagearray[idx]->getGLDisplayList(state);
  assert(dl);
  tls->glimageage[idx] = 0;
  SoGLImage::tagImage(state, tls->glimagearray[idx]);
  this->resetAge();
  dl->call(state);
}

/*!
  To avoid doing too much work in one frame, there is a limit on the
  number of subtextures that can be changed each frame. If this limit
  is exceeded, this function will return TRUE, otherwise FALSE.
*/
SbBool
SoGLBigImage::exceededChangeLimit(void)
{
  return THIS->getTls()->changecnt >= CHANGELIMIT;
}

// needed for cc_storage_apply_to_all() callback
typedef struct {
  uint32_t maxage;
  SoState * state;
} soglbigimage_unrefolddl_data;

// cc_storage_apply_to_all() callback
static void
soglbigimage_unrefolddl_cb(void * tls, void * closure)
{
  soglbigimage_unrefolddl_data * data =
    (soglbigimage_unrefolddl_data *) closure;

  SoGLBigImageP::unrefOldDL((SoGLBigImageTls*)tls, data->state, data->maxage);
}

// Documented in superclass. Overridden to handle age on subimages.
void
SoGLBigImage::unrefOldDL(SoState * state, const uint32_t maxage)
{
#ifdef COIN_THREADSAFE
  soglbigimage_unrefolddl_data data;
  data.maxage = maxage;
  data.state = state;
  cc_storage_apply_to_all(THIS->storage, soglbigimage_unrefolddl_cb, &data);
#else // COIN_THREADSAFE
  SoGLBigImageP::unrefOldDL(&THIS->storagedata, state, maxage);
#endif // ! COIN_THREADSAFE
  this->incAge();
}

#undef THIS

#ifndef DOXYGEN_SKIP_THIS

SoGLBigImageP::SoGLBigImageP(void) :
  cache(NULL),
  cachesize(NULL),
  numcachelevels(0)
{
#ifdef COIN_THREADSAFE
  this->storage = cc_storage_construct_etc(sizeof(SoGLBigImageTls),
                                           soglbigimagetls_construct,
                                           soglbigimagetls_destruct);
#else // COIN_THREADSAFE
  soglbigimagetls_construct(&this->storagedata);
#endif // !COIN_THREADSAFE
}

SoGLBigImageP::~SoGLBigImageP()
{
  this->resetCache();
#ifdef COIN_THREADSAFE
  cc_storage_destruct(this->storage);
#else // COIN_THREADSAFE
  soglbigimagetls_destruct(&this->storagedata);
#endif // !COIN_THREADSAFE
}

//  The method copySubImage() handles the downsampling. It averages
//  the full-resolution pixels to create the low resolution image.
void
SoGLBigImageP::copySubImage(SoGLBigImageTls * tls,
                            const int idx,
                            const unsigned char * src,
                            const SbVec2s & fsize,
                            const int nc,
                            unsigned char * dst,
                            const int div,
                            const int level)
{
  if ((div == 1) || (this->cache && level < this->numcachelevels && this->cache[level])) {
    SbVec2s pos(idx % tls->dim[0], idx / tls->dim[0]);

    // FIXME: investigate if it's possible to set the pixel transfer
    // mode so that we don't have to copy the data into a temporary
    // image. This is probably fast enough though.

    int origin[2];
    int fullsize[2];
    int w, h;
    const unsigned char * datasrc;

    if (div == 1) { // use original image
      origin[0] = pos[0] * tls->imagesize[0];
      origin[1] = pos[1] * tls->imagesize[1];

      fullsize[0] = fsize[0];
      fullsize[1] = fsize[1];
      w = tls->imagesize[0];
      h = tls->imagesize[1];
      datasrc = src;
    }
    else { // use cache image
      origin[0] = pos[0] * (tls->imagesize[0] >> level);
      origin[1] = pos[1] * (tls->imagesize[1] >> level);
      fullsize[0] = this->cachesize[level][0];
      fullsize[1] = this->cachesize[level][1];
      w = tls->imagesize[0] >> level;
      h = tls->imagesize[1] >> level;
      datasrc = this->cache[level];
    }

    // check for fast loop (common case)
    if ((origin[0] + w) < fullsize[0] && (origin[1] + h) < fullsize[1]) {
      for (int y = 0; y < h; y++) {
        int tmpyadd = fullsize[0] * (origin[1]+y);
        for (int x = 0; x < w; x++) {
          const unsigned char * srcptr =
            datasrc + nc * (tmpyadd + origin[0]+x);
          for (int c = 0; c < nc; c++) {
            *dst++ = srcptr[c];
          }
        }
      }
    }
    else { // slower loop (x and y values are clamped)
      for (int y = 0; y < h; y++) {
        int tmpyadd = fullsize[0] * SbClamp(origin[1]+y, 0, fullsize[1]-1);
        for (int x = 0; x < w; x++) {
          const unsigned char * srcptr =
            datasrc + nc * (tmpyadd + SbClamp(origin[0]+x, 0, fullsize[0]-1));
          for (int c = 0; c < nc; c++) {
            *dst++ = srcptr[c];
          }
        }
      }
    }
  }
  else {
    SbVec2s pos(idx % tls->dim[0], idx / tls->dim[0]);

    int origin[2];
    origin[0] = pos[0] * tls->imagesize[0];
    origin[1] = pos[1] * tls->imagesize[1];

    int fullsize[2];
    fullsize[0] = fsize[0];
    fullsize[1] = fsize[1];

    int w = tls->imagesize[0];
    int h = tls->imagesize[1];

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

    memset(tls->averagebuf, 0, w*h*nc*sizeof(int)/div);
    unsigned int * aptr = tls->averagebuf;
    int y;
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

    aptr = tls->averagebuf;
    int mydiv = div * div;

    int lineadd = tls->imagesize[0] - w;

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
SoGLBigImageP::copyResizeSubImage(SoGLBigImageTls * tls,
                                  const int idx,
                                  const unsigned char * src,
                                  const SbVec2s & fullsize,
                                  const int nc,
                                  unsigned char * dst,
                                  const SbVec2s & targetsize)
{
  SbVec2s pos(idx % tls->dim[0], idx / tls->dim[0]);

  SbVec2s origin;
  origin[0] = pos[0] * tls->imagesize[0];
  origin[1] = pos[1] * tls->imagesize[1];

  int incy = ((tls->imagesize[1]<<8) / targetsize[1]);
  int incx = ((tls->imagesize[0]<<8) / targetsize[0]);

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

// create a lower resolution image by averaging all pixels in a block
// (from the full resolution image) into a new pixel. This is pretty
// slow, but yields a higher quality result compared to when each
// level is calculated based on the previous level.
static  unsigned char *
image_downsample(const unsigned char * bytes, const SbVec2s fullsize,
                 const int nc, const SbVec2s subsize, const int div)
{
  unsigned char * dst = new unsigned char[subsize[0]*subsize[1]*nc];
  unsigned char * dstptr = dst;

  int starty = 0;
  int stopy = div;
  for (int y = 0; y < subsize[1]; y++) {
    assert(starty < fullsize[1]);

    int startx = 0;
    int stopx = div;

    for (int x = 0; x < subsize[0]; x++) {
      assert(startx < fullsize[0]);

      int avg[4] = {0};
      int numavg = 0;

      for (int y2 = starty; y2 < stopy; y2++) {
        for (int x2 = startx; x2 < stopx; x2++) {
          const unsigned char * src = bytes + (fullsize[0]*y2 + x2) * nc;
          for (int c = 0; c < nc; c++) {
            avg[c] += src[c];
          }
          numavg++;
        }
      }
      assert(numavg > 0);
      for (int c = 0; c < nc; c++) {
        *dstptr++ = avg[c] / numavg;
      }
      startx += div;
      if (startx >= fullsize[0]) startx = fullsize[0] - 1;
      stopx += div;
      if (stopx > fullsize[0]) stopx = fullsize[0];
    }
    starty += div;
    if (starty >= fullsize[1]) starty = fullsize[1]-1;
    stopy += div;
    if (stopy > fullsize[1]) stopy = fullsize[1];
  }

  return dst;
}

// create a lower resolution image by averaging four and four pixels
// into a new pixel. This is the same technique as the one usually
// used when creating OpenGL mipmaps. Each level is calculated based
// on the previous level, not on the full-resolution image.
static void
image_downsample_fast(const int width, const int height, const int nc,
                      const unsigned char * datain, unsigned char * dataout)
{
  assert(width > 1 || height > 1);

  int nextrow = width * nc;
  if (width & 1) nextrow += nc; // in case original image has odd size
  int newwidth = width >> 1;
  int newheight = height >> 1;
  unsigned char * dst = dataout;
  const unsigned char * src = datain;

  // check for 1D images
  if (width == 1 || height == 1) {
    int n = SbMax(newwidth, newheight);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < nc; j++) {
        *dst = (src[0] + src[nc]) >> 1;
        dst++; src++;
      }
      src += nc; // skip to next pixel
    }
  }
  else {
    for (int i = 0; i < newheight; i++) {
      for (int j = 0; j < newwidth; j++) {
        for (int c = 0; c < nc; c++) {
          *dst = (src[0] + src[nc] + src[nextrow] + src[nextrow+nc] + 2) >> 2;
          dst++; src++;
        }
        src += nc; // skip to next pixel
      }
      src += nextrow;
    }
  }
}

void
SoGLBigImageP::createCache(const unsigned char * bytes, const SbVec2s size, const int nc)
{
  int levels = 0;

  while (((size[0]>>levels) > 0) || ((size[1]>>levels) > 0)) {
    levels++;
  }
  if (levels == 0) return;
  this->numcachelevels = levels;

  this->cache = new unsigned char*[levels];
  this->cachesize = new SbVec2s[levels];
  // temporarily set first cache to simplify code below
  this->cache[0] = (unsigned char*) bytes;
  this->cachesize[0] = size;

  for (int l = 1; l < levels; l++) {
#if 0 // high-quality downsample is too slow, currently disabled
    int sx = size[0] >> l;
    if (sx == 0) sx = 1;
    int sy = size[1] >> l;
    if (sy == 0) sy = 1;

    this->cachesize[l] = SbVec2s((short)sx, (short)sy);
    this->cache[l] = image_downsample(bytes, size, nc, this->cachesize[l], 1<<l);
#else // end of high quality downsample
    short w = size[0]>>l;
    short h = size[1]>>l;
    this->cachesize[l] = SbVec2s(w, h);
    if (w == 0) w = 1;
    if (h == 0) h = 1;
    this->cache[l] = new unsigned char[w*h*nc];
    image_downsample_fast(this->cachesize[l-1][0], this->cachesize[l-1][1], nc,
                          this->cache[l-1], this->cache[l]);
#endif // end of low quality downsample
  }
  this->cache[0] = NULL;
  this->cachesize[0] = SbVec2s(0, 0);
}

void
SoGLBigImageP::resetCache(void)
{
  for (int i = 0; i < this->numcachelevels; i++) {
    delete[] this->cache[i];
  }
  delete[] this->cache;
  delete[] this->cachesize;
  this->cache = NULL;
  this->cachesize = NULL;
  this->numcachelevels = 0;
}

void
SoGLBigImageP::reset(SoGLBigImageTls * tls, SoState * state)
{
  const int n = tls->currentdim[0] * tls->currentdim[1];
  for (int i = 0; i < n; i++) {
    if (tls->glimagearray[i]) {
      tls->glimagearray[i]->unref(state);
      tls->glimagearray[i] = NULL;
    }
  }
  delete[] tls->glimagearray;
  delete[] tls->glimageage;
  delete[] tls->glimagediv;
  delete[] tls->averagebuf;
  tls->glimagearray = NULL;
  tls->glimageage = NULL;
  tls->glimagediv = NULL;
  tls->averagebuf = NULL;
  tls->currentdim.setValue(0,0);
}

void
SoGLBigImageP::unrefOldDL(SoGLBigImageTls * tls, SoState * state, const uint32_t maxage)
{
  const int numimages = tls->currentdim[0] * tls->currentdim[1];
  for (int i = 0; i < numimages; i++) {
    if (tls->glimagearray[i]) {
      if (tls->glimageage[i] >= maxage) {
#if COIN_DEBUG && 0 // debug
        SoDebugError::postInfo("SoGLBigImageP::unrefOldDL",
                               "Killed image because of old age.");
#endif // debug
        tls->glimagearray[i]->unref(state);
        tls->glimagearray[i] = NULL;
      }
      else tls->glimageage[i] += 1;
    }
  }
}

// cc_storage_apply_to_all callback used by resetAllTls()
static void
soglbigimage_resetall_cb(void * tls, void * closure)
{
  // simply call SoGLBigImageP::reset()
  SoGLBigImageP::reset((SoGLBigImageTls*) tls, (SoState*) closure);
}

void
SoGLBigImageP::resetAllTls(SoState * state)
{
#ifdef COIN_THREADSAFE
  cc_storage_apply_to_all(this->storage, soglbigimage_resetall_cb, state);
#else // COIN_THREADSAFE
  SoGLBigImageP::reset(&this->storagedata, state);
#endif // ! COIN_THREADSAFE
}

#endif // DOXYGEN_SKIP_THIS
