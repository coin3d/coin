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

#include <Inventor/SoImageInterface.h>
#include <Inventor/errors/SoDebugError.h>
#include <stdlib.h>
#include <stddef.h>

#include <config.h>

#if defined(HAVE_LIBSIMAGE)
#include <simage.h>
#endif // HAVE_LIBSIMAGE

//
// TODO: write support for other image libraries
//

/*!
  \class SoImageInterface include/Inventor/SoImageInterface.h
  \brief The SoImageInterface class organizes an image file.

  It is a simple class which accepts four types of images.
  The number of components specyfies the image type.
  <li> 1 - Grayscale image, no alpha.</li>
  <li> 2 - Grayscale image with alpha component.</li>
  <li> 3 - RGB image, no alpha.</li>
  <li> 4 - RGBA image.</li>

  This image class is organized using reference counting in
  a similar way to Coin nodes. This way it can be shared
  between, for instance, several nodes.

  This class uses the CoinImage library for loading images.
*/

/*!
  Constructor with \a file_name specifying the path and name of the
  image file.
*/
SoImageInterface::SoImageInterface(const char * const file_name)
  : filename(file_name),
    orgSize(0,0),
    orgNumComponents(0),
    size(0,0),
    numComponents(0),
    dataPtr(NULL),
    refCount(0),
    hasTried(0),
    didAlloc(0)
{
}

/*!
  Constructor with \a size specifying the size of the image,
  \a numComponents specifying the number of components
  per pixel, and \a data specyfying the image data.

  \a data will not be freed when the instance is deleted.
*/
SoImageInterface::SoImageInterface(const SbVec2s size,
                                   const int numComponents,
                                   const unsigned char * data)
{
  this->orgSize = this->size = size;
  this->orgNumComponents = this->numComponents = numComponents;
  this->dataPtr = (unsigned char*)data;
  this->didAlloc = 0; // do not delete this data!
  this->refCount = 0;
  this->hasTried = 1; // data is loaded, kind of...
}

/*!
  Destructor which frees the image data if it was allocated by
  this class.
*/
SoImageInterface::~SoImageInterface()
{
  if (this->didAlloc && this->dataPtr) free(this->dataPtr);
}

/*!
  Increases the reference count for this image.
  \sa SoImageInterface::unref()
*/
void SoImageInterface::ref()
{
  refCount++;
}

/*!
  Decreases the reference count for this image. If the reference
  count equals zero after decreasing, the image is destructed.
  \sa SoImageInterface::ref()
*/
void SoImageInterface::unref()
{
  if (--refCount == 0) {
#if COIN_DEBUG
    if (filename.getLength())
      SoDebugError::postInfo("SoImageInterface::unref",
                             "deleting image: %s\n", filename.getString());
#endif // COIN_DEBUG
    delete this;
  }
}

/*!
  A very lame resize function. No bilinear interpolation when
  scaling up, and no square sum when scaling down. FIXME
*/
SbBool
SoImageInterface::resize(const SbVec2s newsize)
{
  if (newsize != size) {
#if COIN_DEBUG
    SoDebugError::postInfo("SoImageInterface::resize",
                           "(%d): %d %d --> %d %d\n",
                           numComponents,
                           size[0], size[1], newsize[0], newsize[1]);
#endif // COIN_DEBUG
    int num_comp = this->numComponents;
    unsigned char *dest =
      (unsigned char *)malloc(newsize[0]*newsize[1]*num_comp);

    float sx, sy, dx, dy;
    dx = ((float)this->size[0])/((float)newsize[0]);
    dy = ((float)this->size[1])/((float)newsize[1]);
    int src_bpr = this->size[0] * num_comp;
    int dest_bpr = newsize[0] * num_comp;

    unsigned char *src = this->dataPtr; // use local variable for speed

    sy = 0.0f;
    int ystop = newsize[1] * dest_bpr;
    int xstop = newsize[0] * num_comp;
    for (int y = 0; y < ystop; y += dest_bpr) {
      sx = 0.0f;
      for (int x = 0; x < xstop; x += num_comp) {
        int offset = ((int)sy)*src_bpr + ((int)sx)*num_comp;
        for (int i = 0; i < num_comp; i++) dest[x+y+i] = src[offset+i];
        sx += dx;
      }
      sy += dy;
    }

    this->size = newsize;
    if (this->didAlloc && this->dataPtr) free(this->dataPtr);
    this->dataPtr = dest;
    this->didAlloc = 1; // we did alloc this data
  }

  return TRUE;
}

/*!
  If the file isn't already loaded, this method will attempt
  to load the file specified in the constructor.
  If \a forceTry is \e TRUE, it will try to load the file
  even though the file could not be loaded the last the
  this method was called.

  \e TRUE is returned if the image file is loaded, \e FALSE
  otherwise.
*/
SbBool
SoImageInterface::load(const SbBool forceTry)
{
  if (this->dataPtr) return TRUE;
  if (forceTry || !this->hasTried) {
    this->hasTried = TRUE;
    int w, h, nc;
    this->dataPtr = NULL;

    //
    // TODO: code to read images using other image libraries
    // should be inserted here.
    //

#if defined(HAVE_LIBSIMAGE)
    this->dataPtr = simage_read_image(this->filename.getString(),
                                      &w, &h, &nc);
    if (this->dataPtr) {
      this->orgSize = this->size = SbVec2s(w, h);
      this->orgNumComponents = this->numComponents = nc;
      return TRUE;
    }
#endif // HAVE_LIBSIMAGE
  }
  return FALSE;
}

/*!
  Returns \e TRUE if the image contains data
  (is loaded or was constructed with image data).
*/
SbBool
SoImageInterface::isLoaded() const
{
  return (this->dataPtr != NULL);
}

/*!
  Returns a pointer to the image data.
*/
const unsigned char *
SoImageInterface::getDataPtr() const
{
  return this->dataPtr;
}

/*!
  Returns the number of components in the image.
*/
int
SoImageInterface::getNumComponents() const
{
  return this->numComponents;
}

/*!
  Returns the size of the image.
*/
SbVec2s
SoImageInterface::getSize() const
{
  return this->size;
}

/*!
  Returns the filename for this image.
*/
const char *
SoImageInterface::getFilename() const
{
  return this->filename.getString();
}

/*!
  Returns the original number of components of this image.
  Currently it is not possible to change the number of components,
  but this might change in the future.
*/
int
SoImageInterface::getOrgNumComponents() const
{
  return this->orgNumComponents;
}

/*!
  Returns the original size of the image, before any resize.
*/
SbVec2s
SoImageInterface::getOriginalSize() const
{
  return this->orgSize;
}
