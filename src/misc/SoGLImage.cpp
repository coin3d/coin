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

/*!
  \class SoGLImage include/Inventor/misc/SoGLImage.h
  \brief The SoGLImage class is used to control OpenGL textures.
*/

#include <Inventor/misc/SoGLImage.h>
#include <Inventor/misc/SoImageInterface.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/errors/SoDebugError.h>
#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif // _WIN32
#include <GL/gl.h>
#include <Inventor/lists/SbList.h>

// if textureQuality is equal or greater than this, create mipmap
#define MIPMAP_LIMIT 0.5f

//
// private constructor
//
SoGLImage::SoGLImage(SoImageInterface * const img,
                     const SbBool clamps,
                     const SbBool clampt,
                     const float quality,
                     void * const context)
{
  this->image = img;
  this->handle = 0;
  this->clampS = clamps;
  this->clampT = clampt;
  this->quality = quality;
  this->refCount = 0;
  this->context = context;
  if (this->image) this->image->ref();
}

//
// private destructor
//
SoGLImage::~SoGLImage()
{
  if (this->handle) sogl_free_texture(this->handle);
  if (this->image) this->image->unref();
}

/*!
  Checks whether the GL object matches the parameters. If not, you'll
  have to unref this GLImage and create a new one.

  This will rarely happen, since this is the kind of variables
  that should not change very often.
*/
SbBool
SoGLImage::matches(const SbBool clamps, const SbBool clampt) const
{
  return
    this->clampS == clamps &&
    this->clampT == clampt;
}

/*!
  Unreferences the texture. Make sure the OpenGL context using
  this texture is the current GL context.

  An object using a GL image should call this method when the
  object is not going to use the texture ant more. The reference
  counting will make sure a GL image is not freed until all
  objects using the image has called this method.
*/
void
SoGLImage::unref()
{
  SoGLImage::unrefGLImage(this);
}

/*!
  Makes this texture the current OpenGL texture.
*/
void
SoGLImage::apply(const float quality) const
{
  sogl_apply_texture(this->handle);
  if (quality < 0.1f) {
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
  Returns the OpenGL handle for this texture. An OpenGL handle
  will either be an OpenGL texture object or a displat list
  for old OpenGL implementations.
*/
int
SoGLImage::getHandle() const
{
  return this->handle;
}

/*!
  Returns \e TRUE if this texture has some pixels with alpha != 255
*/
SbBool
SoGLImage::hasTransparency() const
{
  assert(this->image);
  return this->image->hasTransparency();
}

/*!
  Returns \e TRUE if texture coordinates should be clamped in
  the s-direction.
*/
SbBool
SoGLImage::shouldClampS() const
{
  return this->clampS;
}

/*!
  Returns \e TRUE if texture coordinates should be clamped in
  the t-direction.
*/
SbBool
SoGLImage::shouldClampT() const
{
  return this->clampT;
}

/*!
  Returns the image data for this OpenGL texture.
*/
const SoImageInterface *
SoGLImage::getImage() const
{
  return this->image;
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
SoGLImage::getQuality() const
{
  return this->quality;
}

//
// private method that initializes the GL texture object/display list.
//
SbBool
SoGLImage::GLinit()
{
  if (this->handle) return TRUE;
  if (this->image && this->image->load()) {
    this->checkResize(); // resize if necessary
    SbVec2s size = image->getSize();
    int format = image->getNumComponents();
    this->handle = sogl_create_texture(this->clampS, this->clampT,
                                       this->image->getDataPtr(),
                                       format, size[0], size[1],
                                       this->quality >= MIPMAP_LIMIT);
    return this->handle != 0;
  }
  return FALSE;
}

//
// some helpful functions
//
static int
cnt_bits(unsigned long val, int &highbit)
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

static unsigned long
nearest_binary(unsigned long val)
{
  int highbit;
  if (cnt_bits(val, highbit) > 1) {
    return 1<<highbit;
  }
  return val;
}

//
// private method that tests the size of the image, and
// performs an resize if the size is not "binary".
//
void
SoGLImage::checkResize()
{
  SbVec2s size = this->image->getSize();
  int xsize = size[0];
  int ysize = size[1];
  unsigned long newx = nearest_binary(xsize);
  unsigned long newy = nearest_binary(ysize);

  // if >= 256, don't scale up unless size is close to an above binary
  // this saves a lot of texture memory
  if (newx >= 256) {
    if ((newx - xsize) > (newx>>3)) newx >>= 1;
  }
  if (newy >= 256) {
    if ((newy - ysize) > (newy>>3)) newy >>= 1;
  }

  // downscale until legal GL size (implementation dependant)
  // e.g. old 3dfx hardware only allows up to 256x256 textures.

  unsigned long maxsize = sogl_max_texture_size();
  while (newx > maxsize) newx >>= 1;
  while (newy > maxsize) newy >>= 1;

  if (newx != (unsigned long) xsize || newy != (unsigned long) ysize) {
    this->image->resize(SbVec2s((short)newx, (short)newy));
  }
}

/**** some static methods needed to reuse GL images *********/

static SbList <SoGLImage *> storedImages;

/*!
  Searches the texture database and returns a texture object that
  matches all parameters. If no such texture is found, a new texture
  object is created and returned.

  It is currently not possible to share textures between contexts, but
  this will be implemented at a later stage.

  \a clamps and \a clampt specifies whether texture coordininates
  should be clamped outside 0 and 1. \a qualiy specifies the
  texture quality. A value of 0 means use lowest quality texture, a
  value of 1 means use maximum quality possible. Exactly what this
  means might vary from platform to platform. Returns \e TRUE on
  success.

  The OpenGL context that is going to use this texture must
  be the current GL context when calling this method.
*/
SoGLImage *
SoGLImage::findOrCreateGLImage(SoImageInterface * const image,
                               const SbBool clamps,
                               const SbBool clampt,
                               const float quality,
                               void * const context)
{
  int i, n = storedImages.getLength();
  for (i = 0; i < n; i++) {
    SoGLImage *glimage = storedImages[i];
    if (glimage->image == image &&
        glimage->context == context &&
        glimage->clampS == clamps &&
        glimage->clampT == clampt &&
        glimage->quality == quality) break;
  }
  if (i < n) {
    storedImages[i]->refCount++;
    return storedImages[i];
  }
  else {
    SoGLImage *glimage = new SoGLImage(image, clamps, clampt, quality, context);
    glimage->GLinit();
    glimage->refCount++;
    storedImages.append(glimage);
    return glimage;
  }
}


void
SoGLImage::unrefGLImage(SoGLImage * const image)
{
  int i, n = storedImages.getLength();
  for (i = 0; i < n; i++) {
    if (storedImages[i] == image) break;
  }
  assert(i < n);
  if (image->refCount == 1) {
    storedImages.removeFast(i);
    delete image;
  }
  else image->refCount--;
}
