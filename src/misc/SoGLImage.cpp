/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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
  \class SoGLImage include/Inventor/misc/SoGLImage.h
  \brief The SoGLImage class is used to handle OpenGL textures.
*/

#include <Inventor/misc/SoGLImage.h>
#include <Inventor/misc/SoGL.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif // _WIN32
#include <GL/gl.h>
#include <GL/glu.h>
#include <Inventor/lists/SbList.h>

// if textureQuality is equal or greater than this, use linear filtering
#define LINEAR_LIMIT 0.2f
// if textureQuality is equal or greater than this, create mipmap
#define MIPMAP_LIMIT 0.5f
// if textureQulaity is equal or greater than this, always scale up
#define QUALITY_SCALELIMIT 0.7f


#define FLAG_CLAMPS               0x01
#define FLAG_CLAMPT               0x02
#define FLAG_TRANSPARENCY         0x04
#define FLAG_ALPHATEST            0x08
#define FLAG_INVALIDHANDLE        0x10
#define FLAG_NEEDTRANSPARENCYTEST 0x20

/*!
  Constructor.
*/
SoGLImage::SoGLImage(void)
  : bytes(NULL),
    size(0,0),
    numcomponents(0),
    flags(0),
    context(NULL),
    handle(0),
    quality(0.0f)
{
}

/*!
  Sets the data for this GL image. Should only be called
  when one of the parameters have changed, since this
  will cause the GL texture object to be recreated.
*/
void
SoGLImage::setData(const unsigned char * bytes,
                   const SbVec2s size,
                   const int nc,
                   const SbBool clamps,
                   const SbBool clampt,
                   const float quality,
                   void * context)
{
  this->bytes = bytes;
  this->size = size;
  this->numcomponents = nc;
  this->flags = 0;
  if (clamps) this->flags |= FLAG_CLAMPS;
  if (clampt) this->flags |= FLAG_CLAMPT;
  if (nc == 2 || nc == 4) this->flags |= FLAG_NEEDTRANSPARENCYTEST;
  this->flags |= FLAG_INVALIDHANDLE;
  this->quality = quality;
  this->context = context;
}

/*!
  Destructor.
*/
SoGLImage::~SoGLImage()
{
  if (this->handle) sogl_free_texture(this->handle);
}

/*!
  Returns a pointer to the image data.
*/
const unsigned char *
SoGLImage::getDataPtr(void) const
{
  return this->bytes;
}

/*!
  Returns the size of the texture, in pixels.
*/
SbVec2s
SoGLImage::getSize(void) const
{
  return this->size;
}

/*!
  Returns the number of image components.
*/
int
SoGLImage::getNumComponents(void) const
{
  return this->numcomponents;
}


/*!
  Makes this texture the current OpenGL texture. \a quality
  is the current textureQuality value found from the
  Complexity node.
*/
void
SoGLImage::apply(const float quality)
{
  if (this->handle && (this->flags & FLAG_INVALIDHANDLE)) {
    sogl_free_texture(this->handle);
    this->handle = 0;
    this->flags &= ~FLAG_INVALIDHANDLE;
  }
  if (this->handle == 0) {
    this->handle = this->createHandle();
    this->flags &= ~FLAG_INVALIDHANDLE;
  }
  sogl_apply_texture(this->handle);
  if (quality < LINEAR_LIMIT) {
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  }
  else if ((quality < MIPMAP_LIMIT) || (this->quality < MIPMAP_LIMIT)) {
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }
  else if (quality < 0.8f) {
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
  }
  else { // max quality
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  }
}

/*!
  Returns \e TRUE if this texture has some pixels with alpha != 255
*/
SbBool
SoGLImage::hasTransparency(void) const
{
  if (this->flags & FLAG_NEEDTRANSPARENCYTEST) {
    ((SoGLImage*)this)->checkTransparency();
  }
  return (this->flags & FLAG_TRANSPARENCY) != 0;
}

/*!
  Returns TRUE if this image has some alpha value != 255, and all
  these values are 0. If this is the case, alpha test can be used
  to render this texture instead of for instance blending, which
  is usually slower and might yield z-buffer artifacts.
*/
SbBool
SoGLImage::needAlphaTest(void) const
{
  if (this->flags & FLAG_NEEDTRANSPARENCYTEST) {
    ((SoGLImage*)this)->checkTransparency();
  }
  return (this->flags & FLAG_ALPHATEST) != 0;
}

/*!
  Returns \e TRUE if texture coordinates should be clamped in
  the s-direction.
*/
SbBool
SoGLImage::shouldClampS(void) const
{
  return (this->flags & FLAG_CLAMPS) !=0;
}

/*!
  Returns \e TRUE if texture coordinates should be clamped in
  the t-direction.
*/
SbBool
SoGLImage::shouldClampT(void) const
{
  return (this->flags & FLAG_CLAMPT) !=0;
}

/*!
  Returns texture quality for this GL image. For now, if quality >
  0.5 when created, we create mipmaps, otherwise a normal texture is
  created.  Be aware, if you for instance create a texture with
  texture quality 0.4, and then later try to apply the texture with a
  texture quality greater than 0.5, you will not get a mipmap
  texture. If you suspect you might need a mipmap texture, it should
  be created with a texture quality greater than 0.5.
*/
float
SoGLImage::getQuality(void) const
{
  return this->quality;
}

/*!
  Returns TRUE if the GL handle inside this instance is valid.
*/
SbBool
SoGLImage::isValid(void) const
{
  return (this->flags & FLAG_INVALIDHANDLE) == 0;
}

// returns the number of bits set, and ets highbit to
// the highest bit set.
static int
cnt_bits(unsigned long val, int & highbit)
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
static unsigned long
nearest_power_of_two(unsigned long val)
{
  int highbit;
  if (cnt_bits(val, highbit) > 1) {
    return 1<<highbit;
  }
  return val;
}

// static data used to temporarily store image when resizing
static unsigned char * glimage_tmpimagebuffer = NULL;
static int glimage_tmpimagebuffersize = 0;

void cleanup_tmpimage(void)
{
  delete [] glimage_tmpimagebuffer;
}

//
// private method that tests the size of the image, and
// performs an resize if the size is not a power of two.
//
int
SoGLImage::createHandle(void)
{
  int xsize = this->size[0];
  int ysize = this->size[1];
  unsigned int newx = (unsigned int)nearest_power_of_two(xsize);
  unsigned int newy = (unsigned int)nearest_power_of_two(ysize);

  // if >= 256 and low quality, don't scale up unless size is
  // close to an above power of two. This saves a lot of texture memory
  if (this->quality < 0.7f) {
    if (newx >= 256) {
      if ((newx - xsize) > (newx>>3)) newx >>= 1;
    }
    if (newy >= 256) {
      if ((newy - ysize) > (newy>>3)) newy >>= 1;
    }
  }

  // downscale to legal GL size (implementation dependant)
  unsigned long maxsize = sogl_max_texture_size();
  while (newx > maxsize) newx >>= 1;
  while (newy > maxsize) newy >>= 1;

  // these might change if image is resized
  const unsigned char * imageptr = this->bytes;

  if (newx != (unsigned long) xsize || newy != (unsigned long) ysize) {
    int numbytes = newx * newy * this->numcomponents;
    if (numbytes > glimage_tmpimagebuffersize) {
      delete [] glimage_tmpimagebuffer;
      glimage_tmpimagebuffer = new unsigned char[numbytes];
      glimage_tmpimagebuffersize = numbytes;
    }
    GLenum format;
    switch (this->numcomponents) {
    default: // avoid compiler warnings
    case 1: format = GL_LUMINANCE; break;
    case 2: format = GL_LUMINANCE_ALPHA; break;
    case 3: format = GL_RGB; break;
    case 4: format = GL_RGBA; break;
    }

    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_PACK_ROW_LENGTH, 0);
    glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_PACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    
    gluScaleImage(format, xsize, ysize,
                  GL_UNSIGNED_BYTE, (void*) this->bytes,
                  newx, newy, GL_UNSIGNED_BYTE,
                  (void*)glimage_tmpimagebuffer);
    imageptr = glimage_tmpimagebuffer;
  }

  return sogl_create_texture(this->shouldClampS(),
                             this->shouldClampT(),
                             imageptr,
                             this->numcomponents,
                             newx, newy,
                             this->quality >= MIPMAP_LIMIT);
}

// test image data for transparency
void
SoGLImage::checkTransparency(void)
{
  if (this->numcomponents == 2 || this->numcomponents == 4) {
    int n = this->size[0] * this->size[1];
    int nc = this->numcomponents;
    unsigned char * ptr = (unsigned char *) this->bytes + nc - 1;

    while (n) {
      if (*ptr != 255 && *ptr != 0) break;
      if (*ptr == 0) this->flags |= FLAG_ALPHATEST;
      ptr += nc;
      n--;
    }
    if (n > 0) {
      this->flags |= FLAG_TRANSPARENCY;
      this->flags &= ~FLAG_ALPHATEST;
    }
    else this->flags &= ~FLAG_TRANSPARENCY;
  }
  else this->flags &= ~(FLAG_TRANSPARENCY|FLAG_ALPHATEST);

  // clear test flag before returning
  this->flags &= ~FLAG_NEEDTRANSPARENCYTEST;
}
