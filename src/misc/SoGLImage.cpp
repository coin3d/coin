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

/*!
  \class SoGLImage include/Inventor/misc/SoGLImage.h
  \brief The SoGLImage class is used to control OpenGL textures.
*/

#include <Inventor/misc/SoGLImage.h>
#include <Inventor/SoImageInterface.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/errors/SoDebugError.h>
#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif // _WIN32
#include <GL/gl.h>
#include <Inventor/lists/SbList.h>

//
// private constructor
//
SoGLImage::SoGLImage(SoImageInterface * const img,
                     void * const context)
{
  this->image = img;
  this->handle = 0;
  this->alpha = 0;
  this->clampS = 0;
  this->clampT = 0;
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
  Initializes the GL image. \a clamps and \a clampt specifies
  whether texture coordininates should be clamped outside
  0 and 1. Returns \e TRUE on success.

  The OpenGL context that is going to use this texture must
  be the current GL context.
*/
SbBool
SoGLImage::init(const SbBool clamps, const SbBool clampt)
{
  if (this->handle) return TRUE;
  if (this->image && this->image->load()) {
    checkResize(); // resize if necessary
    SbVec2s size = image->getSize();
    int format = image->getNumComponents();

    this->handle = sogl_create_texture(clamps, clampt,
                                       this->image->getDataPtr(),
                                       format, size[0], size[1]);
    if (format == 2 || format == 4) this->alpha = TRUE;
    else this->alpha = FALSE;
    return TRUE;
  }
  return FALSE;
}

/*!
  Returns \e TRUE if the GL image has been initialized.
*/
SbBool
SoGLImage::isInitialized() const
{
  return this->handle != 0;
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
    this->image->resize(SbVec2s(newx, newy));
  }
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
SoGLImage::apply() const
{
  //  assert(this->handle);
  sogl_apply_texture(this->handle);
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
  Returns \e TRUE if this texture has an alpha component.
*/
SbBool
SoGLImage::hasAlphaComponent() const
{
  return this->alpha == 1;
}

/*!
  Returns \e TRUE if texture coordinates should be clamped in
  the s-direction.
*/
SbBool
SoGLImage::shouldClampS() const
{
  return this->clampS == 1;
}

/*!
  Returns \e TRUE if texture coordinates should be clamped in
  the t-direction.
*/
SbBool
SoGLImage::shouldClampT() const
{
  return this->clampT == 1;
}

/*!
  Returns the image data for this OpenGL texture.
*/
const SoImageInterface *
SoGLImage::getImage() const
{
  return this->image;
}

/**** some static methods needed to reuse GL images *********/

static SbList <SoGLImage *> storedImages;

/*!
  Searches the texture database and returns a texture object
  that matches \a texname and \a context. If no such texture
  is found, a new texture object is created and returned.

  It is currently not possible to share textures between
  contexts, but this will be implemented at a later stage.
*/
SoGLImage *
SoGLImage::findOrCreateGLImage(SoImageInterface * const image,
                               void * const context)
{
  int i, n = storedImages.getLength();
  for (i = 0; i < n; i++) {
    SoGLImage *glimage = storedImages[i];
    if (glimage->image == image && glimage->context == context) break;
  }
  if (i < n) {
    storedImages[i]->refCount++;
    return storedImages[i];
  }
  else {
    SoGLImage *glimage = new SoGLImage(image, context);
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
