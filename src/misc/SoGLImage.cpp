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

/*!
  \enum SoGLImage::Wrap

  Used to specify how texture coordinates < 0.0 and > 1.0 should be handled.
  It can either be repeated (REPEAT), clamped (CLAMP) or clamped to edge
  (CLAMP_TO_EDGE), which is useful when tiling textures.
*/

#include <Inventor/misc/SoGLImage.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/elements/SoGLTextureImageElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>
#include <GLUWrapper.h>
#include <Inventor/lists/SbList.h>

// if textureQuality is equal or greater than this, use linear filtering
#define LINEAR_LIMIT 0.2f
// if textureQuality is equal or greater than this, create mipmap
#define MIPMAP_LIMIT 0.5f
// if textureQulaity is equal or greater than this, always scale up
#define QUALITY_SCALELIMIT 0.7f


#define FLAG_TRANSPARENCY         0x01
#define FLAG_ALPHATEST            0x02
#define FLAG_NEEDTRANSPARENCYTEST 0x04

/*!
  Constructor.
*/
SoGLImage::SoGLImage(void)
  : bytes(NULL),
    size(0,0),
    numcomponents(0),
    flags(0),
    wraps(CLAMP),
    wrapt(CLAMP),
    border(0)
{
}

/*!
  Sets the data for this GL image. Should only be called when one
  of the parameters have changed, since this will cause the GL texture
  object to be recreated.  Caller is responsible for sending legal
  Wrap values.  CLAMP_TO_EDGE is only supported on OpenGL v1.2
  implementations, and as an extension on some earlier SGI
  implementations (GL_SGIS_texture_edge_clamp).

  For now, if quality > 0.5 when created, we create mipmaps, otherwise
  a regular texture is created.  Be aware, if you for instance create a
  texture with texture quality 0.4, and then later try to apply the
  texture with a texture quality greater than 0.5, you will not get a
  mipmap texture. If you suspect you might need a mipmap texture, it
  should be created with a texture quality bigger than 0.5.

  If \a border != 0, the OpenGL texture will be created with this
  border size. Be aware that this might be extremely slow on most
  PC hardware.

  Normally, the OpenGL texture object isn't created until the first
  time it is needed, but if \a createinstate is != NULL, the texture
  object is created immediately. This is useful if you use a temporary
  buffer to hold the texture data. Be careful when using this feature,
  since the texture data might be needed at a later stage (for
  instance to create a texture object for another context).
  It will not be possible to create texture objects for other cache
  contexts when \a createinstate is != NULL.
*/
void
SoGLImage::setData(const unsigned char * bytes,
                   const SbVec2s size,
                   const int nc,
                   const Wrap wraps,
                   const Wrap wrapt,
                   const float quality,
                   const int border,
                   SoState * createinstate)

{
  this->bytes = bytes;
  this->size = size;
  this->numcomponents = nc;
  this->flags = 0;
  this->wraps = wraps;
  this->wrapt = wrapt;
  if (nc == 2 || nc == 4) this->flags |= FLAG_NEEDTRANSPARENCYTEST;
  this->border = border;
  this->unrefDLists(createinstate);
  if (createinstate) {
    this->dlists.append(this->createGLDisplayList(createinstate, quality));
    this->bytes = NULL; // data is assumed to be temporary
  }
}

/*!
  Destructor.
*/
SoGLImage::~SoGLImage()
{
  this->unrefDLists(NULL);
}

/*!
  This class has a private destuctor since we want users to supply
  the current GL state when deleting the image. This is to make sure
  gl texture objects are freed as soon as possible. If you supply
  NULL to this method, the gl texture objects won't be deleted
  until the next time an GLRenderAaction is applied in the image's
  cache context(s).
*/
void
SoGLImage::unref(SoState * state)
{
  this->unrefDLists(state);
  delete this;
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
  Returns or creates a SoGLDisplayList to be used for rendering.
  \a quality should contain the desired quality of the texture,
  as found in the SoComplexity::textureQuality. Returns
  NULL if no SoDLDisplayList could be created.
*/
SoGLDisplayList *
SoGLImage::getGLDisplayList(SoState * state, const float quality)
{
  int currcontext = SoGLCacheContextElement::get(state);
  int i, n = this->dlists.getLength();
  SoGLDisplayList * dl = NULL;
  for (i = 0; i < n; i++) {
    dl = this->dlists[i];
    if (dl->getContext() == currcontext) break;
  }
  if (i == n) {
    dl = this->createGLDisplayList(state, quality);
    if (dl) this->dlists.append(dl);
  }
  return dl;
}

/*!
  Conveniece method which makes \a dl the current texture for
  the current context, and sets the filtering values based on
  \a quality. It will not enable mipmap filtering unless the
  the texture object contains mipmap data.

  \sa setData()
*/
void
SoGLImage::apply(SoState * state, SoGLDisplayList * dl, const float quality)
{
  SbBool ismipmap = dl->isMipMapTextureObject();
  dl->call(state);
  if (quality < LINEAR_LIMIT) {
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  }
  else if ((quality < MIPMAP_LIMIT) || !ismipmap) {
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
  Returns the wrap strategy for the S (horizontal) direction.
*/
SoGLImage::Wrap
SoGLImage::getWrapS(void) const
{
  return this->wraps;
}

/*!
  Returns the wrap strategy for the T (vertical) direction.
*/
SoGLImage::Wrap
SoGLImage::getWrapT(void) const
{
  return this->wrapt;
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
SoGLDisplayList *
SoGLImage::createGLDisplayList(SoState * state, const float quality)
{
  if (!this->bytes) return NULL;

  int xsize = this->size[0];
  int ysize = this->size[1];

  // these might change if image is resized
  const unsigned char * imageptr = this->bytes;
  unsigned int newx = (unsigned int)nearest_power_of_two(xsize-2*this->border);
  unsigned int newy = (unsigned int)nearest_power_of_two(ysize-2*this->border);

  // if >= 256 and low quality, don't scale up unless size is
  // close to an above power of two. This saves a lot of texture memory
  if (quality < 0.7f) {
    if (newx >= 256) {
      if ((newx - (xsize-2*this->border)) > (newx>>3)) newx >>= 1;
    }
    if (newy >= 256) {
      if ((newy - (ysize-2*this->border)) > (newy>>3)) newy >>= 1;
    }
  }

  // downscale to legal GL size (implementation dependant)
  unsigned long maxsize = SoGLTextureImageElement::getMaxGLTextureSize();
  while (newx > maxsize) newx >>= 1;
  while (newy > maxsize) newy >>= 1;

  newx += 2*this->border;
  newy += 2*this->border;


  if ((newx != (unsigned long) xsize) || (newy != (unsigned long) ysize)) {
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

    // FIXME: ignoring the error code. Silly. 20000929 mortene.
    (void)GLUWrapper()->gluScaleImage(format, xsize, ysize,
                                      GL_UNSIGNED_BYTE, (void*) this->bytes,
                                      newx, newy, GL_UNSIGNED_BYTE,
                                      (void*)glimage_tmpimagebuffer);
    imageptr = glimage_tmpimagebuffer;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
  }

  SoGLDisplayList * dl =
    new SoGLDisplayList(state,
                        SoGLDisplayList::TEXTURE_OBJECT,
                        1, quality > MIPMAP_LIMIT);
  dl->ref();
  dl->open(state);
  this->reallyCreateTexture(state, imageptr, this->numcomponents,
                            newx, newy, dl->getType() == SoGLDisplayList::DISPLAY_LIST,
                            quality > MIPMAP_LIMIT,
                            this->border);
  dl->close(state);
  return dl;
}

// test image data for transparency
void
SoGLImage::checkTransparency(void)
{
  if (this->bytes == NULL) {
    if (this->numcomponents == 2 || this->numcomponents == 4) {
      this->flags = FLAG_TRANSPARENCY;
    }
    else {
      this->flags = 0;
    }
  }
  else {
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
}

static GLenum
translate_wrap(SoState * state, const SoGLImage::Wrap wrap)
{
  if (wrap == SoGLImage::REPEAT) return GL_REPEAT;
  if (wrap == SoGLImage::CLAMP_TO_EDGE) {
#if GL_VERSION_1_2
    return GL_CLAMP_TO_EDGE;
#elif GL_EXT_texture_edge_clamp
    static int texture_clamp_ext = -1;
    if (texture_clamp_ext == -1) {
      texture_clamp_ext = SoGLCacheContextElement::getExtID("GL_EXT_texture_edge_clamp");
    }
    if (SoGLCacheContextElement::extSupported(state, texture_clamp_ext))
      return GL_CLAMP_TO_EDGE_EXT;
#elif GL_SGIS_texture_edge_clamp
    static int texture_clamp_sgis = -1;
    if (texture_clamp_sgis == -1) {
      texture_clamp_sgis = SoGLCacheContextElement::getExtID("SGIS_texture_edge_clamp");
    }
    if (SoGLCacheContextElement::extSupported(state, texture_clamp_sgis))
      return GL_CLAMP_TO_EDGE_SGIS;
#endif // GL_SGIS_texture_edge_clamp
  }
  return GL_CLAMP;
}

void
SoGLImage::reallyCreateTexture(SoState * state,
                               const unsigned char * const texture,
                               const int numComponents,
                               const int w, const int h,
                               const SbBool dlist,
                               const SbBool mipmap,
                               const int border)
{
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                  translate_wrap(state, this->wraps));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                  translate_wrap(state, this->wrapt));
  // set filtering to legal values for non mipmapped texture
  if (!dlist && !mipmap) {
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  }

  GLenum glformat;
  switch (numComponents) {
  case 1:
    glformat = GL_LUMINANCE;
    break;
  case 2:
    glformat = GL_LUMINANCE_ALPHA;
    break;
  case 3:
    glformat = GL_RGB;
    break;
  case 4:
    glformat = GL_RGBA;
    break;
  default:
    assert(0 && "illegal numComonents");
    glformat = GL_RGB; // Unnecessary, but kills a compiler warning.
  }

  if (!mipmap) {
    glTexImage2D(GL_TEXTURE_2D, 0, numComponents, w, h,
                 border, glformat, GL_UNSIGNED_BYTE, texture);
  }
  else { // mipmaps
    // FIXME: ignoring the error code. Silly. 20000929 mortene.
    (void)GLUWrapper()->gluBuild2DMipmaps(GL_TEXTURE_2D, numComponents, w, h,
                                          glformat, GL_UNSIGNED_BYTE, texture);
  }
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

//
// unref all dlists stored in image
//
void
SoGLImage::unrefDLists(SoState * state)
{
  int n = this->dlists.getLength();
  for (int i = 0; i < n; i++) {
    this->dlists[i]->unref(state);
  }
  this->dlists.truncate(0);
}
